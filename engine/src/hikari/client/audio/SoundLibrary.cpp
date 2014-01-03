#include "hikari/client/audio/SoundLibrary.hpp"
#include "hikari/client/audio/NSFSoundStream.hpp"

#include <algorithm>
#include <functional>

#include <json/reader.h>
#include <json/value.h>

namespace hikari {

    SoundLibrary::SoundLibrary(const std::string & file)
        : isEnabledFlag(false)
        , file(file)
        , music()
        , samples()
        , samplers()
        , currentlyPlayingSample(nullptr) {

    }

    void SoundLibrary::loadLibrary() {
        isEnabledFlag = true;
    }

    bool SoundLibrary::isEnabled() const {
        return isEnabledFlag;
    }

    void SoundLibrary::addMusic(const std::string & name, std::shared_ptr<MusicEntry> entry) {
        music.insert(std::make_pair(name, entry));
    }

    void SoundLibrary::addSample(const std::string & name, std::shared_ptr<SampleEntry> entry) {
        samples.insert(std::make_pair(name, entry));
    }

    std::shared_ptr<NSFSoundStream> SoundLibrary::playMusic(const std::string & name) {
        const auto & iterator = music.find(name);

        if(iterator != std::end(music)) {
            const std::shared_ptr<MusicEntry> & musicEntry = (*iterator).second;
            const SamplerPair & samplerPair = samplers.at(musicEntry->samplerId);
            const auto & stream = samplerPair.musicStream;

            stopMusic();
            stream->setCurrentTrack(musicEntry->track);
            stream->play();

            return stream;
        }

        return std::shared_ptr<NSFSoundStream>(nullptr);
    }

    std::shared_ptr<NSFSoundStream> SoundLibrary::playSample(const std::string & name) {
        const auto & iterator = samples.find(name);

        if(iterator != std::end(samples)) {
            const std::shared_ptr<SampleEntry> & sampleEntry = (*iterator).second;
            const SamplerPair & samplerPair = samplers.at(sampleEntry->samplerId);
            const auto & stream = samplerPair.sampleStream;

            if(currentlyPlayingSample) {
                if(sampleEntry->priority < currentlyPlayingSample->priority) {
                    // We're trying to play a sample with lower priority so just bail out.
                    return std::shared_ptr<NSFSoundStream>(nullptr);
                }
            }

            stopSample();
            stream->setCurrentTrack(sampleEntry->track);
            stream->play();

            return stream;
        }

        return std::shared_ptr<NSFSoundStream>(nullptr);
    }

    void SoundLibrary::stopMusic() {
        std::for_each(std::begin(samplers), std::end(samplers), [](SamplerPair & sampler) {
            if(const auto & musicSampler = sampler.musicStream) {
                musicSampler->stop();
            }
        });
    }

    void SoundLibrary::stopSample() {
        std::for_each(std::begin(samplers), std::end(samplers), [](SamplerPair & sampler) {
            if(const auto & sampleSampler = sampler.sampleStream) {
                sampleSampler->stop();
            }
        });
    }

} // hikari
