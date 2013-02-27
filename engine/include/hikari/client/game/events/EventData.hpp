#ifndef HIKARI_CLIENT_EVENTDATA
#define HIKARI_CLIENT_EVENTDATA

#include <memory>

namespace hikari {

    typedef unsigned long EventType;
    typedef float EventTime;

    //
    // Forward declaration to enable shared_ptr typedef
    //
    class EventData;

    typedef std::shared_ptr<EventData> EventDataPtr;

    /**
     * Interface for all event data objects.
     */
    class EventData {
    public:
        virtual ~EventData();
        virtual const EventType & getEventType() const = 0;
        virtual EventTime getTimeStamp() const = 0;
        virtual EventDataPtr copy() const = 0;
        virtual const char * getName() const = 0;
    };

} // hikari

#endif // HIKARI_CLIENT_EVENTDATA