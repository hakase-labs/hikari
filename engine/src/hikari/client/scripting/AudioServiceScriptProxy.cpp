#include "hikari/client/scripting/AudioServiceScriptProxy.hpp"
#include "hikari/client/audio/AudioService.hpp"

namespace hikari {

    std::weak_ptr<AudioService> AudioServiceScriptProxy::audioService = std::weak_ptr<AudioService>();

    AudioServiceScriptProxy::AudioServiceScriptProxy() {
        // Private to prevent construction
    }

    AudioServiceScriptProxy::AudioServiceScriptProxy(const AudioServiceScriptProxy& other) {
        // Private to prevent copy construction
    }

    std::weak_ptr<AudioService> AudioServiceScriptProxy::getWrappedService() {
        return audioService;
    }

    void AudioServiceScriptProxy::setWrappedService(const std::weak_ptr<AudioService> & audioService) {
        AudioServiceScriptProxy::audioService = audioService;
    }

    // void AudioServiceScriptProxy::playMusic(int id) {
    //     if(auto audio = audioService.lock()) {
    //         audio->playMusic(id);
    //     }
    // }

    void AudioServiceScriptProxy::playMusic(const std::string & name) {
        if(auto audio = audioService.lock()) {
            audio->playMusic(name);
        }
    }

    void AudioServiceScriptProxy::stopMusic() {
        if(auto audio = audioService.lock()) {
            audio->stopMusic();
        }
    }

    // void AudioServiceScriptProxy::playSample(int id) {
    //     if(auto audio = audioService.lock()) {
    //         audio->playSample(id);
    //     }
    // }

    void AudioServiceScriptProxy::playSample(const std::string & name) {
        if(auto audio = audioService.lock()) {
            audio->playSample(name);
        }
    }

    void AudioServiceScriptProxy::stopAllSamples() {
        if(auto audio = audioService.lock()) {
            audio->stopAllSamples();
        }
    }

    bool AudioServiceScriptProxy::isMusicLoaded() {
        if(auto audio = audioService.lock()) {
            return audio->isMusicLoaded();
        }

        return false;
    }
    bool AudioServiceScriptProxy::isSamplesLoaded() {
        if(auto audio = audioService.lock()) {
            return audio->isSamplesLoaded();
        }

        return false;
    }

} // hikari