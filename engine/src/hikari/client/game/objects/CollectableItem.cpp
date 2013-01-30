#include "hikari/client/game/objects/CollectableItem.hpp"
#include "hikari/client/game/Effect.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include "hikari/core/game/map/Room.hpp"
#include "hikari/core/util/Log.hpp"

namespace hikari {

    const float CollectableItem::DEFAULT_AGE_IN_M_SECONDS = 0.0f;
    const float CollectableItem::DEFAULT_MAXIMUM_AGE_IN_M_SECONDS = 10.0f;

    CollectableItem::CollectableItem(const int& id, std::shared_ptr<Room> room, std::shared_ptr<Effect> effect)
        : Entity(id, room)
        , effect(effect)
        , ageless(false)
        , age(DEFAULT_AGE_IN_M_SECONDS)
        , maximumAge(DEFAULT_MAXIMUM_AGE_IN_M_SECONDS)
    {
        body.setGravitated(true);
        body.setHasWorldCollision(true);

        /*
        this->collisionCallback = [&](Movable& movable, CollisionInfo& collision) {
            HIKARI_LOG(info) 
                << "Movable::collisionCallback "
                << "(cx? = " << collision.isCollisionX << ", "
                << "dx = " << (collision.directionX == Directions::Left ? "LEFT" : "RIGHT") << ", "
                << "cy? = " << collision.isCollisionY << ", "
                << "dy = " << (collision.directionY == Directions::Up ? "UP" : "DOWN") << ")";
        };
        */
    }

    CollectableItem::~CollectableItem() {

    }

    std::shared_ptr<CollectableItem> CollectableItem::clone() const {
        // Deep copy the effect object
        //auto clonedEffect = std::make_shared<Effect>(getEffect().get());
        auto clonedBoundingBox = getBoundingBox();

        auto clone = std::make_shared<CollectableItem>(
            GameObject::generateObjectId(), nullptr, getEffect()->clone());

    
        clone->setBoundingBox(clonedBoundingBox);
        clone->setActive(false);
        //clone->setGravitated(this->isGravitated());
        clone->setAgeless(this->isAgeless());
        clone->setAnimationSet(this->getAnimationSet());

        return clone;
    }

    const float& CollectableItem::getAge() const {
        return age;
    }

    void CollectableItem::setAge(const float& newAge) {
        age = newAge;
    }

    const float& CollectableItem::getMaximumAge() const {
        return maximumAge;
    }

    void CollectableItem::setMaximumAge(const float& newMaximumAge) {
        maximumAge = newMaximumAge;
    }

    const bool& CollectableItem::isAgeless() const {
        return ageless;
    }

    void CollectableItem::setAgeless(const bool& isAgeless) {
        ageless = isAgeless;
    }

    void CollectableItem::setEffect(std::shared_ptr<Effect> newEffect) {
        effect = newEffect;
    }

    std::shared_ptr<Effect> CollectableItem::getEffect() const {
        return effect;
    }

    void CollectableItem::onBirth() {
        Entity::onBirth();
        //setActive(true);
    }

    void CollectableItem::onDeath() {
        Entity::onDeath();
        //setActive(false);
    }

    void CollectableItem::onSleep() {
        Entity::onSleep();
        //setActive(false);
    }

    void CollectableItem::onWake() {
        Entity::onWake();
        //setActive(true);
    }

    void CollectableItem::update(const float& dt) {
        if(isActive()) {
            Entity::update(dt);

            if(!isAgeless()) {
                setAge(getAge() + dt);

                if(getAge() >= getMaximumAge()) {
                    onDeath();
                }
            }
        }

        getSprite().setPosition(
            std::floor(getBoundingBox().getLeft()), 
            std::floor(getBoundingBox().getTop())
        );

        getSprite().setOrigin(getBoundingBox().getOrigin().getX(), getBoundingBox().getOrigin().getY());
    }

    void CollectableItem::render(sf::RenderTarget &target) {
        Entity::render(target);
        target.draw(getSprite());
    }

    void CollectableItem::reset() {
        Entity::reset();
        setActive(false);
        setAge(DEFAULT_AGE_IN_M_SECONDS);
    }

} // hikari