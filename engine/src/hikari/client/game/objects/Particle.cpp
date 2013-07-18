#include "hikari/client/game/objects/Particle.hpp"
#include "hikari/core/game/AnimationSet.hpp"
#include "hikari/core/game/SpriteAnimator.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace hikari {

    Particle::Particle(float maximumAge)
        : GameObject()
        , age(0.0f)
        , maximumAge(maximumAge)
        , velocity(0.0f, 0.0f)
        , boundingBox(0.0f, 0.0f, 0.0f, 0.0f)
        , sprite()
        , spriteTexture(nullptr)
        , animation()
        , animationSet()
        , animator(new SpriteAnimator(sprite))
    {
        animator->rewind();
    }

    Particle::Particle(const Particle& proto)
        : GameObject()
        , age(0.0f)
        , maximumAge(proto.maximumAge)
        , velocity(proto.velocity)
        , boundingBox(proto.boundingBox)
        , sprite(proto.sprite)
        , spriteTexture(proto.spriteTexture)
        , animation(proto.animation)
        , animationSet(proto.animationSet)
        , animator(new SpriteAnimator(sprite))
    {
        setAnimationSet(animationSet);
        setSpriteTexture(spriteTexture);
        animator->setAnimation(animation.lock());
        animator->rewind();
    }

    Particle::~Particle() {

    }

    std::unique_ptr<Particle> Particle::clone() const {
        return std::unique_ptr<Particle>(new Particle(*this));
    }

    void Particle::update(float dt) {
        GameObject::update(dt);

        animator->update(dt);
        
        setPosition(getPosition() + getVelocity());

        age += dt;

        if(getMaximumAge() > 0 && (age >= getMaximumAge())) {
            // Die
            setActive(false);
        }
    }

    void Particle::render(sf::RenderTarget &target) {
        auto position = getPosition();

        sprite.setPosition(
            std::floor(position.getX()), 
            std::floor(position.getY())
        );

        target.draw(sprite);
    }

    void Particle::setMaximumAge(float maximumAge) {
        this->maximumAge = maximumAge;
    }

    float Particle::getMaximumAge() const {
        return maximumAge;
    }

    void Particle::setPosition(const Vector2<float> & position) {
        boundingBox.setPosition(position);
    }

    const Vector2<float> & Particle::getPosition() const {
        return boundingBox.getPosition();
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
