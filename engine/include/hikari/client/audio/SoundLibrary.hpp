#ifndef HIKARI_CLIENT_SOUND_LIBRARY
#define HIKARI_CLIENT_SOUND_LIBRARY

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace hikari {

    class NSFSoundStream;

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
        std::shared_ptr<NSFSoundStream> musicStream;
        std::shared_ptr<NSFSoundStream> sampleStream;
    };

    class SoundLibrary {
    private:
        bool isEnabledFlag;
        const std::string file;
        std::unordered_map<std::string, std::shared_ptr<MusicEntry>> music;
        std::unordered_map<std::string, std::shared_ptr<SampleEntry>> samples;
        std::vector<SamplerPair> samplers;

        void loadLibrary();

    public:
        SoundLibrary(const std::string & file);

        bool isEnabled() const;

        void addMusic(const std::string & name, MusicEntry entry);
        void addSample(const std::string & name, SampleEntry entry);

        void playMusic(const std::string & name);
        void playSample(const std::string & name);

        void stopMusic();
        void stopSample();
    };

} // hikari

#endif // HIKARI_CLIENT_SOUND_LIBRARY
