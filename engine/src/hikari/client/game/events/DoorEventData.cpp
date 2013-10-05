#include "hikari/client/game/events/DoorEventData.hpp"
#include "hikari/core/util/HashedString.hpp"

namespace hikari {

    const EventType DoorEventData::Type = HashedString("DoorEventData").getHash();

    DoorEventData::DoorEventData(const std::weak_ptr<Door> & door)
        : BaseEventData(0.0f)
        , door(door)
    {

    }

    DoorEventData::~DoorEventData() {
        // Do nothing!
    }

    const std::weak_ptr<Door> & DoorEventData::getDoor() const {
        return door;
    }

    const EventType & DoorEventData::getEventType() const {
        return DoorEventData::Type;
    }

    EventDataPtr DoorEventData::copy() const {
        return EventDataPtr(new DoorEventData(getDoor()));
    }

    const char * DoorEventData::getName() const {
        return "DoorEventData";
    }

} // hikari