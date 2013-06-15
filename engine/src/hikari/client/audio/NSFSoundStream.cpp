#include "hikari/client/audio/NSFSoundStream.hpp"
#include "hikari/core/util/FileSystem.hpp"
#include "hikari/core/util/PhysFS.hpp"
#include "hikari/core/util/Log.hpp"
#include <Music_Emu.h>
#include <gme.h>
#include <iostream>

namespace hikari {

    NSFSoundStream::NSFSoundStream(std::size_t bufferSize, unsigned int samplerCount) :
    sf::SoundStream(),
    myBufferSize(bufferSize),
    myBuffer    (new short[myBufferSize]),
    emu         (nullptr),
    trackInfo   (nullptr),
    samplerCount(samplerCount),
    activeSampler(0),
    mutex       ()
    {
        activeSampler = 0;

        for(int i = 0; i < samplerCount; ++i) {
            sampleBuffers.push_back(std::unique_ptr<short[]>(new short[myBufferSize]));
        }

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
        auto fs = FileSystem::openFile(fileName);

        fs->seekg (0, std::ios::end);
        length = static_cast<int>(fs->tellg());
        fs->seekg (0, std::ios::beg);

        std::unique_ptr<char[]> buffer(new char[length]);

        fs->read(buffer.get(), length);

        // To honor the contract of returning false on failure,
        // catch these exceptions and return false instead? Good/bad?
        try {
            //HandleError(gme_identify_file(fileName.c_str(), &file_type));
            gme_type_t file_type = gme_identify_extension(fileName.c_str());
            if(!file_type) {
                return false;
                //throw std::runtime_error("Unsupported music type");
            }

            emu.reset(file_type->new_emu());

            for(int i = 0; i < samplerCount; ++i) {
                auto sampleEmu = std::unique_ptr<Music_Emu>(file_type->new_emu());

                sampleEmu->set_sample_rate(SAMPLE_RATE);
                gme_load_data(sampleEmu.get(), buffer.get(), length);
                sampleEmu->start_track(-1);
                sampleEmu->ignore_silence(false);
                sampleEmus.push_back(std::move(sampleEmu));
            }

            trackInfo.reset(new track_info_t());

            if(!emu.get()) {
                return false;
                //throw std::runtime_error("Out of memory");
            }

            handleError(emu->set_sample_rate(SAMPLE_RATE));
            handleError(gme_load_data(emu.get(), buffer.get(), length));
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
        handleError(emu->seek(static_cast<long>(timeOffset.asMilliseconds())));
    }

    bool NSFSoundStream::onGetData(sf::SoundStream::Chunk& Data) {
        sf::Lock lock(mutex);

        auto * mixedBuffer = myBuffer.get();

         for(int bufferIndex = 0; bufferIndex < samplerCount; ++bufferIndex) {
            if(!sampleEmus[bufferIndex]->track_ended()) {
                sampleEmus[bufferIndex]->play(myBufferSize, sampleBuffers[bufferIndex].get());
            }
        }

        // Mix buffers
        for(int i = 0; i < myBufferSize; ++i) {
            short mixedValue = 0;

            for(int bufferIndex = 0; bufferIndex < samplerCount; ++bufferIndex) {
                // Don't mix buffers that aren't playing.
                if(!sampleEmus[bufferIndex]->track_ended()) {
                    mixedValue += sampleBuffers[bufferIndex].get()[i];
                } else {
                    // Zero-out any buffer that isn't playing.
                    //sampleBuffers[bufferIndex].get()[i] = 0;
                }
            }

            mixedBuffer[i] = mixedValue;
        }

        // HIKARI_LOG(debug) << "onGetData";

        Data.samples   = &myBuffer[0];
        Data.sampleCount = myBufferSize;

        //if(!emu->track_ended()) {
            // HIKARI_LOG(debug) << "onGetData ... track not over";
            return true;
        //} else {
            // HIKARI_LOG(debug) << "onGetData ... track over";
        //    return false;
        //}
    }

    void NSFSoundStream::handleError(const char* str) const {
        if(str) {
            throw std::runtime_error(str);
        }
    }

    long NSFSoundStream::getSampleRate() const {
        return emu->sample_rate();
    }

    int NSFSoundStream::getCurrentTrack() const {
        return emu->current_track();
    }

    void NSFSoundStream::setCurrentTrack(int track) {
        sf::Lock lock(mutex);
        if(track >= 0 && track < getTrackCount()) {
            emu->start_track(track);

            sampleEmus[activeSampler]->start_track(track);

            activeSampler++;
            activeSampler %= samplerCount;
        }
    }

    int NSFSoundStream::getTrackCount() const {
        return emu->track_count();
    }

    const std::string NSFSoundStream::getTrackName() {
        sf::Lock lock(mutex);
        handleError(emu->track_info(trackInfo.get()));
        return std::string(trackInfo->song);
    }

    int NSFSoundStream::getVoiceCount() const {
        return emu->voice_count();
    }

    std::vector<std::string> NSFSoundStream::getVoiceNames() const {
        return std::vector<std::string>(
            emu->voice_names(), emu->voice_names() + getVoiceCount());
    }

} // hikari
