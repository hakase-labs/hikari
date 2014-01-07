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

    class SoundLibrary {
    private:
        static const unsigned int MUSIC_BUFFER_SIZE;
        static const unsigned int SAMPLE_BUFFER_SIZE;
        static const unsigned int AUDIO_SAMPLE_RATE;

        bool isEnabledFlag;
        const std::string file;
        std::unordered_map<std::string, std::shared_ptr<MusicEntry>> music;
        std::unordered_map<std::string, std::shared_ptr<SampleEntry>> samples;
        std::unordered_map<std::string, std::shared_ptr<sf::SoundBuffer>> sampleSoundBuffers;
        std::vector<SamplerPair> samplers;
        std::shared_ptr<SampleEntry> currentlyPlayingSample;
        std::unique_ptr<sf::Sound> soundPlayer;

        void loadLibrary();

    public:
        SoundLibrary(const std::string & file);

        bool isEnabled() const;

        void addMusic(const std::string & name, std::shared_ptr<MusicEntry> entry);
        void addSample(const std::string & name, std::shared_ptr<SampleEntry> entry);

        /**
         * Tries to play a music by looking it up by name. If the music is found,
         * a valid GMESoundStream pointer is returned. Otherwise, a nullptr is
         * returned and no music is played.
         */
        std::shared_ptr<GMESoundStream> playMusic(const std::string & name);

        /**
         * Tries to play a sample by looking it up by name. If the sample is found,
         * a valid GMESoundStream pointer is returned. Otherwise, a nullptr is
         * returned and no sample is played.
         */
        std::shared_ptr<GMESoundStream> playSample(const std::string & name);

        void stopMusic();
        void stopSample();
    };

} // hikari

#endif // HIKARI_CLIENT_SOUND_LIBRARY
