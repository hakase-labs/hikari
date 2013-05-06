#ifndef HIKARI_CLIENT_ENTITYDEATHEVENTDATA
#define HIKARI_CLIENT_ENTITYDEATHEVENTDATA

#include "hikari/client/game/events/BaseEventData.hpp"

namespace hikari {

    class EntityDeathEventData : public BaseEventData {
    public:
        static const EventType Type;

    private:
        int entityId;
        
    public:
        explicit EntityDeathEventData(int entityId);
        virtual ~EntityDeathEventData();

        int getEntityId() const;

        virtual const EventType & getEventType() const;

        virtual EventDataPtr copy() const;
        virtual const char * getName() const;
    };

} // hikari

#endif // HIKARI_CLIENT_ENTITYDEATHEVENTDATA