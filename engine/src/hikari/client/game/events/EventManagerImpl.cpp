#include "hikari/client/game/events/EventManagerImpl.hpp"
#include "hikari/client/game/events/EventData.hpp"
#include "hikari/core/util/Log.hpp"

#include <iterator>

namespace hikari {

    EventManagerImpl::EventManagerImpl(const std::string & name, bool setAsGlobal)
        : EventManager(name, setAsGlobal)
    {
        activeQueueIndex = 0;
    }

    EventManagerImpl::~EventManagerImpl() {
        // Do nothing!
    }

    bool EventManagerImpl::addListener(const EventListenerDelegate & eventDelegate, const EventType & type) {
        // This will create a list of one doesn't exist.
        EventListenerList & eventListenerList = eventListeners[type];

        for(auto it = std::begin(eventListenerList); it != std::end(eventListenerList); ++it) {
            if(eventDelegate == (*it)) {
                HIKARI_LOG(error) << "Attempting to double-register a delegate.";
                return false;
            }
        }

        eventListenerList.push_back(eventDelegate);

        return true;
    }

    bool EventManagerImpl::removeListener(const EventListenerDelegate & eventDelegate, const EventType & type) {
        bool success = false;

        auto findIt = eventListeners.find(type);

        if(findIt != std::end(eventListeners)) {

            EventListenerList & listeners = findIt->second;

            for(auto it = std::begin(listeners); it != std::end(listeners); ++it) {
                if(eventDelegate == (*it)) {
                    listeners.erase(it);
                    success = true;
                    break;
                }
            }
        }

        return success;
    }
    
    bool EventManagerImpl::triggerEvent(const EventDataPtr & event) const {
        bool processed = false;

        auto findIt = eventListeners.find(event->getEventType());

        if(findIt != std::end(eventListeners)) {
            const EventListenerList & eventListenerList = findIt->second;

            for(auto it = eventListenerList.cbegin(); it != eventListenerList.cend(); ++it) {
                auto listener = (*it);

                listener(event);

                processed = true;
            }
        }

        return processed;
    }
    
    bool EventManagerImpl::queueEvent(const EventDataPtr & event) {
        // ASSERT activeQueueIndex >= 0
        // ASSERT activeQueueIndex < 2
        
        auto findIt = eventListeners.find(event->getEventType());

        if(findIt != std::end(eventListeners)) {
            eventQueues[activeQueueIndex].push_back(event);
            return true;
        }

        return false;
    }
    
    bool EventManagerImpl::cancelEvent(const EventType & type, bool allOfType) {
        // ASSERT activeQueueIndex >= 0
        // ASSERT activeQueueIndex < 2
        
        bool success = false;

        auto findIt = eventListeners.find(type);

        if(findIt != std::end(eventListeners)) {
            EventQueue & eventQueue = eventQueues[activeQueueIndex];

            auto it = std::begin(eventQueue);

            while(it != std::end(eventQueue)) {
                // Keep next iterator since erasing will invalidate the current iterator
                auto thisIt = it;
                ++it;

                if((*thisIt)->getEventType() == type) {
                    eventQueue.erase(thisIt);
                    success = true;

                    if(!allOfType) {
                        break;
                    }
                }
            }
        }

        return success;
    }
    
    bool EventManagerImpl::processEvents(unsigned long maxMillis) {
        int queueToProcessIndex = activeQueueIndex;

        // Swap active queue and clear the new queue
        activeQueueIndex = (activeQueueIndex + 1) % 2;
        eventQueues[activeQueueIndex].clear();

        // Process queued events
        while(!eventQueues[queueToProcessIndex].empty()) {
            EventDataPtr event = eventQueues[queueToProcessIndex].front();
            eventQueues[queueToProcessIndex].pop_front();

            const EventType & eventType = event->getEventType();

            // Find all delegate functions for this event
            auto findIt = eventListeners.find(eventType);

            if(findIt != std::end(eventListeners)) {
                const EventListenerList & listeners = findIt->second;

                // Call each listener
                for(auto it = std::begin(listeners); it != std::end(listeners); ++it) {
                    EventListenerDelegate listener = (*it);
                    listener(event);
                }
            }

            // TODO: Add elapsed processing time here so we can bail out if
            // event processing is taking too much time in one frame.
        }

        // If we couldn't process all events this frame, put them in the other
        // queue so they'll get processed next frame.
        bool queueFlushed = eventQueues[queueToProcessIndex].empty();

        if(!queueFlushed) {
            while(!eventQueues[queueToProcessIndex].empty()) {
                auto event = eventQueues[queueToProcessIndex].back();
                eventQueues[queueToProcessIndex].pop_back();
                eventQueues[activeQueueIndex].push_front(event);
            }
        }

        return queueFlushed;
    }
    

} // hikari