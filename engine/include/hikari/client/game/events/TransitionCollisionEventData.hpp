#ifndef HIKARI_CLIENT_TRANSITIONCOLLISIONEVENTDATA
#define HIKARI_CLIENT_TRANSITIONCOLLISIONEVENTDATA

#include "hikari/client/game/events/BaseEventData.hpp"

namespace hikari {

    class TransitionCollisionEventData : public BaseEventData {
    public:
        static const EventType Type;
        
    public:
        TransitionCollisionEventData();
        virtual ~TransitionCollisionEventData();

        virtual const EventType & getEventType() const;

        virtual EventDataPtr copy() const;
        virtual const char * getName() const;
    };

} // hikari

#endif // HIKARI_CLIENT_TRANSITIONCOLLISIONEVENTDATA