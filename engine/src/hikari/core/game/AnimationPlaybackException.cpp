#include <hikari/core/game/AnimationPlaybackException.hpp>

namespace hikari {

    AnimationPlaybackException::AnimationPlaybackException(const std::string& what) 
        : std::runtime_error(what) {
        
    }

} // hikari