#ifndef HIKARI_CORE_GAME_ANIMATIONPLAYBACKEXCEPTION
#define HIKARI_CORE_GAME_ANIMATIONPLAYBACKEXCEPTION

#include <hikari/core/Platform.hpp>
#include <string>
#include <stdexcept>

#pragma warning(push)
#pragma warning(disable:4275)

namespace hikari {

    class HIKARI_API AnimationPlaybackException : public std::runtime_error {
    public:
        explicit AnimationPlaybackException(const std::string& what);
        virtual ~AnimationPlaybackException() throw();
    };

} // hikari

#pragma warning(pop)

#endif // HIKARI_CORE_GAME_ANIMATIONPLAYBACKEXCEPTION