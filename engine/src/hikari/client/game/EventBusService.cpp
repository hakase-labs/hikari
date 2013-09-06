#include "hikari/client/game/EventBusService.hpp"
#include "hikari/core/util/exception/HikariException.hpp"

namespace hikari {

    EventBusService::EventBusService(const std::string & name, bool setAsGlobal)
        : EventBus(name, setAsGlobal)
        , Service()
    {
        throw HikariException("EventBusService must wrap an EventBust instance; don't use this constructor.");
    }

    EventBusService::EventBusService(const std::shared_ptr<EventBus> eventBus)
        : EventBus("WrapperService", false)
        , Service()
        , eventBus(eventBus)
    {

    }

    EventBusService::~EventBusService() {

    }

    bool EventBusService::addListener(const EventListenerDelegate & eventDelegate, const EventType & type) {
        if(eventBus) {
            return eventBus->addListener(eventDelegate, type);
        } else {
            return false;
        }
    }

    bool EventBusService::removeListener(const EventListenerDelegate & eventDelegate, const EventType & type) {
        if(eventBus) {
            return eventBus->removeListener(eventDelegate, type);
        } else {
            return false;
        }
    }

    bool EventBusService::triggerEvent(const EventDataPtr & event) const {
        if(eventBus) {
            return eventBus->triggerEvent(event);
        } else {
            return false;
        }
    }

    bool EventBusService::queueEvent(const EventDataPtr & event) {
        if(eventBus) {
            return eventBus->queueEvent(event);
        } else {
            return false;
        }
    }

    bool EventBusService::cancelEvent(const EventType & type, bool allOfType) {
        if(eventBus) {
            return eventBus->cancelEvent(type, allOfType);
        } else {
            return false;
        }
    }

    bool EventBusService::processEvents(unsigned long maxMillis) {
        if(eventBus) {
            return eventBus->processEvents(maxMillis);
        } else {
            return false;
        }
    }

} // hikari
