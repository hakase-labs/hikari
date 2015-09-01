#include "hikari/client/audio/AudioService.hpp"
#include "hikari/client/audio/SoundLibrary.hpp"
#include "hikari/client/audio/GMESoundStream.hpp"

#include <json/value.h>

namespace hikari {

    const unsigned int AudioService::MUSIC_BUFFER_SIZE = 2048 * 2;  // some platforms need larger buffer
    const unsigned int AudioService::SAMPLE_BUFFER_SIZE = 2048 * 2; // so we'll double it for now.
    const unsigned int AudioService::AUDIO_SAMPLE_RATE = 44000;
    const float AudioService::DEFAULT_VOLUME = 100.0f;

    AudioService::AudioService(const Json::Value &configuration)
        : musicLoaded(false)
        , samplesLoaded(false)
        , mutedFlag(false)
        , sampleVolume(DEFAULT_VOLUME)
        , musicVolume(DEFAULT_VOLUME)
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

    void AudioService::playMusic(const std::string & name) {
        if(library->isEnabled()) {
            const auto stream = library->playMusic(name, getMusicVolume());
        }
    }

    void AudioService::stopMusic() {
        library->stopMusic();
    }

    void AudioService::setMusicVolume(float volume) {
        musicVolume = volume;

        if(library->isEnabled()) {
            library->setMusicVolume(musicVolume);
        }
    }

    float AudioService::getMusicVolume() const {
        return isMuted() ? 0.0f : musicVolume;
    }

    void AudioService::playSample(const std::string & name) {
        if(library->isEnabled()) {
            const auto stream = library->playSample(name, getSampleVolume());
        }
    }

    void AudioService::setSampleVolume(float volume) {
        sampleVolume = volume;

        if(library->isEnabled()) {
            library->setSampleVolume(sampleVolume);
        }
    }

    float AudioService::getSampleVolume() const {
        return isMuted() ? 0.0f : sampleVolume;
    }

    void AudioService::stopAllSamples() {
        library->stopSample();
    }

    bool AudioService::isMusicLoaded() const {
        return musicLoaded;
    }

    bool AudioService::isSamplesLoaded() const {
        return samplesLoaded;
    }

    void AudioService::mute() {
        mutedFlag = true;

        if(library->isEnabled()) {
            library->setMusicVolume(getMusicVolume());
            library->setSampleVolume(getSampleVolume());
        }
    }

    void AudioService::unmute() {
        mutedFlag = false;

        if(library->isEnabled()) {
            library->setMusicVolume(getMusicVolume());
            library->setSampleVolume(getSampleVolume());
        }
    }

    bool AudioService::isMuted() const {
        return mutedFlag;
    }

} // hikari