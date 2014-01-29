#include "hikari/client/game/GameWorld.hpp"
#include "hikari/client/game/events/EventBus.hpp"
#include "hikari/client/game/events/ObjectRemovedEventData.hpp"
#include "hikari/client/game/objects/GameObject.hpp"
#include "hikari/client/game/objects/CollectableItem.hpp"
#include "hikari/client/game/objects/Enemy.hpp"
#include "hikari/client/game/objects/Particle.hpp"
#include "hikari/client/game/objects/Projectile.hpp"
#include "hikari/client/game/objects/Hero.hpp"
#include "hikari/client/game/objects/ItemFactory.hpp"
#include "hikari/client/game/objects/EnemyFactory.hpp"
#include "hikari/client/game/objects/ParticleFactory.hpp"
#include "hikari/client/game/objects/ProjectileFactory.hpp"

#include "hikari/core/game/map/Room.hpp"
#include "hikari/core/util/Log.hpp"
#include "hikari/core/util/exception/HikariException.hpp"

namespace hikari {

    GameWorld::GameWorld() 
        : eventBus()
        , player(nullptr)
        , currentRoom(nullptr)
        , itemFactory()
        , enemyFactory()
        , particleFactory()
        , projectileFactory()
        , queuedAdditions()
        , queuedRemovals()
        , activeObjects()
        , queuedItemAdditions()
        , queuedItemRemovals()
        , activeItems()
        , queuedEnemyAdditions()
        , queuedEnemyRemovals()
        , activeEnemies()
        , queuedParticleAdditions()
        , queuedParticleRemovals()
        , activeParticles()
        , objectRegistry()
        , gravityEnabled(true)
        
    {

    }

    GameWorld::~GameWorld() {
        // no-op
    }

    void GameWorld::setEventBus(const std::weak_ptr<EventBus>& eventBus) {
        this->eventBus = eventBus;
    }

    const std::weak_ptr<EventBus>& GameWorld::getEventBus() const {
        return eventBus;
    }

    void GameWorld::setCurrentRoom(const std::shared_ptr<Room> & room) {
        currentRoom = room;
    }

    const std::shared_ptr<Room> & GameWorld::getCurrentRoom() const {
        return currentRoom;
    }

    void GameWorld::setItemFactory(const std::weak_ptr<ItemFactory> & itemFactory) {
        this->itemFactory = itemFactory;
    }

    void GameWorld::setEnemyFactory(const std::weak_ptr<EnemyFactory> & enemyFactory) {
        this->enemyFactory = enemyFactory;
    }

    void GameWorld::setParticleFactory(const std::weak_ptr<ParticleFactory> & particleFactory) {
        this->particleFactory = particleFactory;
    }

    void GameWorld::setProjectileFactory(const std::weak_ptr<ProjectileFactory> & projectileFactory) {
        this->projectileFactory = projectileFactory;
    }
        
    void GameWorld::render(sf::RenderTarget &target) {

    }

    void GameWorld::update(float dt) {
        processRemovals();
        processAdditions();
    }

    void GameWorld::queueObjectAddition(const std::shared_ptr<GameObject> &obj) {
        if(obj) {
            queuedAdditions.push_back(obj);
        } else {
            HIKARI_LOG(debug) << "Tried to add a null object (game object); ignoring.";
        }
    }

    void GameWorld::queueObjectAddition(const std::shared_ptr<CollectableItem> &obj) {
        if(obj) {
            queuedItemAdditions.push_back(obj);
        } else {
            HIKARI_LOG(debug) << "Tried to add a null object (item); ignoring.";
        }
    }

    void GameWorld::queueObjectAddition(const std::shared_ptr<Enemy> &obj) {
        if(obj) {
            queuedEnemyAdditions.push_back(obj);
        } else {
            HIKARI_LOG(debug) << "Tried to add a null object (enemy); ignoring.";
        }
    }

    void GameWorld::queueObjectAddition(const std::shared_ptr<Particle> &obj) {
        if(obj) {
            queuedParticleAdditions.push_back(obj);
        } else {
            HIKARI_LOG(debug) << "Tried to add a null object (particle); ignoring.";
        }
    }

    void GameWorld::queueObjectAddition(const std::shared_ptr<Projectile> &obj) {
        if(obj) {
            queuedProjectileAdditions.push_back(obj);
        } else {
            HIKARI_LOG(debug) << "Tried to add a null object (projectile); ignoring.";
        }
    }

    void GameWorld::queueObjectRemoval(const std::shared_ptr<GameObject> &obj) {
        if(obj) {
            auto finder = std::find(
                std::begin(queuedRemovals),
                std::end(queuedRemovals), 
                obj);

            // Avoid double-enqueueing
            if(finder == std::end(queuedRemovals)) {
                queuedRemovals.push_back(obj);
            }
        } else {
            HIKARI_LOG(debug) << "Tried to remove a null object (game object); ignoring.";
        }
    }

    void GameWorld::queueObjectRemoval(const std::shared_ptr<CollectableItem> &obj) {
        if(obj) {
            auto finder = std::find(
                std::begin(queuedItemRemovals),
                std::end(queuedItemRemovals), 
                obj);

            // Avoid double-enqueueing
            if(finder == std::end(queuedItemRemovals)) {
                queuedItemRemovals.push_back(obj);
            }
        } else {
            HIKARI_LOG(debug) << "Tried to remove a null object (item); ignoring.";
        }
    }

    void GameWorld::queueObjectRemoval(const std::shared_ptr<Enemy> &obj) {
        if(obj) {
            auto finder = std::find(
                std::begin(queuedEnemyRemovals),
                std::end(queuedEnemyRemovals), 
                obj);

            // Avoid double-enqueueing
            if(finder == std::end(queuedEnemyRemovals)) {
                queuedEnemyRemovals.push_back(obj);
            }
        } else {
            HIKARI_LOG(debug) << "Tried to remove a null object (enemy); ignoring.";
        }
    }

    void GameWorld::queueObjectRemoval(const std::shared_ptr<Particle> &obj) {
        if(obj) {
            auto finder = std::find(
                std::begin(queuedParticleRemovals),
                std::end(queuedParticleRemovals), 
                obj);

            // Avoid double-enqueueing
            if(finder == std::end(queuedParticleRemovals)) {
                queuedParticleRemovals.push_back(obj);
            }
        } else {
            HIKARI_LOG(debug) << "Tried to remove a null object (particle); ignoring.";
        }
    }

    void GameWorld::queueObjectRemoval(const std::shared_ptr<Projectile> &obj) {
        if(obj) {
            auto finder = std::find(
                std::begin(queuedProjectileRemovals),
                std::end(queuedProjectileRemovals), 
                obj);

            // Avoid double-enqueueing
            if(finder == std::end(queuedProjectileRemovals)) {
                queuedProjectileRemovals.push_back(obj);
            }
        } else {
            HIKARI_LOG(debug) << "Tried to remove a null object (projectile); ignoring.";
        }
    }

    void GameWorld::processAdditions() {
        // Generic objects
        while(!queuedAdditions.empty()) {
            auto objectToBeAdded = queuedAdditions.front();

            activeObjects.push_back(objectToBeAdded);
            objectRegistry.emplace(std::make_pair(objectToBeAdded->getId(), objectToBeAdded));

            queuedAdditions.pop_front();
        }

        // Collectable Items
        while(!queuedItemAdditions.empty()) {
            auto objectToBeAdded = queuedItemAdditions.front();

            activeItems.push_back(objectToBeAdded);
            objectRegistry.emplace(std::make_pair(objectToBeAdded->getId(), objectToBeAdded));

            objectToBeAdded->setRoom(getCurrentRoom());
            objectToBeAdded->setEventBus(getEventBus());

            queuedItemAdditions.pop_front();
        }

        // Enemies
        while(!queuedEnemyAdditions.empty()) {
            auto objectToBeAdded = queuedEnemyAdditions.front();

            activeEnemies.push_back(objectToBeAdded);
            objectRegistry.emplace(std::make_pair(objectToBeAdded->getId(), objectToBeAdded));

            objectToBeAdded->setRoom(getCurrentRoom());
            objectToBeAdded->setEventBus(getEventBus());

            queuedEnemyAdditions.pop_front();
        }

        // Particles
        while(!queuedParticleAdditions.empty()) {
            auto objectToBeAdded = queuedParticleAdditions.front();

            activeParticles.push_back(objectToBeAdded);
            objectRegistry.emplace(std::make_pair(objectToBeAdded->getId(), objectToBeAdded));

            queuedParticleAdditions.pop_front();
        }

        // Projectiles
        while(!queuedProjectileAdditions.empty()) {
            auto objectToBeAdded = queuedProjectileAdditions.front();

            activeProjectiles.push_back(objectToBeAdded);
            objectRegistry.emplace(std::make_pair(objectToBeAdded->getId(), objectToBeAdded));

            objectToBeAdded->setRoom(getCurrentRoom());
            objectToBeAdded->setEventBus(getEventBus());

            queuedProjectileAdditions.pop_front();
        }
    }

    void GameWorld::processRemovals() {
        auto eventBusPtr = eventBus.lock();

        while(!queuedRemovals.empty()) {
            auto objectToBeRemoved = queuedRemovals.front();
            
            activeObjects.erase(
                std::remove(std::begin(activeObjects), std::end(activeObjects), objectToBeRemoved));

            objectRegistry.erase(objectToBeRemoved->getId());
            
            queuedRemovals.pop_front();

            //objectToBeRemoved->setEventBus(std::weak_ptr<EventBus>());

            if(eventBusPtr) {
                eventBusPtr->queueEvent(std::make_shared<ObjectRemovedEventData>(objectToBeRemoved->getId()));
            }
        }

        while(!queuedItemRemovals.empty()) {
            auto objectToBeRemoved = queuedItemRemovals.front();
            
            activeItems.erase(
                std::remove(std::begin(activeItems), std::end(activeItems), objectToBeRemoved));

            objectRegistry.erase(objectToBeRemoved->getId());
            
            queuedItemRemovals.pop_front();

            objectToBeRemoved->setEventBus(std::weak_ptr<EventBus>());

            if(eventBusPtr) {
                eventBusPtr->queueEvent(std::make_shared<ObjectRemovedEventData>(objectToBeRemoved->getId()));
            }
        }

        while(!queuedEnemyRemovals.empty()) {
            auto objectToBeRemoved = queuedEnemyRemovals.front();
            
            activeEnemies.erase(
                std::remove(std::begin(activeEnemies), std::end(activeEnemies), objectToBeRemoved));

            objectRegistry.erase(objectToBeRemoved->getId());
            
            queuedEnemyRemovals.pop_front();

            objectToBeRemoved->setEventBus(std::weak_ptr<EventBus>());

            if(eventBusPtr) {
                eventBusPtr->queueEvent(std::make_shared<ObjectRemovedEventData>(objectToBeRemoved->getId()));
            }
        }

        while(!queuedParticleRemovals.empty()) {
            auto objectToBeRemoved = queuedParticleRemovals.front();

            activeParticles.erase(
                std::remove(std::begin(activeParticles), std::end(activeParticles), objectToBeRemoved));

            objectRegistry.erase(objectToBeRemoved->getId());
            
            queuedParticleRemovals.pop_front();

            if(eventBusPtr) {
                eventBusPtr->queueEvent(std::make_shared<ObjectRemovedEventData>(objectToBeRemoved->getId()));
            }
        }

        while(!queuedProjectileRemovals.empty()) {
            auto objectToBeRemoved = queuedProjectileRemovals.front();

            activeProjectiles.erase(
                std::remove(std::begin(activeProjectiles), std::end(activeProjectiles), objectToBeRemoved));

            objectRegistry.erase(objectToBeRemoved->getId());
            
            queuedProjectileRemovals.pop_front();

            objectToBeRemoved->setEventBus(std::weak_ptr<EventBus>());

            if(eventBusPtr) {
                eventBusPtr->queueEvent(std::make_shared<ObjectRemovedEventData>(objectToBeRemoved->getId()));
            }
        }
    }

    void GameWorld::removeAllObjects() {
        processRemovals();

        // Items
        std::for_each(
            std::begin(activeItems),
            std::end(activeItems),
            [this](const std::shared_ptr<CollectableItem> item) {
                this->queueObjectRemoval(item);
            });

        // Enemies
        std::for_each(
            std::begin(activeEnemies),
            std::end(activeEnemies),
            [this](const std::shared_ptr<Enemy> enemy) {
                this->queueObjectRemoval(enemy);
            });

        // Particles
        std::for_each(
            std::begin(activeParticles),
            std::end(activeParticles),
            [this](const std::shared_ptr<Particle> particle) {
                this->queueObjectRemoval(particle);
            });

        // Projectiles
        std::for_each(
            std::begin(activeProjectiles),
            std::end(activeProjectiles),
            [this](const std::shared_ptr<Projectile> projectile) {
                this->queueObjectRemoval(projectile);
            });

        processRemovals();
    }

    std::shared_ptr<CollectableItem> GameWorld::spawnCollectableItem(const std::string & name /* CollectableItemInstanceConfig instanceConfig */) const {
        if(auto itemFactoryPtr = itemFactory.lock()) {
            try {
                return itemFactoryPtr->createItem(name);
            } catch(HikariException & ex) {
                HIKARI_LOG(debug) << ex.what();
            }
        }

        return std::shared_ptr<CollectableItem>(nullptr);
    }

    std::unique_ptr<Enemy> GameWorld::spawnEnemy(const std::string & name /* EnemyInstanceConfig instanceConfig */) const {
        if(auto enemyFactoryPtr = enemyFactory.lock()) {
            try {
                return enemyFactoryPtr->create(name);
            } catch(HikariException & ex) {
                HIKARI_LOG(debug) << ex.what();
            }
        }

        return std::unique_ptr<Enemy>(nullptr);
    }

    std::unique_ptr<Particle> GameWorld::spawnParticle(const std::string & name) const {
        if(auto particleFactoryPtr = particleFactory.lock()) {
            try {
                return particleFactoryPtr->create(name);
            } catch(HikariException & ex) {
                HIKARI_LOG(debug) << ex.what();
            }
        }

        return std::unique_ptr<Particle>(nullptr);
    }

    std::unique_ptr<Projectile> GameWorld::spawnProjectile(const std::string & name) const {
        if(auto projectileFactoryPtr = projectileFactory.lock()) {
            try {
                return projectileFactoryPtr->create(name);
            } catch(HikariException & ex) {
                HIKARI_LOG(debug) << ex.what();
            }
        }

        return std::unique_ptr<Projectile>(nullptr);
    }

    const std::weak_ptr<GameObject> GameWorld::getObjectById(int id) const {
        auto finder = objectRegistry.find(id);

        if(finder != std::end(objectRegistry)) {
            return std::weak_ptr<GameObject>(finder->second);
        }

        return std::weak_ptr<GameObject>();
    }

    const std::vector<std::shared_ptr<CollectableItem>> & GameWorld::getActiveItems() const {
        return activeItems;
    }

    const std::vector<std::shared_ptr<Enemy>> & GameWorld::getActiveEnemies() const {
        return activeEnemies;
    }

    const std::vector<std::shared_ptr<Particle>> & GameWorld::getActiveParticles() const {
        return activeParticles;
    }

    const std::vector<std::shared_ptr<Projectile>> & GameWorld::getActiveProjectiles() const {
        return activeProjectiles;
    }

    std::vector<std::shared_ptr<Enemy>> GameWorld::getObstacles() const {
        std::vector<std::shared_ptr<Enemy>> result;

        std::for_each(
            std::begin(getActiveEnemies()),
            std::end(getActiveEnemies()),
            [&](const std::shared_ptr<Enemy> & enemy) {
                if(enemy->isObstacle()) {
                    result.push_back(enemy);
                }
            }
        );

        return result;
    }


    void GameWorld::setPlayer(const std::shared_ptr<Hero>& player) {
        this->player = player;
    }

    const Vector2<float> GameWorld::getPlayerPosition() const {
        return player ? Vector2<float>(player->getPosition()) : Vector2<float>(0, 0);
    }

    const Direction GameWorld::getPlayerFacingDirection() const {
        return player ? player->getDirection() : Directions::None;
    }

} // hikari