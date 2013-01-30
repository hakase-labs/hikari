#ifndef HIKARI_CORE_UTIL_SERVICE_HPP
#define HIKARI_CORE_UTIL_SERVICE_HPP

#include "hikari/core/Platform.hpp"

namespace hikari {

    /**
        Interface for all services.
    */
    class HIKARI_API Service {
    public:
        virtual ~Service() { }
    };

} // hikari

#endif // HIKARI_CORE_UTIL_SERVICE_HPP