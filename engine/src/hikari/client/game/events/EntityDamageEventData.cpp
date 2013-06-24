#include "hikari/client/game/events/EntityDamageEventData.hpp"
#include "hikari/core/util/HashedString.hpp"

namespace hikari {

    const EventType EntityDamageEventData::Type = HashedString("EntityDamageEventData").getHash();

    EntityDamageEventData::EntityDamageEventData(int entityId, float amount)
        : BaseEventData(0.0f)
        , entityId(entityId)
        , amount(amount)
    {

    }

    int EntityDamageEventData::getEntityId() const {
        return entityId;
    }

    float EntityDamageEventData::getAmount() const {
        return amount;
    }

    EntityDamageEventData::~EntityDamageEventData() {
        // Do nothing!
    }

    const EventType & EntityDamageEventData::getEventType() const {
        return EntityDamageEventData::Type;
    }

    EventDataPtr EntityDamageEventData::copy() const {
        return EventDataPtr(new EntityDamageEventData(getEntityId(), getAmount()));
    }

    const char * EntityDamageEventData::getName() const {
        return "EntityDamageEventData";
    }

} // hikari