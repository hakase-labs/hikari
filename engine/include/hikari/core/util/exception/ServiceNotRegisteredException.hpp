#ifndef HIKARI_CORE_UTIL_EXCEPTION_SERVICENOTREGISTEREDEXCEPTION
#define HIKARI_CORE_UTIL_EXCEPTION_SERVICENOTREGISTEREDEXCEPTION

#include "hikari/core/util/exception/HikariException.hpp"
#include <string>

namespace hikari {

    class HIKARI_API ServiceNotRegisteredException : public HikariException {
    public:
        explicit ServiceNotRegisteredException(const std::string& messages);
        virtual ~ServiceNotRegisteredException() throw() { }
    };

} // hikari

#endif // HIKARI_CORE_UTIL_EXCEPTION_SERVICENOTREGISTEREDEXCEPTION