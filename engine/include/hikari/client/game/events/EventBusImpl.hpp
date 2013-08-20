#ifndef HIKARI_CLIENT_EventBusIMPL
#define HIKARI_CLIENT_EventBusIMPL

#include "hikari/client/game/events/EventBus.hpp"
#include <list>
#include <map>

namespace hikari {

    /**
     * EventBus implementation.
     */
    class EventBusImpl : public EventBus {
    private:
        typedef std::list<EventListenerDelegate> EventListenerList;
        typedef std::map<EventType, EventListenerList> EventListenerMap;
        typedef std::list<EventDataPtr> EventQueue;

        EventListenerMap eventListeners;
        EventQueue eventQueues[2];

        int activeQueueIndex;
        
    public:
        explicit EventBusImpl(const std::string & name, bool setAsGlobal);
        virtual ~EventBusImpl();

        virtual bool addListener(const EventListenerDelegate & eventDelegate, const EventType & type);
        virtual bool removeListener(const EventListenerDelegate & eventDelegate, const EventType & type);
        virtual bool triggerEvent(const EventDataPtr & event) const;
        virtual bool queueEvent(const EventDataPtr & event);
        virtual bool cancelEvent(const EventType & type, bool allOfType = false);
        virtual bool processEvents(unsigned long maxMillis = INFINITE);
    };

} // hikari

#endif // HIKARI_CLIENT_EventBusIMPL