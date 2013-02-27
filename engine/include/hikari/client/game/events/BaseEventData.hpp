#ifndef HIKARI_CLIENT_BASEEVENTDATA
#define HIKARI_CLIENT_BASEEVENTDATA

#include "hikari/client/game/events/EventData.hpp"

namespace hikari {

    class BaseEventData : public EventData {
    private:
        const EventTime timeStamp;

    public:
        explicit BaseEventData(const EventTime timeStamp = 0.0f);
        virtual ~BaseEventData();

        virtual const EventType & getEventType() const = 0;

        virtual EventTime getTimeStamp() const;
    };

} // hikari

#endif // HIKARI_CLIENT_BASEEVENTDATA