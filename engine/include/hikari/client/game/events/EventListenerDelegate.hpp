#ifndef HIKARI_CLIENT_EVENTLISTENERDELEGATE
#define HIKARI_CLIENT_EVENTLISTENERDELEGATE

#include "hikari/client/game/events/EventData.hpp"
#include <functional>

namespace hikari {

    /**
     * A wrapper class for std::function objects so that they can be used as
     * delegates in the EventBus. FunctionDelegateBase are comparable but are
     * only equivalent if they are the same object or a copy of the original.
     *
     * Two delegates that were constructed independently will never be equal.
     */
    struct FunctionDelegateBase {
    private:
        static long long nextId;
        long long id;
        std::function<void(EventDataPtr)> fn;

    public:
        /**
         * Constructor which wraps a std::function object to be called as a
         * delegate.
         */
        FunctionDelegateBase(const std::function<void(EventDataPtr)> & func);

        /**
         * Calls the delegated function (if it is valid) with a given event
         * object.
         * 
         * @param eventPtr EventDataPtr object containing event details
         */
        void operator()(EventDataPtr eventPtr);

        /**
         * Tests if two FunctionDelegateBase objects are the same. They are
         * only equal if they are the same object or were copy-constructed.
         * This does not compare the std::function objects (since they are
         * not comparable).
         */
        bool operator == (const FunctionDelegateBase &fdb) const;

        /**
         * Tests the inverse equality of two FunctionDelegateBase objects.
         */
        bool operator != (const FunctionDelegateBase &fdb) const;
    };

    typedef FunctionDelegateBase EventListenerDelegate;

} // hikari

#endif // HIKARI_CLIENT_EVENTLISTENERDELEGATE