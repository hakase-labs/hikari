#ifndef HIKARI_CLIENT_AUDIOEVENTDATA
#define HIKARI_CLIENT_AUDIOEVENTDATA

#include "hikari/client/game/events/BaseEventData.hpp"

namespace hikari {

    typedef int SoundType;

    class AudioEventData : public BaseEventData {
    public:
        static const EventType Type;
        static const char * NO_NAME;

        enum AudioAction {
            ACTION_PLAY_SAMPLE = 0,
            ACTION_STOP_ALL_SAMPLES = 1,
            ACTION_PLAY_MUSIC = 2,
            ACTION_STOP_MUSIC = 3,
            ACTION_MUTE = 4,
            ACTION_UNMUTE = 5
        };

    private:
        AudioAction action;
        std::string name;
        
    public:
        explicit AudioEventData(AudioAction action, const std::string & name = NO_NAME);
        virtual ~AudioEventData();

        AudioAction getAudioAction() const;
        const std::string & getMusicOrSampleName() const;

        virtual const EventType & getEventType() const;

        virtual EventDataPtr copy() const;
        virtual const char * getName() const;
    };

} // hikari

#endif // HIKARI_CLIENT_AUDIOEVENTDATA