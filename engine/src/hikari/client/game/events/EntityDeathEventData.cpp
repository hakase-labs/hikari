#include "hikari/client/game/events/EntityDeathEventData.hpp"

namespace hikari {

    const EventType EntityDeathEventData::Type = 0xdeadb33f;

    EntityDeathEventData::EntityDeathEventData(int entityId)
        : BaseEventData(0.0f)
        , entityId(entityId)
    {

    }

    int EntityDeathEventData::getEntityId() const {
        return entityId;
    }

    EntityDeathEventData::~EntityDeathEventData() {
        // Do nothing!
    }

    const EventType & EntityDeathEventData::getEventType() const {
        return EntityDeathEventData::Type;
    }

    EventDataPtr EntityDeathEventData::copy() const {
        return EventDataPtr(new EntityDeathEventData(getEntityId()));
    }

    const char * EntityDeathEventData::getName() const {
        return "EntityDeathEventData";
    }

} // hikari