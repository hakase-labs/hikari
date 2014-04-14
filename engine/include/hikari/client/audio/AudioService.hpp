#ifndef HIKARI_CLIENT_AUDIOSERVICE
#define HIKARI_CLIENT_AUDIOSERVICE

#include "hikari/core/util/Service.hpp"
#include "hikari/client/audio/NSFSoundStream.hpp"

#include <memory>
#include <string>

namespace Json {
    class Value;
}

namespace hikari {

    class SoundLibrary;

    class AudioService : public Service {
    private:
        static const unsigned int MUSIC_BUFFER_SIZE;
        static const unsigned int SAMPLE_BUFFER_SIZE;
        static const unsigned int AUDIO_SAMPLE_RATE;
        static const float DEFAULT_VOLUME;
        bool musicLoaded;
        bool samplesLoaded;
        bool mutedFlag;
        float sampleVolume;
        float musicVolume;

        NSFSoundStream musicStream;
        NSFSoundStream sampleStream;

        std::unique_ptr<SoundLibrary> library;

        bool isValidConfiguration(const Json::Value &configuration) const;

    public:
        typedef int MusicId;
        typedef int SampleId;

        AudioService(const Json::Value &configuration);
        virtual ~AudioService();

        void playMusic(const std::string & name);
        void stopMusic();
        void setMusicVolume(float volume);
        float getMusicVolume() const;

        void playSample(const std::string & name);
        void setSampleVolume(float volume);
        float getSampleVolume() const;
        void stopAllSamples();

        bool isMusicLoaded() const;
        bool isSamplesLoaded() const;

        /**
         * Disables all audio and stops any playing music and samples. When
         * disabled, any requests to play music or samples are ignored.
         *
         * @see AudioService::unmute
         * @see AudioService::isMuted
         */
        void mute();

        /**
         * Enables music and sample playback.
         *
         * @see AudioService::mute
         * @see AudioService::isMuted
         */
        void unmute();

        /**
         * Checks to see if audio is enabled or not. When disabled, any requests
         * to play music or samples are ignored.
         *
         * @return true if currently enabled, false otherwise
         */
        bool isMuted() const;
    };

} // hikari

#endif // HIKARI_CLIENT_AUDIOSERVICE