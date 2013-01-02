#include <hikari/client/audio/NSFSoundStream.hpp>
#include <hikari/core/util/FileSystem.hpp>
#include <hikari/core/util/PhysFS.hpp>
#include <hikari/core/util/Log.hpp>
#include <Music_Emu.h>
#include <gme.h>
#include <iostream>

namespace hikari {

    NSFSoundStream::NSFSoundStream(std::size_t bufferSize) : 
    sf::SoundStream(),
    myBufferSize(bufferSize), 
    myBuffer    (new short[myBufferSize]) 
    {
        this->stop();
    }

    NSFSoundStream::~NSFSoundStream() {
        this->stop();
    }

    bool NSFSoundStream::open(const std::string& fileName) {
        sf::Lock lock(mutex);

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

        /*int count = emu->track_count();
        for(int i = 0; i < count; i++) {
            track_info_t info;
            
            emu->track_info(&info, i);

            std::string song(info.song);
            std::string comment(info.comment);

            std::cout << i << '\t' <<
                song << '\t' << comment << std::endl;
        }*/

        return true;
    }

    void NSFSoundStream::onSeek(sf::Time timeOffset) {
        sf::Lock lock(mutex);
        handleError(emu->seek(static_cast<long>(timeOffset.asMilliseconds())));
    }

    bool NSFSoundStream::onGetData(sf::SoundStream::Chunk& Data) {
        sf::Lock lock(mutex);
        handleError(emu->play(myBufferSize, myBuffer.get()));

        Data.samples   = &myBuffer[0]; 
        Data.sampleCount = myBufferSize;

        if(!emu->track_ended()) { 
            return true;
        } else {
            return false;
        }
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