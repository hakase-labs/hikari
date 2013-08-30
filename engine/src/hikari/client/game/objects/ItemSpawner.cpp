#include "hikari/client/game/objects/ItemSpawner.hpp"
#include "hikari/client/game/objects/CollectableItem.hpp"
#include "hikari/client/game/GameWorld.hpp"
#include "hikari/client/game/events/EventBus.hpp"
#include "hikari/client/game/events/EntityDeathEventData.hpp"

#include "hikari/core/util/Log.hpp"

namespace hikari {

    ItemSpawner::ItemSpawner(const std::string & itemName)
        : Spawner()
        , spawnedItemId(-1)
        , itemName(itemName)
    {

    }

    ItemSpawner::~ItemSpawner() {
        // No-op
    }

    void ItemSpawner::handleEntityDeathEvent(EventDataPtr event) {
        auto eventData = std::static_pointer_cast<EntityDeathEventData>(event);

        // When an item "dies" it means that it was consumed.
        // Consumed items don't respawn until the ItemSpawner is set back to an active state.
        if(eventData->getEntityId() == spawnedItemId) {
            HIKARI_LOG(debug4) << "ItemSpawner's item was consumed! id = " << eventData->getEntityId();
            setActive(false);
        } 
    }

    void ItemSpawner::performAction(GameWorld & world) {
        if(auto spawnedObject = world.spawnCollectableItem(itemName)) {
            spawnedItemId = spawnedObject->getId();

            spawnedObject->reset();
            spawnedObject->setPosition(getPosition());
            spawnedObject->setAgeless(true);
            spawnedObject->setActive(true);

            world.queueObjectAddition(spawnedObject);
        }
    }

    void ItemSpawner::attachEventListeners(EventBus & EventBus) {
        auto entityDeathDelegate = EventListenerDelegate(std::bind(&ItemSpawner::handleEntityDeathEvent, this, std::placeholders::_1)); // fastdelegate::MakeDelegate(this, &ItemSpawner::handleEntityDeathEvent);
        EventBus.addListener(entityDeathDelegate, EntityDeathEventData::Type);
        eventHandlerDelegates.push_back(std::make_pair(entityDeathDelegate, EntityDeathEventData::Type));
    }

    void ItemSpawner::detachEventListeners(EventBus & EventBus) {
        std::for_each(
            std::begin(eventHandlerDelegates),
            std::end(eventHandlerDelegates), 
            [&](const std::pair<EventListenerDelegate, EventType> & del) {
                bool removed = EventBus.removeListener(del.first, del.second);
                HIKARI_LOG(debug) << "ItemSpawner :: Removing event listener, type = " << del.second << ", succes = " << removed;
            }
        );

        eventHandlerDelegates.clear();
    }

    void ItemSpawner::onActivated() {
        GameObject::onActivated();

        HIKARI_LOG(debug3) << "ItemSpawner::onActivated(), id = " << getId();
    }

    void ItemSpawner::onDeactivated() {
        GameObject::onDeactivated();

        HIKARI_LOG(debug3) << "ItemSpawner::onDeactivated(), id = " << getId();
    }

} // hikari
