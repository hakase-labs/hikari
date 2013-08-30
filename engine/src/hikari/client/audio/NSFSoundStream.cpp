#include "hikari/client/audio/NSFSoundStream.hpp"
#include "hikari/core/util/FileSystem.hpp"
#include "hikari/core/util/PhysFS.hpp"
#include "hikari/core/util/Log.hpp"
#include <Music_Emu.h>
#include <gme.h>
#include <iostream>

namespace hikari {

    NSFSoundStream::NSFSoundStream(std::size_t bufferSize, std::size_t samplerCount)
        : sf::SoundStream()
        , masterBufferSize(bufferSize)
        , samplerCount(samplerCount)
        // , activeSampler(0)
        , masterBuffer(new short[masterBufferSize])
        , sampleBuffers()
        , sampleEmus()
        , trackInfo(nullptr)
        , mutex()
    {
        samplerCount = std::max(samplerCount, static_cast<std::size_t>(1));
        this->samplerCount = samplerCount;

        createSampleBuffers();

        this->stop();
    }

    NSFSoundStream::~NSFSoundStream() {
        this->stop();
    }

    bool NSFSoundStream::open(const std::string& fileName) {
        sf::Lock lock(mutex);

        if(!FileSystem::exists(fileName)) {
            return false;
        }

        int length = 0;
        auto fs = FileSystem::openFileRead(fileName);

        fs->seekg (0, std::ios::end);
        length = static_cast<int>(fs->tellg());
        fs->seekg (0, std::ios::beg);

        std::unique_ptr<char[]> nsfFileBuffer(new char[length]);

        fs->read(nsfFileBuffer.get(), length);

        // To honor the contract of returning false on failure,
        // catch these exceptions and return false instead? Good/bad?
        try {
            gme_type_t file_type = gme_identify_extension(fileName.c_str());

            if(!file_type) {
                return false;
            }

            for(std::size_t i = 0; i < samplerCount; ++i) {
                auto sampleEmu = std::shared_ptr<Music_Emu>(file_type->new_emu());

                if(!sampleEmu) {
                    return false;
                }

                // Must set sample rate before loading data
                handleError(sampleEmu->set_sample_rate(SAMPLE_RATE));
                handleError(gme_load_data(sampleEmu.get(), nsfFileBuffer.get(), length));

                sampleEmu->start_track(-1);
                sampleEmu->ignore_silence(false);

                auto sampleBuffer = std::make_shared<std::vector<short>>(masterBufferSize);
                std::fill(std::begin(*sampleBuffer), std::end(*sampleBuffer), 0);
                availableSamplers.push(std::make_pair(sampleEmu, sampleBuffer));
                // sampleEmus.push_back(std::move(sampleEmu));
            }

            trackInfo.reset(new track_info_t());
        } catch(std::runtime_error& ex) {
            HIKARI_LOG(debug) << ex.what();
            return false;
        }

        initialize(2, SAMPLE_RATE);
        //setCurrentTrack(0);

        return true;
    }

    void NSFSoundStream::onSeek(sf::Time timeOffset) {
        sf::Lock lock(mutex);

        if(!activeSamplers.empty()) {
            auto & sampler = activeSamplers.front();
            auto & emu = sampler.first;

            handleError(emu->seek(static_cast<long>(timeOffset.asMilliseconds())));
        }
    }

    bool NSFSoundStream::onGetData(sf::SoundStream::Chunk& Data) {
        sf::Lock lock(mutex);

        auto * mixedBuffer = masterBuffer.get();

        // Generate samples for any playing emulators
        // for(int bufferIndex = 0; bufferIndex < samplerCount; ++bufferIndex) {
        //     if(!sampleEmus[bufferIndex]->track_ended()) {
        //         sampleEmus[bufferIndex]->play(masterBufferSize, sampleBuffers[bufferIndex].get());
        //     }
        // }
        activeSamplers.remove_if([&](const SamplerPair & pair) -> bool {
            bool ended = (pair.first)->track_ended();
            int track = (pair.first)->current_track();

            if(ended) {
                availableSamplers.push(pair);
                samplerSlots.erase(track);
            }

            return ended;
        });

        std::for_each(std::begin(activeSamplers), std::end(activeSamplers), [&](SamplerPair & pair) {
            auto & emu = pair.first;
            auto & buffer = *pair.second;

            if(!emu->track_ended()) {
                emu->play(masterBufferSize, &buffer[0]);
            } else {
                //availableSamplers.push(pair);
            }
        });

        for(std::size_t i = 0; i < masterBufferSize; ++i) {
            short mixedValue = 65535;

            std::for_each(std::begin(activeSamplers), std::end(activeSamplers), [&](SamplerPair & pair) {
                auto & emu = pair.first;
                auto & buffer = *pair.second;

                if(!emu->track_ended()) {
                    // This mixing strategy came from: http://www.vttoth.com/CMS/index.php/technical-notes/68
                    // And supporting information from: http://cboard.cprogramming.com/c-programming/103456-mixing-pcm-samples-dealing-clicks-overflow.html
                    //mixedValue = mixedValue + buffer[i] - ((mixedValue * buffer[i]) / 65535);

                    // Method dreived from: http://stackoverflow.com/a/10029792
                    float a = (mixedValue + 32767.0f) / 65536.0f;
                    float b = (buffer[i] + 32767.0f) / 65536.0f;

                    mixedValue = static_cast<short>((a + b) * 65536.0f);
                } else {
                    buffer[i] = 0;
                }
            });

            mixedBuffer[i] = mixedValue;
        }

        // HIKARI_LOG(debug2) << "Active samplers: " <<activeSamplers.size();

        // Mix buffers
        // for(int i = 0; i < masterBufferSize; ++i) {
        //     short mixedValue = 0;

        //     for(int bufferIndex = 0; bufferIndex < samplerCount; ++bufferIndex) {
        //         // Don't mix buffers that aren't playing.
        //         if(!sampleEmus[bufferIndex]->track_ended()) {
        //             mixedValue += sampleBuffers[bufferIndex].get()[i];
        //         } else {
        //             // Zero-out any buffer that isn't playing.
        //             sampleBuffers[bufferIndex].get()[i] = 0;
        //         }
        //     }

        //     mixedBuffer[i] = mixedValue;
        // }

        Data.samples     = &masterBuffer[0];
        Data.sampleCount = masterBufferSize;

        // Never stop streaming...
        return true;
    }

    void NSFSoundStream::handleError(const char* str) const {
        if(str) {
            throw std::runtime_error(str);
        }
    }

    void NSFSoundStream::createSampleBuffers() {
        sampleBuffers.clear();

        for(std::size_t i = 0; i < samplerCount; ++i) {
            auto sampleBuffer = std::unique_ptr<short[]>(new short[masterBufferSize]);

            std::fill(sampleBuffer.get(), sampleBuffer.get() + masterBufferSize, 0);

            sampleBuffers.push_back(std::move(sampleBuffer));
        }
    }

    long NSFSoundStream::getSampleRate() const {
        return sampleEmus[0]->sample_rate();
    }

    int NSFSoundStream::getCurrentTrack() const {
        return sampleEmus[0]->current_track();
    }

    void NSFSoundStream::setCurrentTrack(int track) {
        sf::Lock lock(mutex);

        if(track >= 0 && track < getTrackCount()) {
            // sampleEmus[activeSampler]->start_track(track);
            // activeSampler++;
            // activeSampler %= samplerCount;
            if(samplerSlots.count(track) > 0) {
                auto sampler = samplerSlots.at(track);
                (sampler.first)->start_track(track);
            } else if(!availableSamplers.empty()) {
                auto sampler = availableSamplers.top();
                availableSamplers.pop();
                (sampler.first)->start_track(track);
                activeSamplers.push_back(sampler);
                samplerSlots[track] = sampler;
            }
        }
    }

    int NSFSoundStream::getTrackCount() const {
        int trackCount = 0;

        if(!availableSamplers.empty()) {
            auto & sampler = availableSamplers.top();
            auto & emu = sampler.first;

            trackCount = emu->track_count();
        }

        return trackCount; // sampleEmus[0]->track_count();
    }

    void NSFSoundStream::stopAllSamplers() {
        activeSamplers.remove_if([&](const SamplerPair & pair) -> bool {
            int track = (pair.first)->current_track();

            availableSamplers.push(pair);
            samplerSlots.erase(track);

            return true;
        });
    }

    const std::string NSFSoundStream::getTrackName() {
        sf::Lock lock(mutex);
        handleError(sampleEmus[0]->track_info(trackInfo.get()));
        return std::string(trackInfo->song);
    }

    int NSFSoundStream::getVoiceCount() const {
        return sampleEmus[0]->voice_count();
    }

    std::vector<std::string> NSFSoundStream::getVoiceNames() const {
        return std::vector<std::string>(
            sampleEmus[0]->voice_names(), sampleEmus[0]->voice_names() + getVoiceCount());
    }

} // hikari
