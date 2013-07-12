#include "hikari/client/game/objects/Particle.hpp"
#include "hikari/core/game/AnimationSet.hpp"
#include "hikari/core/game/SpriteAnimator.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace hikari {

    Particle::Particle(float maximumAge)
        : GameObject()
        , age(0.0f)
        , maximumAge(maximumAge)
        , boundingBox(0.0f, 0.0f, 0.0f, 0.0f)
        , sprite()
        , spriteTexture(nullptr)
        , animation()
        , animationSet()
        , animator(new SpriteAnimator(sprite))
    {

    }

    Particle::Particle(const Particle& proto)
        : GameObject()
        , age(0.0f)
        , maximumAge(proto.maximumAge)
        , boundingBox(proto.boundingBox)
        , sprite(proto.sprite)
        , spriteTexture(proto.spriteTexture)
        , animation(proto.animation)
        , animationSet(proto.animationSet)
        , animator(new SpriteAnimator(sprite))
    {

    }

    Particle::~Particle() {

    }

    void Particle::update(float dt) {
        GameObject::update(dt);

        animator->update(dt);

        if(age >= maximumAge) {
            // Die
            setActive(false);
        }
    }

    void Particle::render(sf::RenderTarget &target) {
        target.draw(sprite);
    }

    void Particle::setVelocity(const Vector2<float> & velocity) {
        this->velocity = velocity;
    }

    const Vector2<float> & Particle::getVelocity() const {
        return velocity;
    }

    void Particle::setBoundingBox(const BoundingBox<float> & boundingBox) {
        this->boundingBox = boundingBox;
    }

    const BoundingBox<float> & Particle::getBoundingBox() const {
        return boundingBox;
    }

    void Particle::setAnimationSet(const std::weak_ptr<AnimationSet> & animationSet) {
        this->animationSet = animationSet;
        animator->setAnimation(animation.lock());
    }

    const std::weak_ptr<AnimationSet> & Particle::getAnimationSet() const {
        return animationSet;
    }

    void Particle::setSpriteTexture(const std::shared_ptr<sf::Texture>& newTexture) {
        spriteTexture = newTexture;

        if(spriteTexture) {
            sprite.setTexture(*(spriteTexture.get()));
        }
    }

    void Particle::setCurrentAnimation(const std::string & animationName) {
        if(auto set = animationSet.lock()) {
            if(set->has(animationName)) {
                animation = set->get(animationName);
            }
        }

        animator->setAnimation(animation.lock());
    }

} // hikari
