#ifndef HIKARI_CLIENT_ENTITYDEATHEVENTDATA
#define HIKARI_CLIENT_ENTITYDEATHEVENTDATA

#include "hikari/client/game/events/BaseEventData.hpp"

namespace hikari {

    class EntityDeathEventData : public BaseEventData {
    public:
        static const EventType Type;

        enum EntityType {
            Unknown = 1,
            Hero = 2,
            Enemy = 3,
            Projectile = 4,
            Item = 5
        };

    private:
        int entityId;
        EntityType entityType;
        
    public:
        explicit EntityDeathEventData(int entityId, EntityType entityType = EntityType::Unknown);
        virtual ~EntityDeathEventData();

        int getEntityId() const;
        EntityType getEntityType() const;

        virtual const EventType & getEventType() const;

        virtual EventDataPtr copy() const;
        virtual const char * getName() const;
    };

} // hikari

#endif // HIKARI_CLIENT_ENTITYDEATHEVENTDATA