#include "catch.hpp"

#include <hikari/client/game/events/EventBus.hpp>
#include <hikari/client/game/events/EventBusImpl.hpp>
#include <hikari/client/game/events/EventData.hpp>
#include <hikari/client/game/events/BaseEventData.hpp>

#include <FastDelegate.h>

#include <memory>

//
// Helper classes and functions
//
namespace helpers {
    unsigned int testAddListenerCounter = 0;

    void testAddListener(hikari::EventDataPtr data) {
        testAddListenerCounter++;
    }

    class MockEventDataA : public ::hikari::BaseEventData {
    public:
        static const hikari::EventType Type;

        virtual ~MockEventDataA() {

        }

        virtual const hikari::EventType & getEventType() const {
            return Type;
        }

        virtual hikari::EventDataPtr copy() const {
            return hikari::EventDataPtr(new MockEventDataA(*this));
        }

        virtual const char * getName() const {
            return "MockEventDataA";
        }
    };

    const hikari::EventType MockEventDataA::Type = 1;

    class MockEventDataB : public ::hikari::BaseEventData {
    public:
        static const hikari::EventType Type;

        virtual ~MockEventDataB() {

        }

        virtual const hikari::EventType & getEventType() const {
            return Type;
        }

        virtual hikari::EventDataPtr copy() const {
            return hikari::EventDataPtr(new MockEventDataB(*this));
        }

        virtual const char * getName() const {
            return "MockEventDataB";
        }
    };

    const hikari::EventType MockEventDataB::Type = 2;
}

//
// Tests for hikari::EventBusImpl
//
TEST_CASE( "EventBusImpl/addListener", "A listener is added and the manager returns true" ) {
    std::unique_ptr<hikari::EventBus> eventBus(new hikari::EventBusImpl("global", true));

    hikari::EventListenerDelegate delegatedFunction(&::helpers::testAddListener);

    REQUIRE( eventBus->addListener(delegatedFunction, ::helpers::MockEventDataA::Type) );
}

TEST_CASE( "EventBusImpl/addListener (multiple times)", "A listener is cannot be added to the manager multiple times" ) {
    std::unique_ptr<hikari::EventBus> eventBus(new hikari::EventBusImpl("global", true));

    hikari::EventListenerDelegate delegatedFunction(&::helpers::testAddListener);

    eventBus->addListener(delegatedFunction, ::helpers::MockEventDataA::Type);

    REQUIRE_FALSE( eventBus->addListener(delegatedFunction, ::helpers::MockEventDataA::Type) );
}

TEST_CASE( "EventBusImpl/removeListener", "A listener is removed and the manager returns true" ) {
    std::unique_ptr<hikari::EventBus> eventBus(new hikari::EventBusImpl("global", true));

    hikari::EventListenerDelegate delegatedFunction(&::helpers::testAddListener);

    eventBus->addListener(delegatedFunction, ::helpers::MockEventDataA::Type);

    REQUIRE( eventBus->removeListener(delegatedFunction, ::helpers::MockEventDataA::Type) );
}

TEST_CASE( "EventBusImpl/removeListener (mismatched type)", "A listener is not removed and the manager returns false" ) {
    std::unique_ptr<hikari::EventBus> eventBus(new hikari::EventBusImpl("global", true));

    hikari::EventListenerDelegate delegatedFunction(&::helpers::testAddListener);
    ::hikari::EventType correctType = ::helpers::MockEventDataA::Type;
    ::hikari::EventType incorrectType = ::helpers::MockEventDataB::Type;

    eventBus->addListener(delegatedFunction, correctType);

    REQUIRE_FALSE( eventBus->removeListener(delegatedFunction, incorrectType) );
}

TEST_CASE( "EventBusImpl/triggerEvent", "Any listeners for the event type will be called" ) {
    std::unique_ptr<hikari::EventBus> eventBus(new hikari::EventBusImpl("global", true));

    hikari::EventListenerDelegate delegatedFunction(&::helpers::testAddListener);

    eventBus->addListener(delegatedFunction, ::helpers::MockEventDataA::Type);

    ::helpers::testAddListenerCounter = 0;

    eventBus->triggerEvent(hikari::EventDataPtr(new ::helpers::MockEventDataA()));

    REQUIRE( ::helpers::testAddListenerCounter == 1 );
}

TEST_CASE( "EventBusImpl/queueEvent", "The event is enqueued and true is returned" ) {
    std::unique_ptr<hikari::EventBus> eventBus(new hikari::EventBusImpl("global", true));

    hikari::EventListenerDelegate delegatedFunction(&::helpers::testAddListener);

    eventBus->addListener(delegatedFunction, ::helpers::MockEventDataA::Type);

    REQUIRE( eventBus->queueEvent(hikari::EventDataPtr(new ::helpers::MockEventDataA())) );
}

TEST_CASE( "EventBusImpl/queueEvent (check listeners)", "The event is enqueued but any listeners for the event type will not be called" ) {
    std::unique_ptr<hikari::EventBus> eventBus(new hikari::EventBusImpl("global", true));

    hikari::EventListenerDelegate delegatedFunction(&::helpers::testAddListener);

    eventBus->addListener(delegatedFunction, ::helpers::MockEventDataA::Type);

    ::helpers::testAddListenerCounter = 0;

    eventBus->queueEvent(hikari::EventDataPtr(new ::helpers::MockEventDataA()));

    REQUIRE( ::helpers::testAddListenerCounter == 0 );
}

TEST_CASE( "EventBusImpl/cancelEvent (normal use case)", "The enqueued instance of the event is cancelled and true is returned" ) {
    std::unique_ptr<hikari::EventBus> eventBus(new hikari::EventBusImpl("global", true));

    hikari::EventListenerDelegate delegatedFunction(&::helpers::testAddListener);

    eventBus->addListener(delegatedFunction, ::helpers::MockEventDataA::Type);

    ::helpers::testAddListenerCounter = 0;

    hikari::EventDataPtr event(new ::helpers::MockEventDataA());

    eventBus->queueEvent(event);

    REQUIRE( eventBus->cancelEvent(::helpers::MockEventDataA::Type) );
}

TEST_CASE( "EventBusImpl/cancelEvent (cancel only one, leave others)", "The enqueued instance of the event is cancelled and true is returned" ) {
    std::unique_ptr<hikari::EventBus> eventBus(new hikari::EventBusImpl("global", true));

    hikari::EventListenerDelegate delegatedFunction(&::helpers::testAddListener);

    eventBus->addListener(delegatedFunction, ::helpers::MockEventDataA::Type);

    ::helpers::testAddListenerCounter = 0;

    hikari::EventDataPtr eventA(new ::helpers::MockEventDataA());
    hikari::EventDataPtr eventB(new ::helpers::MockEventDataA());

    eventBus->queueEvent(eventA);
    eventBus->queueEvent(eventB);

    eventBus->cancelEvent(::helpers::MockEventDataA::Type);
    eventBus->processEvents();

    REQUIRE( ::helpers::testAddListenerCounter == 1 );
}

TEST_CASE( "EventBusImpl/cancelEvent (cancel all of type)", "The enqueued instances of the event is cancelled and true is returned" ) {
    std::unique_ptr<hikari::EventBus> eventBus(new hikari::EventBusImpl("global", true));

    hikari::EventListenerDelegate delegatedFunction(&::helpers::testAddListener);

    eventBus->addListener(delegatedFunction, ::helpers::MockEventDataA::Type);

    ::helpers::testAddListenerCounter = 0;

    hikari::EventDataPtr eventA(new ::helpers::MockEventDataA());
    hikari::EventDataPtr eventB(new ::helpers::MockEventDataA());

    eventBus->queueEvent(eventA);
    eventBus->queueEvent(eventB);

    eventBus->cancelEvent(::helpers::MockEventDataA::Type, true);
    eventBus->processEvents();

    REQUIRE( ::helpers::testAddListenerCounter == 0 );
}

TEST_CASE( "EventBusImpl/cancelEvent (invalid cancellation)", "The event is not cancelled and false is returned" ) {
    std::unique_ptr<hikari::EventBus> eventBus(new hikari::EventBusImpl("global", true));

    REQUIRE_FALSE( eventBus->cancelEvent(::helpers::MockEventDataA::Type) );
}

TEST_CASE( "EventBusImpl/cancelEvent (doesn't trigger handlers)", "The enqueued instance of the event is cancelled and no handlers are executed" ) {
    std::unique_ptr<hikari::EventBus> eventBus(new hikari::EventBusImpl("global", true));

    hikari::EventListenerDelegate delegatedFunction(&::helpers::testAddListener);

    eventBus->addListener(delegatedFunction, ::helpers::MockEventDataA::Type);

    ::helpers::testAddListenerCounter = 0;

    hikari::EventDataPtr event(new ::helpers::MockEventDataA());

    eventBus->queueEvent(event);
    eventBus->cancelEvent(::helpers::MockEventDataA::Type);
    eventBus->processEvents();

    REQUIRE( ::helpers::testAddListenerCounter == 0 );
}

TEST_CASE( "EventBusImpl/processEvents (empty queue)", "Returns true if the queue is empty" ) {
    std::unique_ptr<hikari::EventBus> eventBus(new hikari::EventBusImpl("global", true));

    REQUIRE( eventBus->processEvents() );
}

// addListener(const hikari::EventListenerDelegate & eventDelegate, const EventType & type) = 0;