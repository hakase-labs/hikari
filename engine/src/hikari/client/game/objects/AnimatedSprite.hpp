#include "hikari/client/game/objects/AnimatedSprite.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace hikari {
    class AnimatedSprite {
    private:
        sf::Sprite sprite;
        std::string currentAnimation;
        std::weak_ptr<Animation> animation;
        std::weak_ptr<AnimationSet> animationSet;
        SpriteAnimator animator;

    public:
        AnimatedSprite::AnimatedSprite()
            : sprite()
            , currentAnimation("")
            , animation()
            , animationSet()
            , animator(sprite)
        {

        }

        void AnimatedSprite::update(float dt) {
            animator.update(dt);
        }

        void AnimatedSprite::render(sf::RenderTarget &target) {
            target.draw(sprite);
        }

        void AnimatedSprite::setAnimation(const std::string & animationName) {

        }

        void AnimatedSprite::setAnimationSet(const std::weak_ptr<AnimationSet> & animationSetPtr) {

        }

        const std::string & AnimatedSprite::getAnimation() const {
            return currentAnimation;
        }

        const std::weak_ptr<AnimationSet> AnimatedSprite::getAnimationSet() const {
            return animationSet;
        }

} // hikari
