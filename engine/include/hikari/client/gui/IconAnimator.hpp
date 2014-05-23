#ifndef HIKARI_CORE_GAME_ICONANIMATOR
#define HIKARI_CORE_GAME_ICONANIMATOR

#include "hikari/core/Platform.hpp"
#include "hikari/core/game/Animator.hpp"

namespace hikari {
namespace gui {

    class Icon;

    class HIKARI_API IconAnimator : public Animator {
    private:
        Icon & icon;

    public:
        IconAnimator(Icon & icon); 
        virtual ~IconAnimator();
        virtual void update(float delta);
    };

} // gui
} // hikari

#endif // HIKARI_CORE_GAME_ICONANIMATOR