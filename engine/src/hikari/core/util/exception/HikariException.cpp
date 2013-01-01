#include <hikari/core/util/exception/HikariException.hpp>

namespace hikari {

    HikariException::HikariException(const std::string& message) 
        : std::runtime_error(message) 
    {

    }

} // hikari