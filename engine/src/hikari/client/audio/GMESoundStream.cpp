#include "hikari/client/audio/GMESoundStream.hpp"
#include "hikari/core/util/FileSystem.hpp"
#include "hikari/core/util/StringUtils.hpp"
#include "hikari/core/util/PhysFS.hpp"
#include "hikari/core/util/Log.hpp"
#include <Music_Emu.h>
#include <gme.h>
#include <iostream>

namespace hikari {

    GMESoundStream::GMESoundStream(std::size_t bufferSize) : 
    sf::SoundStream(),
    myBufferSize(bufferSize), 
    myBuffer    (new short[myBufferSize]) 
    {
        this->stop();
    }

    GMESoundStream::~GMESoundStream() {
        this->stop();
    }

    bool GMESoundStream::open(const std::string& fileName) {
        sf::Lock lock(mutex);

        int length = 0;
        auto fs = FileSystem::openFileRead(fileName);

            
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
        setCurrentTrack(0);

        int count = emu->track_count();
        for(int i = 0; i < count; i++) {
            track_info_t info;
            
            emu->track_info(&info, i);

            std::string song(info.song);
            std::string comment(info.comment);
            std::string length(StringUtils::toString<long>(info.length));

            std::cout << i << '\t' << song << '\t' << comment << '\t' << length << std::endl;
        }

        return true;
    }

    void GMESoundStream::onSeek(sf::Time timeOffset) {
        sf::Lock lock(mutex);
        handleError(emu->seek(static_cast<long>(timeOffset.asMilliseconds())));
    }

    bool GMESoundStream::onGetData(sf::SoundStream::Chunk& Data) {
        sf::Lock lock(mutex);

        handleError(emu->play(myBufferSize, myBuffer.get()));

        Data.samples     = &myBuffer[0]; 
        Data.sampleCount = myBufferSize;

        if(!emu->track_ended()) { 
            return true;
        } else {
            return false;
        }
    }

    void GMESoundStream::handleError(const char* str) const {
        if(str) {
            throw std::runtime_error(str);
        }
    }

    long GMESoundStream::getSampleRate() const {
        return emu->sample_rate();
    }

    int GMESoundStream::getCurrentTrack() const {
        return emu->current_track();
    }

    void GMESoundStream::setCurrentTrack(int track) {
        sf::Lock lock(mutex);
        if(track >= 0 && track < getTrackCount()) {
            emu->start_track(track);
        }
    }

    int GMESoundStream::getTrackCount() const {
        return emu->track_count();
    }

    const std::string GMESoundStream::getTrackName() {
        sf::Lock lock(mutex);
        handleError(emu->track_info(trackInfo.get()));
        return std::string(trackInfo->song); 
    }

    int GMESoundStream::getVoiceCount() const {
        return emu->voice_count();
    }

    std::vector<std::string> GMESoundStream::getVoiceNames() const {
        return std::vector<std::string>(
            emu->voice_names(), emu->voice_names() + getVoiceCount());
    }

    std::unique_ptr<sf::SoundBuffer> GMESoundStream::renderTrackToBuffer(int track) {
        sf::Lock lock(mutex);
        std::unique_ptr<sf::SoundBuffer> buffer(new sf::SoundBuffer);
        std::vector<short> samples;

        emu->start_track(track);

        std::size_t bufferSize = 512;

        // TODO: Be careful that you don't try to render a looping track
        //       or this could get really messy...
        while(!emu->track_ended()) {
            handleError(emu->play(bufferSize, myBuffer.get()));

            for(int i = 0; i < bufferSize; ++i) {
                samples.push_back(myBuffer[i]);
            }
        }

        bool loaded = buffer->loadFromSamples(
            &samples[0],    // Spec gaurantees that std::vector's memory is contiguous
            samples.size(),
            2,
            SAMPLE_RATE
        );

        HIKARI_LOG(debug) << "DID IT LOAD? " << loaded << ", size = " << samples.size();

        return buffer;
    }

} // hikari
