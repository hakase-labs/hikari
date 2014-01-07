#include "hikari/client/audio/AudioService.hpp"
#include "hikari/client/audio/SoundLibrary.hpp"
#include "hikari/client/audio/GMESoundStream.hpp"

#include <json/value.h>

namespace hikari {

    const unsigned int AudioService::MUSIC_BUFFER_SIZE = 2048 * 2;  // some platforms need larger buffer
    const unsigned int AudioService::SAMPLE_BUFFER_SIZE = 2048 * 2; // so we'll double it for now.
    const unsigned int AudioService::AUDIO_SAMPLE_RATE = 44000;

    AudioService::AudioService(const Json::Value &configuration)
        : musicLoaded(false)
        , samplesLoaded(false)
        , enabledFlag(true)
        , musicStream(MUSIC_BUFFER_SIZE, 1)
        , sampleStream(SAMPLE_BUFFER_SIZE, 12)
        , library(nullptr)
    {
        if(isValidConfiguration(configuration)) {
            auto musicDataFilePath = configuration["music"].asString();
            musicLoaded = musicStream.open(musicDataFilePath);

            auto samplesDataFilePath = configuration["samples"].asString();
            samplesLoaded = sampleStream.open(samplesDataFilePath);

            library.reset(new SoundLibrary(configuration["library"].asString()));
        }
    }

    AudioService::~AudioService() {
        if(isMusicLoaded()) {
            musicStream.stop();
        }

        if(isSamplesLoaded()) {
            sampleStream.stop();
        }
    }

    bool AudioService::isValidConfiguration(const Json::Value &configuration) const {
        bool valid = configuration.isMember("music") 
                && configuration.isMember("samples") 
                && configuration.isMember("library")
                && configuration["music"].isString() 
                && configuration["samples"].isString()
                && configuration["library"].isString();

        return valid;
    }

    void AudioService::playMusic(MusicId id) {
        // if(isEnabled() && isMusicLoaded()) {
        //     musicStream.stop();
        //     musicStream.setCurrentTrack(id);
        //     musicStream.play();
        // }
    }

    void AudioService::playMusic(const std::string & name) {
        if(isEnabled() && library->isEnabled()) {
            const auto stream = library->playMusic(name);

            if(stream) {
                stream->play();
            }
        }
    }

    void AudioService::stopMusic() {
        // musicStream.stopAllSamplers();
        // musicStream.stop();
        library->stopMusic();
    }

    void AudioService::playSample(SampleId id) {
        // if(isEnabled() && isSamplesLoaded()) {
        //     sampleStream.setCurrentTrack(id);
        //     sampleStream.play();
        // }
    }

    void AudioService::playSample(const std::string & name) {
        if(isEnabled() && library->isEnabled()) {
            const auto stream = library->playSample(name);

            if(stream) {
                stream->play();
            }
        }
    }


    void AudioService::stopAllSamples() {
        // sampleStream.stop();
        library->stopSample();
    }

    bool AudioService::isMusicLoaded() const {
        return musicLoaded;
    }

    bool AudioService::isSamplesLoaded() const {
        return samplesLoaded;
    }

    void AudioService::disable() {
        enabledFlag = false;

        stopAllSamples();
        stopMusic();
    }

    void AudioService::enable() {
        enabledFlag = true;
    }

    bool AudioService::isEnabled() const {
        return enabledFlag;
    }

} // hikari