#ifndef HIKARI_CORE_GAME_SPRITEANIMATOR
#define HIKARI_CORE_GAME_SPRITEANIMATOR

#include "hikari/core/Platform.hpp"
#include "hikari/core/game/Animator.hpp"
#include <SFML/Graphics/Rect.hpp>

namespace sf {
    class Sprite;
}

namespace hikari {

    class HIKARI_API SpriteAnimator : public Animator {
    private:
        bool invertXOffset;
        bool invertYOffset;
        sf::Sprite &sprite;
        sf::IntRect sourceRectangle;

    public:
        SpriteAnimator(sf::Sprite &sprite); 
        virtual ~SpriteAnimator();
        void setInvertXOffset(const bool flip);
        void setInvertYOffset(const bool flip);
        virtual void update(float delta);
    };

} // hikari

#endif // HIKARI_CORE_GAME_SPRITEANIMATOR