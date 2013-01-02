#include <hikari/client/audio/AudioService.hpp>

#include <json/value.h>

namespace hikari {

    const unsigned int AudioService::MUSIC_BUFFER_SIZE = 2048;
    const unsigned int AudioService::SAMPLE_BUFFER_SIZE = 2048;
    const unsigned int AudioService::AUDIO_SAMPLE_RATE = 44000;

    AudioService::AudioService(const Json::Value &configuration)
        : musicLoaded(false)
        , samplesLoaded(false)
        , musicStream(MUSIC_BUFFER_SIZE)
        , sampleStream(SAMPLE_BUFFER_SIZE) 
    {
        if(isValidConfiguration(configuration)) {
            auto musicDataFilePath = configuration["music"].asString();
            musicLoaded = musicStream.open(musicDataFilePath);

            auto samplesDataFilePath = configuration["samples"].asString();
            samplesLoaded = sampleStream.open(samplesDataFilePath);

            // TODO: Map/hash samples and music?
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
        bool valid = false;

        valid = configuration.isMember("music") 
                && configuration.isMember("samples") 
                && configuration["music"].isString() 
                && configuration["samples"].isString();

        return valid;
    }

    void AudioService::playMusic(MusicId id) {
        if(isMusicLoaded()) {
            musicStream.stop();
            musicStream.setCurrentTrack(id);
            musicStream.play();
        }
    }

    void AudioService::stopMusic() {
        musicStream.stop();
    }

    void AudioService::playSample(SampleId id) {
        if(isSamplesLoaded()) {
            sampleStream.stop();
            sampleStream.setCurrentTrack(id);
            sampleStream.play();
        }
    }

    void AudioService::stopAllSamples() {
        sampleStream.stop();
    }

    bool AudioService::isMusicLoaded() const {
        return musicLoaded;
    }

    bool AudioService::isSamplesLoaded() const {
        return samplesLoaded;
    }

} // hikari