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
    }

    CollectableItem::CollectableItem(const CollectableItem &proto)
        : Entity(proto)
        , effect(proto.effect->clone())
        , ageless(proto.ageless)
        , age(0)
        , maximumAge(proto.maximumAge)
    {
        setActive(false);
        setGravitated(proto.isGravitated());
        setObstacle(proto.isObstacle());
        setCurrentAnimation(proto.getCurrentAnimation());
        setDirection(proto.getDirection());
        setPhasing(proto.isPhasing());
        setPosition(proto.getPosition());
        setBoundingBox(proto.getBoundingBox());
        setAnimationSet(proto.getAnimationSet());
    }

    CollectableItem::~CollectableItem() {

    }

    std::shared_ptr<CollectableItem> CollectableItem::clone() const {
        return std::shared_ptr<CollectableItem>(new CollectableItem(*this));
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

    void CollectableItem::update(float dt) {
        if(isActive()) {
            Entity::update(dt);

            if(!isAgeless()) {
                setAge(getAge() + dt);

                if(getAge() >= getMaximumAge()) {
                    onDeath();
                }
            }
        }
    }

    void CollectableItem::render(sf::RenderTarget &target) {
        Entity::render(target);
    }

    void CollectableItem::reset() {
        Entity::reset();
        setActive(false);
        setAge(DEFAULT_AGE_IN_M_SECONDS);
    }

} // hikari