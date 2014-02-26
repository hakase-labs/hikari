#include "hikari/client/game/objects/EnemySpawner.hpp"
#include "hikari/client/game/objects/Enemy.hpp"
#include "hikari/client/game/objects/EnemyBrain.hpp"
#include "hikari/client/game/GameWorld.hpp"
#include "hikari/client/game/events/EventBus.hpp"
#include "hikari/client/game/events/ObjectRemovedEventData.hpp"

#include "hikari/core/util/Log.hpp"

namespace hikari {

    const unsigned int EnemySpawner::DEFAULT_SPAWN_LIMIT = 1;    // only spawn a single instance
    const float EnemySpawner::DEFAULT_SPAWN_RATE = 0.0; // expressed in seconds

    EnemySpawner::EnemySpawner(const std::string & enemyType, unsigned int spawnLimit, float spawnRate)
        : Spawner()
        , spawnedEnemyIds()
        , enemyType(enemyType)
        , spawnLimit(spawnLimit)
        , spawnRate(spawnRate)
        , spawnRateAccumulator(0.0f)
        , continuousFlag(spawnLimit > 1)
        , wasReawoken(false)
        , instanceConfig()
    {
        // By default spawners are continuous if they have a spawnLimit higher
        // than 1.
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

            spawnedEnemyIds.erase(
                std::remove(std::begin(spawnedEnemyIds), std::end(spawnedEnemyIds), deadEntityId)
            );
        } 
    }

    void EnemySpawner::performAction(GameWorld & world) {
        if(auto spawnedObject = world.spawnEnemy(enemyType)) {
            int objectId = spawnedObject->getId();
            spawnedObject->reset();

            if(const auto brain = spawnedObject->getBrain()) {
                brain->applyConfig(instanceConfig);
            }

            spawnedObject->setPosition(getPosition());
            spawnedObject->setActive(true);

            world.queueObjectAddition(std::shared_ptr<Enemy>(std::move(spawnedObject)));

            spawnedEnemyIds.push_back(objectId);

            // This resets the counter so that the spawner properly waits
            // to spawn another object.
            spawnRateAccumulator = spawnRate;

            wasReawoken = false;
        }
    }

    void EnemySpawner::attachEventListeners(EventBus & EventBus) {
        auto objectRemovedDelegate = EventListenerDelegate(std::bind(&EnemySpawner::handleObjectRemovedEvent, this, std::placeholders::_1));
        EventBus.addListener(objectRemovedDelegate, ObjectRemovedEventData::Type);
        eventHandlerDelegates.push_back(std::make_pair(objectRemovedDelegate, ObjectRemovedEventData::Type));
    }

    void EnemySpawner::detachEventListeners(EventBus & EventBus) {
        std::for_each(
            std::begin(eventHandlerDelegates),
            std::end(eventHandlerDelegates), 
            [&](const std::pair<EventListenerDelegate, EventType> & del) {
                bool removed = EventBus.removeListener(del.first, del.second);
                HIKARI_LOG(debug) << "EnemySpawner :: Removing event listener, type = " << del.second << ", succes = " << removed;
            }
        );

        eventHandlerDelegates.clear();
    }

    void EnemySpawner::setSpawnLimit(unsigned int limit) {
        spawnLimit = limit;

        if(spawnLimit < 1) {
            HIKARI_LOG(warning) << "A spawner (#" << getId() << ") was given a limit less than 1; setting to 1";
            spawnLimit = 1;
        }
    }

    void EnemySpawner::setSpawnRate(float rate) {
        spawnRate = rate;

        if(spawnRate < 0.0f) {
            HIKARI_LOG(warning) << "A spawner (#" << getId() << ") was given a spawn rate less than 0; setting to 0";
            spawnRate = 0.0f;
        }
    }

    void EnemySpawner::setContinuous(bool continuous) {
        continuousFlag = continuous;
    }

    void EnemySpawner::setInstanceConfig(Sqrat::Table config) {
        instanceConfig = config;
    }

    bool EnemySpawner::canSpawn() const {
        bool hasEnoughTimePassed = spawnRateAccumulator == 0.0f;
        bool hasNotReachedLimit = spawnedEnemyIds.size() < spawnLimit;

        if(isContinuous()) {
            return hasEnoughTimePassed && hasNotReachedLimit;
        } else {
            if(wasReawoken) {
                return isAwake() && hasNotReachedLimit;
            } else {
                return false;
            }
        }
    }

    bool EnemySpawner::canSleep() const {
        return spawnedEnemyIds.size() == 0;
    }

    bool EnemySpawner::isContinuous() const {
        return continuousFlag;
    }

    void EnemySpawner::onWake() {
        wasReawoken = true;
        spawnRateAccumulator = 0.0f;
    }

    void EnemySpawner::onSleep() {
        wasReawoken = false;
    }

    void EnemySpawner::reset() {
        spawnRateAccumulator = 0.0f;
        wasReawoken = false;
        spawnedEnemyIds.clear();
    }

    void EnemySpawner::onActivated() {
        GameObject::onActivated();

        HIKARI_LOG(debug3) << "EnemySpawner::onActivated(), id = " << getId();
    }

    void EnemySpawner::onDeactivated() {
        GameObject::onDeactivated();

        spawnedEnemyIds.clear();
        spawnRateAccumulator = 0.0f;

        HIKARI_LOG(debug3) << "EnemySpawner::onDeactivated(), id = " << getId();
    }

    void EnemySpawner::update(float dt) {
        spawnRateAccumulator -= dt;

        // Make sure that our accumulator doesn't go passed 0 (negative)
        spawnRateAccumulator = std::max(spawnRateAccumulator, 0.0f);
    }

} // hikari
