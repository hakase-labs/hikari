#ifndef HIKARI_CORE_UTIL_HIKARIEXCEPTION
#define HIKARI_CORE_UTIL_HIKARIEXCEPTION

#include <hikari/core/Platform.hpp>
#include <exception>
#include <string>

namespace hikari {

    /**
        Base class for all Hikari-specific exceptions.
    */
    class HIKARI_API HikariException : public std::runtime_error {
    public:
        explicit HikariException(const std::string& message);
        virtual ~HikariException() { }
    };

} // hikari

#endif // HIKARI_CORE_UTIL_HIKARIEXCEPTION