#include "hikari/client/game/objects/AnimatedSprite.hpp"
#include "hikari/core/game/Animation.hpp"
#include "hikari/core/game/AnimationSet.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>

namespace hikari {

    AnimatedSprite::AnimatedSprite()
        : sprite()
        , currentAnimation("")
        // , animation()
        , animationSet()
        , animator(sprite)
        , isXAxisFlipped(false)
        , isYAxisFlipped(false)
    {

    }

    void AnimatedSprite::update(float dt) {
        animator.update(dt);
    }

    void AnimatedSprite::render(sf::RenderTarget &target) {
        target.draw(sprite);
    }

    void AnimatedSprite::setAnimation(const std::string & animationName) {
        if(auto animSet = animationSet.lock()) {
            if(animSet->has(animationName)) {
                animator.setAnimation(animSet->get(animationName));
            }
        }
    }

    void AnimatedSprite::setAnimationSet(const std::weak_ptr<AnimationSet> & animationSetPtr) {
        std::cout << "BEFORE THE ANIMATION SET POINTER" << std::endl;
        if(!animationSetPtr.expired()) {
            animationSet = animationSetPtr;
        }
        std::cout << "SET THE ANIMATION SET POINTER" << std::endl;
    }

    const std::string & AnimatedSprite::getAnimation() const {
        return currentAnimation;
    }

    const std::weak_ptr<AnimationSet> AnimatedSprite::getAnimationSet() const {
        return animationSet;
    }

    bool AnimatedSprite::isXFlipped() const {
        return isXAxisFlipped;
    }

    void AnimatedSprite::setXFlipped(bool isFlipped) {
        isXAxisFlipped = isFlipped;

        if(isXAxisFlipped) {
            sprite.setScale(
                -1.0f * std::abs(sprite.getScale().x),
                sprite.getScale().y
            );
        }
    }

    bool AnimatedSprite::isYFlipped() const {
        return isYAxisFlipped;
    }

    void AnimatedSprite::setYFlipped(bool isFlipped) {
        isYAxisFlipped = isFlipped;

        if(isYAxisFlipped) {
            sprite.setScale(
                sprite.getScale().x,
                -1.0f * std::abs(sprite.getScale().y)
            );
        }
    }

    Vector2<float> AnimatedSprite::getPosition() const {
        return Vector2<float>();
    }

    void AnimatedSprite::setPosition(const Vector2<float> & position) {
        sprite.setPosition(position.getX(), position.getY());
    }

} // hikari
