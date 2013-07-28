#include "hikari/client/game/objects/AnimatedSprite.hpp"
#include "hikari/core/game/Animation.hpp"
#include "hikari/core/game/AnimationSet.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>

namespace hikari {

    AnimatedSprite::AnimatedSprite()
        : sprite()
        , currentAnimation("")
        , animationSet()
        , animator(sprite)
        , isXAxisFlipped(false)
        , isYAxisFlipped(false)
    {

    }

    AnimatedSprite::AnimatedSprite(const AnimatedSprite & proto)
        : sprite(proto.sprite)
        , currentAnimation("")
        , animationSet(proto.animationSet)
        , animator(sprite)
        , isXAxisFlipped(proto.isXAxisFlipped)
        , isYAxisFlipped(proto.isYAxisFlipped)
    {
        setAnimationSet(getAnimationSet());
        setAnimation(proto.currentAnimation);
    }

    void AnimatedSprite::update(float dt) {
        animator.update(dt);
    }

    void AnimatedSprite::render(sf::RenderTarget &target) {
        target.draw(sprite);
    }

    void AnimatedSprite::setAnimation(const std::string & animationName) {
         if(animationName != currentAnimation) {
            if(auto animSet = animationSet.lock()) {
                if(animSet->has(animationName)) {
                    animator.setAnimation(animSet->get(animationName));
                    currentAnimation = animationName;
                }
            }
         }
    }

    void AnimatedSprite::setAnimationSet(const std::weak_ptr<AnimationSet> & animationSetPtr) {
        if(!animationSetPtr.expired()) {
            animationSet = animationSetPtr;

            if(auto animSet = animationSet.lock()) {
                auto & texture = animSet->getTexture();
                
                if(texture) {
                    sprite.setTexture(*texture.get());
                }
            }
        }
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
        } else {
            sprite.setScale(
                std::abs(sprite.getScale().x),
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
        } else {
            sprite.setScale(
                sprite.getScale().x,
                std::abs(sprite.getScale().y)
            );
        }
    }

    Vector2<float> AnimatedSprite::getPosition() const {
        return Vector2<float>();
    }

    void AnimatedSprite::setPosition(const Vector2<float> & position) {
        sprite.setPosition(position.getX(), position.getY());
    }

    void AnimatedSprite::rewind() {
        animator.rewind();
    }

    void AnimatedSprite::pause() {
        animator.pause();
    }

    void AnimatedSprite::unpause() {
        animator.unpause();
    }

    bool AnimatedSprite::isPaused() const {
        return animator.isPaused();
    }

} // hikari
