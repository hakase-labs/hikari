#ifndef HIKARI_CLIENT_AUDIOSERVICE
#define HIKARI_CLIENT_AUDIOSERVICE

#include <hikari/core/util/Service.hpp>
#include <hikari/client/audio/NSFSoundStream.hpp>

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
    };

} // hikari

#endif // HIKARI_CLIENT_AUDIOSERVICE