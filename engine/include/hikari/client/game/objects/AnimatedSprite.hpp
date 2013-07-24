#ifndef HIKARI_CLIENT_GAME_OBJECTS_ANIMATEDSPRITE
#define HIKARI_CLIENT_GAME_OBJECTS_ANIMATEDSPRITE

#include "hikari/core/game/SpriteAnimator.hpp"
#include "hikari/core/game/Updatable.hpp"
#include "hikari/core/math/Vector2.hpp"

#include <SFML/Graphics/Sprite.hpp>

#include <string>
#include <memory>

namespace sf {
    class RenderTarget;
}

namespace hikari {
    class Animation;
    class AnimationSet;

    class AnimatedSprite {
    private:
        sf::Sprite sprite;
        std::string currentAnimation;
        // std::weak_ptr<Animation> animation;
        std::weak_ptr<AnimationSet> animationSet;
        SpriteAnimator animator;
        bool isXAxisFlipped;
        bool isYAxisFlipped;

    public:
        AnimatedSprite();

        //
        // Inherited from hikari::Updatable
        //
        virtual void update(float dt);

        void render(sf::RenderTarget &target);

        void setAnimation(const std::string & animationName);
        void setAnimationSet(const std::weak_ptr<AnimationSet> & animationSetPtr);

        // const sf::Sprite & getSprite() const;
        const std::string & getAnimation() const;
        const std::weak_ptr<AnimationSet> getAnimationSet() const;

        bool isXFlipped() const;
        void setXFlipped(bool isFlipped);

        bool isYFlipped() const;
        void setYFlipped(bool isFlipped);

        Vector2<float> getPosition() const;
        void setPosition(const Vector2<float> & position);

        // set position
        // flip x
        // flip y ?
    };
} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_ANIMATEDSPRITE