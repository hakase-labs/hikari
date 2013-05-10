#ifndef HIKARI_CLIENT_ENTITYSTATECHANGEEVENTDATA
#define HIKARI_CLIENT_ENTITYSTATECHANGEEVENTDATA

#include "hikari/client/game/events/BaseEventData.hpp"
#include <string>

namespace hikari {

    class EntityStateChangeEventData : public BaseEventData {
    public:
        static const EventType Type;

    private:
        int entityId;
        std::string stateName;
        
    public:
        explicit EntityStateChangeEventData(int entityId, const std::string & stateName);
        virtual ~EntityStateChangeEventData();

        int getEntityId() const;
        const std::string & getStateName() const;

        virtual const EventType & getEventType() const;

        virtual EventDataPtr copy() const;
        virtual const char * getName() const;
    };

} // hikari

#endif // HIKARI_CLIENT_ENTITYSTATECHANGEEVENTDATA