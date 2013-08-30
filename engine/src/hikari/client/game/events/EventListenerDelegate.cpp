#include "hikari/client/game/events/EventListenerDelegate.hpp"

namespace hikari {

    // Static variable initialization
    long long FunctionDelegateBase::nextId = 0;

    FunctionDelegateBase::FunctionDelegateBase(const std::function<void(EventDataPtr)> & func) 
        : id(nextId++)
        , fn(func)
    {

    }

    void FunctionDelegateBase::operator()(EventDataPtr eventPtr) {
        if(fn) {
            fn(eventPtr);
        }
    }

    bool FunctionDelegateBase::operator == (const FunctionDelegateBase &fdb) const {
        return id == fdb.id;
    }

    bool FunctionDelegateBase::operator != (const FunctionDelegateBase &fdb) const {
        return !(*this==fdb);
    }

} // hikari