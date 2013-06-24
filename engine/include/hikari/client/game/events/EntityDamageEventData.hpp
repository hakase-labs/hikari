#ifndef HIKARI_CLIENT_ENTITYDAMAGEEVENTDATA
#define HIKARI_CLIENT_ENTITYDAMAGEEVENTDATA

#include "hikari/client/game/events/BaseEventData.hpp"

namespace hikari {

    class EntityDamageEventData : public BaseEventData {
    public:
        static const EventType Type;

    private:
        int entityId;
        float amount;
        
    public:
        EntityDamageEventData(int entityId, float amount);
        virtual ~EntityDamageEventData();

        int getEntityId() const;
        float getAmount() const;

        virtual const EventType & getEventType() const;

        virtual EventDataPtr copy() const;
        virtual const char * getName() const;
    };

} // hikari

#endif // HIKARI_CLIENT_ENTITYDAMAGEEVENTDATA