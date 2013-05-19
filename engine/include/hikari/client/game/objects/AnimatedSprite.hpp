#ifndef HIKARI_CLIENT_GAME_OBJECTS_ANIMATEDSPRITE
#define HIKARI_CLIENT_GAME_OBJECTS_ANIMATEDSPRITE

#include "hikari/core/game/SpriteAnimator.hpp"
#include "hikari/core/game/Updatable.hpp"

#include <SFML/Graphics/Sprite.hpp>

#include <memory>

namespace hikari {
    class AnimatedSprite {
    private:
        sf::Sprite sprite;
        std::string currentAnimation;
        std::weak_ptr<Animation> animation;
        std::weak_ptr<AnimationSet> animationSet;
        SpriteAnimator animator;

    public:
        AnimatedSprite();

        //
        // Inherited from hikari::Updatable
        //
        virtual void update(float dt);

        void setAnimation(const std::string & animationName);
        void setAnimationSet(const std::weak_ptr<AnimationSet> & animationSetPtr);

        const sf::Sprite & getSprite() const;
        const std::string & getAnimation() const;
        const std::weak_ptr<AnimationSet> getAnimationSet() const;

        // set position
        // flip x
        // flip y ?
    };
} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_ANIMATEDSPRITE