#ifndef HIKARI_CORE_GAME_GAMECONTROLLEREXCEPTION
#define HIKARI_CORE_GAME_GAMECONTROLLEREXCEPTION

#include <hikari/core/Platform.hpp>
#include <string>
#include <stdexcept>

#pragma warning(push)
#pragma warning(disable:4275)

namespace hikari {

    class HIKARI_API GameControllerException : public std::runtime_error {
    public:
        explicit GameControllerException(const std::string& what);
        virtual ~GameControllerException() throw() { };
    };

} // hikari

#pragma warning(pop)

#endif // HIKARI_CORE_GAME_GAMECONTROLLEREXCEPTION