#include "hikari/client/game/objects/ItemSpawner.hpp"
#include "hikari/client/game/objects/CollectableItem.hpp"
#include "hikari/client/game/GameWorld.hpp"

#include "hikari/core/util/Log.hpp"

namespace hikari {

    ItemSpawner::ItemSpawner(const std::string & itemName)
        : Spawner()
        , itemName(itemName)
        , spawnedItem()
    {

    }

    ItemSpawner::~ItemSpawner() {
        // No-op
    }

    void ItemSpawner::performAction(GameWorld & world) {
        auto spawnedObject = world.spawnCollectableItem(itemName);

        if(spawnedObject) {
            spawnedObject->reset();
            spawnedObject->setPosition(getPosition());
            spawnedObject->setActive(true);

            spawnedItem = std::weak_ptr<CollectableItem>(spawnedObject);
        }

        world.queueObjectAddition(spawnedObject);
    }

    std::weak_ptr<CollectableItem> ItemSpawner::getSpawnedItem() const {
        return spawnedItem;
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
