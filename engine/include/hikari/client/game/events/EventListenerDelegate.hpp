#ifndef HIKARI_CLIENT_EVENTLISTENERDELEGATE
#define HIKARI_CLIENT_EVENTLISTENERDELEGATE

#include "hikari/client/game/events/EventData.hpp"
// #include <FastDelegate.h>
#include <functional>

namespace hikari {

    struct FunctionDelegateBase {
    private:
        static long long nextId;
        long long id;
        std::function<void(EventDataPtr)> fn;

    public:
        FunctionDelegateBase(const std::function<void(EventDataPtr)> & func);

        void operator()(EventDataPtr eventPtr);
        bool operator == (const FunctionDelegateBase &fdb) const;
        bool operator != (const FunctionDelegateBase &fdb) const;
    };

    typedef FunctionDelegateBase EventListenerDelegate;

} // hikari

#endif // HIKARI_CLIENT_EVENTLISTENERDELEGATE