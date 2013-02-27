#include "hikari/client/game/events/BaseEventData.hpp"

namespace hikari {

    BaseEventData::BaseEventData(const EventTime timeStamp)
        : EventData()
        , timeStamp(timeStamp)
    {

    }

    BaseEventData::~BaseEventData() {
        // Do nothing!
    }

    EventTime BaseEventData::getTimeStamp() const {
        return timeStamp;
    }

} // hikari