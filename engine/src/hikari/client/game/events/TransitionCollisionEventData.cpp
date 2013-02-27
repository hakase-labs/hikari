#include "hikari/client/game/events/TransitionCollisionEventData.hpp"

namespace hikari {

    const EventType TransitionCollisionEventData::Type = 0x94ae3da4;

    TransitionCollisionEventData::TransitionCollisionEventData()
        : BaseEventData(0.0f)
    {

    }

    TransitionCollisionEventData::~TransitionCollisionEventData() {
        // Do nothing!
    }

    const EventType & TransitionCollisionEventData::getEventType() const {
        return TransitionCollisionEventData::Type;
    }

    EventDataPtr TransitionCollisionEventData::copy() const {
        return EventDataPtr(new TransitionCollisionEventData());
    }

    const char * TransitionCollisionEventData::getName() const {
        return "TransitionCollisionEventData";
    }

} // hikari