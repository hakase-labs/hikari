#include "hikari/client/game/objects/EnemySpawner.hpp"
#include "hikari/client/game/objects/Enemy.hpp"
#include "hikari/client/game/GameWorld.hpp"
#include "hikari/client/game/events/EventManager.hpp"
#include "hikari/client/game/events/ObjectRemovedEventData.hpp"

#include "hikari/core/util/Log.hpp"

namespace hikari {

    EnemySpawner::EnemySpawner(const std::string & enemyType)
        : Spawner()
        , spawnedEnemyIds()
        , enemyType(enemyType)
        , hasLivingSpawn(false)
    {

    }

    EnemySpawner::~EnemySpawner() {
        // No-op
    }

    void EnemySpawner::handleObjectRemovedEvent(EventDataPtr event) {
        auto eventData = std::static_pointer_cast<ObjectRemovedEventData>(event);

        // When an enemy "dies" it means that we can potentially spawn another
        // one when we wake up.

        int deadEntityId = eventData->getObjectId();

        if(std::find(std::begin(spawnedEnemyIds), std::end(spawnedEnemyIds), deadEntityId) != std::end(spawnedEnemyIds)) {
            HIKARI_LOG(debug4) << "EnemySpawner's enemy was consumed! id = " << eventData->getObjectId();
            //setActive(false);
            spawnedEnemyIds.erase(
                std::remove(std::begin(spawnedEnemyIds), std::end(spawnedEnemyIds), deadEntityId)
            );

            hasLivingSpawn = !spawnedEnemyIds.empty();
        } 
    }

    void EnemySpawner::performAction(GameWorld & world) {
        if(!hasLivingSpawn) {
            if(auto spawnedObject = world.spawnEnemy(enemyType)) {
                int objectId = spawnedObject->getId();
                spawnedObject->reset();
                spawnedObject->setPosition(getPosition());
                spawnedObject->setActive(true);

                world.queueObjectAddition(std::shared_ptr<Enemy>(std::move(spawnedObject)));

                spawnedEnemyIds.push_back(objectId);
                hasLivingSpawn = true;
            }
        }
    }

    void EnemySpawner::attachEventListeners(EventManager & eventManager) {
        auto objectRemovedDelegate = fastdelegate::MakeDelegate(this, &EnemySpawner::handleObjectRemovedEvent);
        eventManager.addListener(objectRemovedDelegate, ObjectRemovedEventData::Type);
        eventHandlerDelegates.push_back(std::make_pair(objectRemovedDelegate, ObjectRemovedEventData::Type));
    }

    void EnemySpawner::detachEventListeners(EventManager & eventManager) {
        std::for_each(
            std::begin(eventHandlerDelegates),
            std::end(eventHandlerDelegates), 
            [&](const std::pair<EventListenerDelegate, EventType> & del) {
                bool removed = eventManager.removeListener(del.first, del.second);
                HIKARI_LOG(debug) << "EnemySpawner :: Removing event listener, type = " << del.second << ", succes = " << removed;
            }
        );

        eventHandlerDelegates.clear();
    }

    void EnemySpawner::onActivated() {
        GameObject::onActivated();

        HIKARI_LOG(debug3) << "EnemySpawner::onActivated(), id = " << getId();
    }

    void EnemySpawner::onDeactivated() {
        GameObject::onDeactivated();

        HIKARI_LOG(debug3) << "EnemySpawner::onDeactivated(), id = " << getId();
    }

} // hikari
