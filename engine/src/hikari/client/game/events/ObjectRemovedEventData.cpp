#include "hikari/client/game/events/ObjectRemovedEventData.hpp"
#include "hikari/core/util/HashedString.hpp"
#include "hikari/core/util/Log.hpp"

namespace hikari {

    const EventType ObjectRemovedEventData::Type = HashedString("ObjectRemovedEventData").getHash();

    ObjectRemovedEventData::ObjectRemovedEventData(int objectId)
        : BaseEventData(0.0f)
        , objectId(objectId)
    {

    }

    int ObjectRemovedEventData::getObjectId() const {
        return objectId;
    }

    ObjectRemovedEventData::~ObjectRemovedEventData() {
        // Do nothing!
        HIKARI_LOG(debug1) << "~ObjectRemovedEventData()";
    }

    const EventType & ObjectRemovedEventData::getEventType() const {
        return ObjectRemovedEventData::Type;
    }

    EventDataPtr ObjectRemovedEventData::copy() const {
        return EventDataPtr(new ObjectRemovedEventData(getObjectId()));
    }

    const char * ObjectRemovedEventData::getName() const {
        return "ObjectRemovedEventData";
    }

} // hikari