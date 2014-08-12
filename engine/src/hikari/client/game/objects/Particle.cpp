#include "hikari/client/game/objects/Particle.hpp"
#include "hikari/client/game/objects/Entity.hpp"
#include "hikari/core/game/AnimationSet.hpp"
#include "hikari/core/game/SpriteAnimator.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace hikari {

    Particle::Particle(float maximumAge)
        : GameObject()
        , zIndex(0)
        , age(0.0f)
        , maximumAge(maximumAge)
        , velocity(0.0f, 0.0f)
        , boundingBox(0.0f, 0.0f, 0.0f, 0.0f)
        , sprite()
        , spriteTexture(nullptr)
        , animation()
        , animationSet()
        , animator(new SpriteAnimator(sprite))
        , trackedObject()
        , trackX(false)
        , trackY(false)
    {
        animator->rewind();
    }

    Particle::Particle(const Particle& proto)
        : GameObject()
        , zIndex(0)
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

        Vector2<float> trackingOffset;

        if(trackX) {
            if(auto ptr = trackedObject.lock()) {
                trackingOffset.setX(ptr->getPosition().getX());
            }
        }

        if(trackY) {
            if(auto ptr = trackedObject.lock()) {
                trackingOffset.setY(ptr->getPosition().getY());
            }
        }

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

    int Particle::getZIndex() const {
        return zIndex;
    }

    void Particle::setZIndex(int index) {
        zIndex = index;
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

    void Particle::setTrackedObject(const std::weak_ptr<Entity> & trackedObject) {
        this->trackedObject = trackedObject;
    }

    const std::weak_ptr<Entity> & Particle::getTrackedObject() const {
        return trackedObject;
    }

    void Particle::setTrackX(bool track) {
        trackX = track;
    }

    void Particle::setTrackY(bool track) {
        trackY = track;
    }

    bool Particle::getTrackX() const {
        return trackX;
    }

    bool Particle::getTrackY() const {
        return trackX;
    }

} // hikari
