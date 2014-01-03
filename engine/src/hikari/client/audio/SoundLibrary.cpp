#include "hikari/client/audio/SoundLibrary.hpp"
#include "hikari/client/audio/NSFSoundStream.hpp"

#include <json/reader.h>
#include <json/value.h>

namespace hikari {
    
    SoundLibrary::SoundLibrary(const std::string & file)
        : isEnabledFlag(false)
        , file(file)
        , music()
        , samples()
        , samplers() {

    }

    void SoundLibrary::loadLibrary() {
        isEnabledFlag = true;
    }

    bool SoundLibrary::isEnabled() const {
        return isEnabledFlag;
    }

    void SoundLibrary::addMusic(const std::string & name, MusicEntry entry) {

    }

    void SoundLibrary::addSample(const std::string & name, SampleEntry entry) {

    }

    void SoundLibrary::playMusic(const std::string & name) {

    }

    void SoundLibrary::playSample(const std::string & name) {

    }

    void SoundLibrary::stopMusic() {

    }

    void SoundLibrary::stopSample() {

    }

} // hikari
