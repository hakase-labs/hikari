#include <hikari/core/util/exception/ServiceNotRegisteredException.hpp>

namespace hikari {

    ServiceNotRegisteredException::ServiceNotRegisteredException(const std::string& message)
        : HikariException(message)
    {

    }

} // hikari