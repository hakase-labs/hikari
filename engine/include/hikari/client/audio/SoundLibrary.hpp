#ifndef HIKARI_CLIENT_SOUND_LIBRARY
#define HIKARI_CLIENT_SOUND_LIBRARY

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace sf {
    class SoundBuffer;
    class Sound;
}

namespace hikari {

    class GMESoundStream;

    class SoundLibrary {
    private:
        static const unsigned int MUSIC_BUFFER_SIZE;
        static const unsigned int SAMPLE_BUFFER_SIZE;
        static const unsigned int AUDIO_SAMPLE_RATE;

        struct MusicEntry {
            unsigned int track;
            unsigned int samplerId;
        };

        struct SampleEntry {
            unsigned int track;
            unsigned int priority;
            unsigned int samplerId;
        };

        struct SamplerPair {
            std::shared_ptr<GMESoundStream> musicStream;
            std::shared_ptr<GMESoundStream> sampleStream;
        };

        struct SamplePlayer {
            std::shared_ptr<sf::SoundBuffer> buffer;
            std::shared_ptr<sf::Sound> player;
            unsigned int priority;
        };

        bool isEnabledFlag;
        const std::string file;
        std::unordered_map<std::string, std::shared_ptr<MusicEntry>> music;
        std::unordered_map<std::string, std::shared_ptr<SampleEntry>> samples;
        std::unordered_map<std::string, std::shared_ptr<sf::SoundBuffer>> sampleSoundBuffers;
        std::vector<std::shared_ptr<GMESoundStream>> samplers;
        std::unordered_map<std::string, std::shared_ptr<SamplePlayer>> samplePlayers;
        std::shared_ptr<SampleEntry> currentlyPlayingSample;

        void loadLibrary();

    public:
        SoundLibrary(const std::string & file);

        bool isEnabled() const;

        /**
         * Tries to play a music by looking it up by name. If the music is found,
         * a valid GMESoundStream pointer is returned. Otherwise, a nullptr is
         * returned and no music is played.
         */
        std::shared_ptr<GMESoundStream> playMusic(const std::string & name, float volume = 100.0f);

        /**
         * Tries to play a sample by looking it up by name. If the sample is found,
         * a valid GMESoundStream pointer is returned. Otherwise, a nullptr is
         * returned and no sample is played.
         */
        std::shared_ptr<GMESoundStream> playSample(const std::string & name, float volume = 100.0f);

        void stopMusic();
        void stopSample();
    };

} // hikari

#endif // HIKARI_CLIENT_SOUND_LIBRARY
