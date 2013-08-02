#ifndef HIKARI_CLIENT_AUDIOSERVICE
#define HIKARI_CLIENT_AUDIOSERVICE

#include "hikari/core/util/Service.hpp"
#include "hikari/client/audio/NSFSoundStream.hpp"

namespace Json {
    class Value;
}

namespace hikari {

    class AudioService : public Service {
    private:
        static const unsigned int MUSIC_BUFFER_SIZE;
        static const unsigned int SAMPLE_BUFFER_SIZE;
        static const unsigned int AUDIO_SAMPLE_RATE;
        bool musicLoaded;
        bool samplesLoaded;
        bool enabledFlag;

        NSFSoundStream musicStream;
        NSFSoundStream sampleStream;

        bool isValidConfiguration(const Json::Value &configuration) const;

    public:
        typedef int MusicId;
        typedef int SampleId;

        AudioService(const Json::Value &configuration);
        virtual ~AudioService();
        
        void playMusic(MusicId id);
        void stopMusic();
        void playSample(SampleId id);
        void stopAllSamples();

        bool isMusicLoaded() const;
        bool isSamplesLoaded() const;

        /**
         * Disables all audio and stops any playing music and samples. When
         * disabled, any requests to play music or samples are ignored.
         *
         * @see AudioService::enable
         * @see AudioService::isEnabled
         */
        void disable();

        /**
         * Enables music and sample playback.
         *
         * @see AudioService::disable
         * @see AudioService::isEnabled
         */
        void enable();

        /**
         * Checks to see if audio is enabled or not. When disabled, any requests
         * to play music or samples are ignored.
         * 
         * @return true if currently enabled, false otherwise
         */
        bool isEnabled() const;
    };

} // hikari

#endif // HIKARI_CLIENT_AUDIOSERVICE