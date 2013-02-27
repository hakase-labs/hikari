#ifndef HIKARI_CLIENT_EVENTLISTENERDELEGATE
#define HIKARI_CLIENT_EVENTLISTENERDELEGATE

#include "hikari/client/game/events/EventData.hpp"
#include <FastDelegate.h>

namespace hikari {

    typedef fastdelegate::FastDelegate1<EventDataPtr> EventListenerDelegate;

} // hikari

#endif // HIKARI_CLIENT_EVENTLISTENERDELEGATE