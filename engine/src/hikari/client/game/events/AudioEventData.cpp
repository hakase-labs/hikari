#include "hikari/client/game/events/AudioEventData.hpp"
#include "hikari/core/util/HashedString.hpp"

namespace hikari {

    const EventType AudioEventData::Type = HashedString("AudioEventData").getHash();
    const char * AudioEventData::NO_NAME = "";

    AudioEventData::AudioEventData(AudioAction action, const std::string & name)
        : BaseEventData(0.0f)
        , action(action)
        , name(name)
    {

    }

    AudioEventData::~AudioEventData() {
        // Do nothing!
    }

    AudioEventData::AudioAction AudioEventData::getAudioAction() const {
        return action;
    }

    const std::string & AudioEventData::getMusicOrSampleName() const {
        return name;
    }

    const EventType & AudioEventData::getEventType() const {
        return AudioEventData::Type;
    }

    EventDataPtr AudioEventData::copy() const {
        return EventDataPtr(new AudioEventData(getAudioAction(), getName()));
    }

    const char * AudioEventData::getName() const {
        return "AudioEventData";
    }

} // hikari
