#ifndef HIKARI_CLIENT_EVENTBUSSERVICE
#define HIKARI_CLIENT_EVENTBUSSERVICE

#include "hikari/client/game/events/EventBus.hpp"
#include "hikari/core/util/Service.hpp"

namespace hikari {

    /**
     * Decorator class for an EventBus instance that exposes it as a service.
     */
    class EventBusService : public EventBus, public Service {
    private:
        std::shared_ptr<EventBus> eventBus;

    public:
        EventBusService(const std::string & name, bool setAsGlobal);
        explicit EventBusService(const std::shared_ptr<EventBus> eventBus);
        virtual ~EventBusService();

        virtual bool addListener(const EventListenerDelegate & eventDelegate, const EventType & type);
        virtual bool removeListener(const EventListenerDelegate & eventDelegate, const EventType & type);
        virtual bool triggerEvent(const EventDataPtr & event) const;
        virtual bool queueEvent(const EventDataPtr & event);
        virtual bool cancelEvent(const EventType & type, bool allOfType = false);
        virtual bool processEvents(unsigned long maxMillis = INFINITE);
    };

} // hikari

#endif // HIKARI_CLIENT_EVENTBUSSERVICE