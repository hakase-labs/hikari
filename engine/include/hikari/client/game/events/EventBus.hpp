#ifndef HIKARI_CLIENT_EVENTBUS
#define HIKARI_CLIENT_EVENTBUS

#include "hikari/client/game/events/EventData.hpp"
#include "hikari/client/game/events/EventListenerDelegate.hpp"
#include <string>

namespace hikari {

    /**
     * Interface for all event busses.
     */
    class EventBus {
    public:
        enum Constants {
            INFINITE = 0xffffffff
        };

        explicit EventBus(const std::string & name, bool setAsGlobal);
        virtual ~EventBus();

        /**
         * Registers a delegate function that will get called when the event type
         * is triggered. Returns true if successful, false if not.
         * 
         * @param  eventDelegate the delegate function to call
         * @param  type          the event type that triggers the delegate call
         * @return               true if successfully registered, false otherwise
         */
        virtual bool addListener(const EventListenerDelegate & eventDelegate, const EventType & type) = 0;

        /**
         * Removes a delegate / event type pairing from the registry. Returns
         * false if the specific pairing is not found.
         * 
         * @param  eventDelegate the delegate function to remove
         * @param  type          the event type to remove pairing with
         * @return               true if pairing removed, false otherwise
         */
        virtual bool removeListener(const EventListenerDelegate & eventDelegate, const EventType & type) = 0;

        /**
         * Triggers all event delegates for a given event immediately.
         * 
         * @param  event the event to trigger
         * @return       true if the event was successfully triggered, false otherwise
         */
        virtual bool triggerEvent(const EventDataPtr & event) const = 0;

        /**
         * Queues an event to be fired. The next time events are processed by the
         * manager any associated delegates will be called.
         * 
         * @param  event the event to queue for processing
         * @return       true if the event was queued, false otherwise
         */
        virtual bool queueEvent(const EventDataPtr & event) = 0;

        virtual bool cancelEvent(const EventType & type, bool allOfType = false) = 0;

        /**
         * Processed any queued events, calling their delegates if any are 
         * registered. If event processing takes an excessively long time, some
         * events may be requeued so that they can be processed in another frame.
         * 
         * @param  maxMillis the maximum amount of time that can be spent processing events
         * @return           true if all events were processed, false if some events were requeued
         */
        virtual bool processEvents(unsigned long maxMillis = INFINITE) = 0;
    };

} // hikari

#endif // HIKARI_CLIENT_EVENTBUS