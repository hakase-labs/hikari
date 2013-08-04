#include "hikari/client/game/objects/CollectableItem.hpp"
#include "hikari/client/game/events/EventManager.hpp"
#include "hikari/client/game/events/EntityDeathEventData.hpp"
#include "hikari/client/game/Effect.hpp"

#include "hikari/core/game/map/Room.hpp"
#include "hikari/core/util/Log.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace hikari {

    CollectableItem::CollectableItem(int id, std::shared_ptr<Room> room, std::shared_ptr<Effect> effect)
        : Entity(id, room)
        , effect(effect)
    {
        body.setGravitated(true);
        body.setHasWorldCollision(true);
    }

    CollectableItem::CollectableItem(const CollectableItem &proto)
        : Entity(proto)
        , effect(proto.effect->clone())
    {
        setActive(false);
    }

    CollectableItem::~CollectableItem() {

    }

    std::unique_ptr<CollectableItem> CollectableItem::clone() const {
        return std::unique_ptr<CollectableItem>(new CollectableItem(*this));
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
        }
    }

    void CollectableItem::render(sf::RenderTarget &target) {
        Entity::render(target);
    }

    void CollectableItem::reset() {
        Entity::reset();
    }

} // hikari
