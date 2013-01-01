#include <hikari/core/game/GameControllerException.hpp>

namespace hikari {

    GameControllerException::GameControllerException(const std::string& what) 
        : std::runtime_error(what) {
        
    }

    GameControllerException::~GameControllerException() { }

} // hikari