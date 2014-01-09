#include "hikari/client/audio/SoundLibrary.hpp"
#include "hikari/client/audio/GMESoundStream.hpp"
#include "hikari/core/util/FileSystem.hpp"
#include "hikari/core/util/Log.hpp"

#include <algorithm>
#include <functional>
#include <string>

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <json/reader.h>
#include <json/value.h>

namespace hikari {

    const unsigned int SoundLibrary::MUSIC_BUFFER_SIZE = 2048 * 2;  // some platforms need larger buffer
    const unsigned int SoundLibrary::SAMPLE_BUFFER_SIZE = 2048 * 2; // so we'll double it for now.
    const unsigned int SoundLibrary::AUDIO_SAMPLE_RATE = 44100;

    SoundLibrary::SoundLibrary(const std::string & file)
        : isEnabledFlag(false)
        , file(file)
        , music()
        , samples()
        , sampleSoundBuffers()
        , samplers()
        , currentlyPlayingSample(nullptr) {
        loadLibrary();
    }

    void SoundLibrary::loadLibrary() {
        const std::string PROP_FILE     = "file";
        const std::string PROP_TITLE    = "title";
        const std::string PROP_MUSIC    = "music";
        const std::string PROP_SAMPLES  = "samples";
        const std::string PROP_TRACK    = "track";
        const std::string PROP_NAME     = "name";
        const std::string PROP_PRIORITY = "priority";
        Json::Reader reader;
        Json::Value root;
        auto fileContents = FileSystem::openFileRead(file);

        if(reader.parse(*fileContents, root, false)) {
            auto nsfCount = root.size();
            
            for(decltype(nsfCount) samplerIndex = 0; samplerIndex < nsfCount; ++samplerIndex) {
                const Json::Value & currentLibrary = root[samplerIndex];
                const std::string nsfFile = currentLibrary[PROP_FILE].asString();

                //
                // Create the sound stream/emulators
                //
                auto musicStream = std::make_shared<GMESoundStream>(MUSIC_BUFFER_SIZE);
                auto sampleStream = std::make_shared<GMESoundStream>(SAMPLE_BUFFER_SIZE);
                musicStream->open(nsfFile);
                sampleStream->open(nsfFile);

                samplers.push_back(musicStream);
                HIKARI_LOG(debug) << "Loaded sampler for " << nsfFile;

                //
                // Parse the music
                //
                const Json::Value & musicArray = currentLibrary[PROP_MUSIC];
                auto musicCount = musicArray.size();

                for(decltype(musicCount) musicIndex = 0; musicIndex < musicCount; ++musicIndex) {
                    const Json::Value & musicEntryJson = musicArray[musicIndex];
                    const std::string & name = musicEntryJson[PROP_NAME].asString();

                    auto musicEntry = std::make_shared<MusicEntry>();
                    musicEntry->track = musicEntryJson[PROP_TRACK].asUInt();
                    musicEntry->samplerId = samplerIndex;

                    music.insert(std::make_pair(name, musicEntry));
                    HIKARI_LOG(debug) << "\t-> loaded music \"" << name << "\"";
                }

                //
                // Parse the samples
                //
                const Json::Value & sampleArray = currentLibrary[PROP_SAMPLES];
                auto sampleCount = sampleArray.size();

                for(decltype(sampleCount) sampleIndex = 0; sampleIndex < sampleCount; ++sampleIndex) {
                    const Json::Value & sampleEntryJson = sampleArray[sampleIndex];
                    const std::string & name = sampleEntryJson[PROP_NAME].asString();

                    auto sampleEntry = std::make_shared<SampleEntry>();
                    sampleEntry->track = sampleEntryJson[PROP_TRACK].asUInt();
                    sampleEntry->priority = sampleEntryJson[PROP_PRIORITY].asUInt();
                    sampleEntry->samplerId = samplerIndex;

                    // Create a sound buffer and pre-render the sample into it.
                    auto sampleSoundBuffer = std::shared_ptr<sf::SoundBuffer>(
                            sampleStream->renderTrackToBuffer(sampleEntry->track));

                    // Index the buffers my the same key (the name of the sample)
                    sampleSoundBuffers.insert(std::make_pair(name, sampleSoundBuffer));

                    auto p = std::make_shared<SamplePlayer>();
                    p->buffer = sampleSoundBuffer;
                    p->player = std::make_shared<sf::Sound>(*sampleSoundBuffer.get());
                    p->priority = sampleEntryJson[PROP_PRIORITY].asUInt();

                    samplePlayers.insert(std::make_pair(name, p));

                    samples.insert(std::make_pair(name, sampleEntry));
                    HIKARI_LOG(debug) << "\t-> loaded sample \"" << name << "\"";

                }
            }
        }

        isEnabledFlag = true;
    }

    bool SoundLibrary::isEnabled() const {
        return isEnabledFlag;
    }

    std::shared_ptr<GMESoundStream> SoundLibrary::playMusic(const std::string & name) {
        const auto & iterator = music.find(name);

        if(iterator != std::end(music)) {
            HIKARI_LOG(debug4) << "Found the music '" << name << "'.";
            const std::shared_ptr<MusicEntry> & musicEntry = (*iterator).second;
            const auto & stream = samplers.at(musicEntry->samplerId);

            HIKARI_LOG(debug4) << "MusicEntry: " << musicEntry << ", samplerId = " << musicEntry->samplerId;

            stopMusic();
            stream->setCurrentTrack(musicEntry->track);
            stream->play();

            return stream;
        }

        HIKARI_LOG(debug4) << "Didn't find the music '" << name << "'.";

        return std::shared_ptr<GMESoundStream>(nullptr);
    }

    std::shared_ptr<GMESoundStream> SoundLibrary::playSample(const std::string & name) {
        const auto & iterator = samplePlayers.find(name);

        if(iterator != std::end(samplePlayers)) {
            const std::shared_ptr<SamplePlayer> & samplePlayer = (*iterator).second;
            const auto & player = samplePlayer->player;

            // Stop any sounds currently playing with lower priority
            std::for_each(std::begin(samplePlayers), std::end(samplePlayers), [&](decltype(samplePlayers)::value_type & pair) {
                const auto & otherPlayer = pair.second;

                if(otherPlayer->priority <= samplePlayer->priority) {
                    otherPlayer->player->stop();
                }
            });

            // Play (or restart) the sound we want to play
            player->play();
        } else {
            HIKARI_LOG(debug4) << "Didn't find the sample '" << name << "'.";
        }

        return std::shared_ptr<GMESoundStream>(nullptr);
    }

    void SoundLibrary::stopMusic() {
        std::for_each(std::begin(samplers), std::end(samplers), [](const std::shared_ptr<GMESoundStream> & musicSampler) {
            musicSampler->stop();
        });
    }

    void SoundLibrary::stopSample() {
        std::for_each(std::begin(samplePlayers), std::end(samplePlayers), [](decltype(samplePlayers)::value_type & pair) {
            const auto & otherPlayer = pair.second;
            const std::shared_ptr<sf::Sound> player = otherPlayer->player;
            
            player->stop();
        });
    }

} // hikari
