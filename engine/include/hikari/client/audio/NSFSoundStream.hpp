#ifndef HIKARI_CLIENT_AUDIO_NSFSOUNDSTREAM_HPP
#define HIKARI_CLIENT_AUDIO_NSFSOUNDSTREAM_HPP

#include <memory>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/System/Time.hpp>
#include <cstdlib>
#include <stack>
#include <string>
#include <vector>

struct Music_Emu;
struct track_info_t;

namespace hikari {

    class NSFSoundStream : public sf::SoundStream {
    public:
        ////////////////////////////////////////////////////////////
        /// \brief Public constructor
        ///
        /// This constructor requires that a buffer size be specified. The
        /// buffer must be a multiple of two. Smaller buffers may cause odd
        /// playback and stuttering. A value of 2048 or 4096 is recommended.
        ///
        /// \param bufferSize The audio buffer size
        /// \param samplerCount The number of smaplers to create
        ///
        ////////////////////////////////////////////////////////////
        NSFSoundStream(std::size_t bufferSize, std::size_t samplerCount = 1);

        ////////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        ////////////////////////////////////////////////////////////
        virtual ~NSFSoundStream();

        ////////////////////////////////////////////////////////////
        /// \brief Open and load an NSF file for playback 
        ///
        /// \return True if file was opened successfully, false otherwise
        ///
        ////////////////////////////////////////////////////////////
        bool open(const std::string& Filename);

        ////////////////////////////////////////////////////////////
        /// \brief Change the current playing position in the stream source
        ///
        /// This function must be overriden by derived classes to
        /// allow random seeking into the stream source.
        ///
        /// \param timeOffset New playing position, relative to the beginning of the stream
        ///
        ////////////////////////////////////////////////////////////
        virtual void onSeek(sf::Time timeOffset);

        ////////////////////////////////////////////////////////////
        // NSF(e)-related methods
        ////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////
        /// \brief Gets the sample rate of the NES APU
        ///
        /// \return NES APU sample rate
        ///
        ////////////////////////////////////////////////////////////
        long getSampleRate() const;

        ////////////////////////////////////////////////////////////
        /// \brief Gets which track of the NSF is currently playing
        ///
        /// \return Currently-playing NSF track
        ///
        ////////////////////////////////////////////////////////////
        int getCurrentTrack() const;

        ////////////////////////////////////////////////////////////
        /// \brief Sets which track of the NSF is currently playing and resets
        /// playback to beginning of the track
        ///
        ////////////////////////////////////////////////////////////
        void setCurrentTrack(int track);

        ////////////////////////////////////////////////////////////
        /// \brief Determines how many tracks are in the currently-loaded NSF file
        ///
        /// \return Number of tracks in current NSF
        ///
        ////////////////////////////////////////////////////////////
        int getTrackCount() const;

        ////////////////////////////////////////////////////////////
        /// \brief Gets the track title of the currently-playing track if it has one
        ///
        /// If the file is an NSF then the track titles will be blank. For NSFE
        /// files, each track may have a name and if it does it will be returned.
        ///
        /// \return Name of currently-playing track if it has one, otherwise ""
        ///
        ////////////////////////////////////////////////////////////
        const std::string getTrackName();

        ////////////////////////////////////////////////////////////
        /// \brief Determines how many voices are used by the currently-loaded NSF
        ///
        /// \return Number of voices used
        ///
        ////////////////////////////////////////////////////////////
        int getVoiceCount() const;

        ////////////////////////////////////////////////////////////
        /// \brief Gets the names of the voices used by the currently-loaded NSF
        ///
        /// \return Strings containing each voice name
        ///
        ////////////////////////////////////////////////////////////
        std::vector<std::string> getVoiceNames() const;

    private:
        ////////////////////////////////////////////////////////////
        /// \brief Request a new chunk of audio samples from the stream source
        ///
        /// This function will fill the chunk with emulated sound
        /// from the NSF data.
        ///
        /// \param data Chunk of data to fill
        ///
        /// \return True to continue playback, false to stop
        ///
        ////////////////////////////////////////////////////////////
        virtual bool onGetData(sf::SoundStream::Chunk& Data);

        ////////////////////////////////////////////////////////////
        /// \brief Handles an error as returned by Blargg's Game_Music_Emu
        /// by turning it in to an exception
        ///
        /// If passed a valid pointer to a c-style string, an exception will be
        /// thrown using that string's contents
        ///
        /// \param str C-style string containing an error message
        ///
        ////////////////////////////////////////////////////////////
        void handleError(const char* str) const;

        /**
         * Creates and initializes buffers for all samplers.
         */
        void createSampleBuffers();

        ////////////////////////////////////////////////////////////
        // Member data
        ////////////////////////////////////////////////////////////
        std::size_t masterBufferSize;                  ///< Size of audio buffer
        std::size_t samplerCount;
        std::size_t activeSampler;

        std::unique_ptr<short[]> masterBuffer;       ///< Audio buffer to read/write to
        std::vector<std::unique_ptr<short[]>> sampleBuffers;
        std::vector<std::unique_ptr<Music_Emu>> sampleEmus;          ///< Pointer to NES APU emulator
        std::unique_ptr<track_info_t> trackInfo; ///< Pointer to current track information
        sf::Mutex mutex;                           ///< A mutex for keeping this thread-safe

        static const long SAMPLE_RATE = 44100;     ///< The sample rate of the NES APU
        static const std::size_t MUSIC_SAMPLER_INDEX = 0;
    };
} // hikari

#endif // HIKARI_CLIENT_AUDIO_NSFSOUNDSTREAM_HPP