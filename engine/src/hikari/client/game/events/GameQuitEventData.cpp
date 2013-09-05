#include "hikari/client/game/events/GameQuitEventData.hpp"
#include "hikari/core/util/HashedString.hpp"

namespace hikari {

    const EventType GameQuitEventData::Type = HashedString("GameQuitEventData").getHash();

    GameQuitEventData::GameQuitEventData(QuitType quitType)
        : BaseEventData(0.0f)
        , quitType(quitType)
    {

    }

    GameQuitEventData::QuitType GameQuitEventData::getQuitType() const {
        return quitType;
    }

    GameQuitEventData::~GameQuitEventData() {
        // Do nothing!
    }

    const EventType & GameQuitEventData::getEventType() const {
        return GameQuitEventData::Type;
    }

    EventDataPtr GameQuitEventData::copy() const {
        return EventDataPtr(new GameQuitEventData(getQuitType()));
    }

    const char * GameQuitEventData::getName() const {
        return "GameQuitEventData";
    }

} // hikari