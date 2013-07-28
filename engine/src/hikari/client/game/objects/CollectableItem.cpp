#include "hikari/client/game/objects/CollectableItem.hpp"
#include "hikari/client/game/events/EventManager.hpp"
#include "hikari/client/game/events/EntityDeathEventData.hpp"
#include "hikari/client/game/Effect.hpp"

#include "hikari/core/game/map/Room.hpp"
#include "hikari/core/util/Log.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace hikari {

    const float CollectableItem::DEFAULT_AGE_IN_M_SECONDS = 0.0f;
    const float CollectableItem::DEFAULT_MAXIMUM_AGE_IN_M_SECONDS = 10.0f;

    CollectableItem::CollectableItem(int id, std::shared_ptr<Room> room, std::shared_ptr<Effect> effect)
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
    }

    CollectableItem::~CollectableItem() {

    }

    std::unique_ptr<CollectableItem> CollectableItem::clone() const {
        return std::unique_ptr<CollectableItem>(new CollectableItem(*this));
    }

    float CollectableItem::getAge() const {
        return age;
    }

    void CollectableItem::setAge(float newAge) {
        age = newAge;
    }

    float CollectableItem::getMaximumAge() const {
        return maximumAge;
    }

    void CollectableItem::setMaximumAge(float newMaximumAge) {
        maximumAge = newMaximumAge;
    }

    bool CollectableItem::isAgeless() const {
        return ageless;
    }

    void CollectableItem::setAgeless(bool isAgeless) {
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
    }

    void CollectableItem::onDeath() {
        HIKARI_LOG(debug2) << "CollectableItem::onDeath()";
        if(auto eventManagetPtr = getEventManager().lock()) {
            // TODO: May want to triggerEvent() instead; test and see.
            eventManagetPtr->queueEvent(EventDataPtr(new EntityDeathEventData(getId(), EntityDeathEventData::Item)));
        }
    }

    void CollectableItem::onSleep() {
        Entity::onSleep();
    }

    void CollectableItem::onWake() {
        Entity::onWake();
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
