#include "hikari/client/game/events/EntityStateChangeEventData.hpp"
#include "hikari/core/util/HashedString.hpp"

namespace hikari {

    const EventType EntityStateChangeEventData::Type = HashedString("EntityStateChangeEventData").getHash(); // was 0xd11f5747;

    EntityStateChangeEventData::EntityStateChangeEventData(int entityId, const std::string & stateName)
        : BaseEventData(0.0f)
        , entityId(entityId)
        , stateName(stateName)
    {

    }

    int EntityStateChangeEventData::getEntityId() const {
        return entityId;
    }

    const std::string & EntityStateChangeEventData::getStateName() const {
        return stateName;
    }

    EntityStateChangeEventData::~EntityStateChangeEventData() {
        // Do nothing!
    }

    const EventType & EntityStateChangeEventData::getEventType() const {
        return EntityStateChangeEventData::Type;
    }

    EventDataPtr EntityStateChangeEventData::copy() const {
        return EventDataPtr(new EntityStateChangeEventData(getEntityId(), getStateName()));
    }

    const char * EntityStateChangeEventData::getName() const {
        return "EntityStateChangeEventData";
    }

} // hikari