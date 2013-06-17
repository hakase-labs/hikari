#include "hikari/client/game/GameWorld.hpp"
#include "hikari/client/game/events/EventManager.hpp"
#include "hikari/client/game/events/ObjectRemovedEventData.hpp"
#include "hikari/client/game/objects/GameObject.hpp"
#include "hikari/client/game/objects/CollectableItem.hpp"
#include "hikari/client/game/objects/Enemy.hpp"
#include "hikari/client/game/objects/Projectile.hpp"
#include "hikari/client/game/objects/Hero.hpp"
#include "hikari/client/game/objects/ItemFactory.hpp"
#include "hikari/client/game/objects/EnemyFactory.hpp"
#include "hikari/client/game/objects/ProjectileFactory.hpp"

#include "hikari/core/game/map/Room.hpp"
#include "hikari/core/util/Log.hpp"
#include "hikari/core/util/exception/HikariException.hpp"

// #include <oolua.h>

namespace hikari {

    GameWorld::GameWorld() 
        : eventManager()
        , player(nullptr)
        , currentRoom(nullptr)
        , itemFactory()
        , enemyFactory()
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
        , objectRegistry()
        , gravityEnabled(true)
        
    {

    }

    GameWorld::~GameWorld() {
        // no-op
    }

    void GameWorld::setEventManager(const std::weak_ptr<EventManager>& eventManager) {
        this->eventManager = eventManager;
    }

    const std::weak_ptr<EventManager>& GameWorld::getEventManager() const {
        return eventManager;
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

    void GameWorld::setProjectileFactory(const std::weak_ptr<ProjectileFactory> & projectileFactory) {
        this->projectileFactory = projectileFactory;
    }
        
    void GameWorld::render(sf::RenderTarget &target) {

    }

    void GameWorld::update(float dt) {
        //HIKARI_LOG(debug) << "GameWorld::update begin; dt = " << dt;
        processRemovals();
        processAdditions();
        
        /*
        std::for_each(
            activeObjects.begin(), 
            activeObjects.end(), 
            std::bind(&GameObject::update, std::placeholders::_1, std::cref(dt)));
        */
        //HIKARI_LOG(debug) << "GameWorld::update finish";
    }

    void GameWorld::queueObjectAddition(const std::shared_ptr<GameObject> &obj) {
        if(obj) {
            queuedAdditions.push(obj);
        } else {
            HIKARI_LOG(debug) << "Tried to add a null object (game object); ignoring.";
        }
    }

    void GameWorld::queueObjectAddition(const std::shared_ptr<CollectableItem> &obj) {
        if(obj) {
            queuedItemAdditions.push(obj);
        } else {
            HIKARI_LOG(debug) << "Tried to add a null object (item); ignoring.";
        }
    }

    void GameWorld::queueObjectAddition(const std::shared_ptr<Enemy> &obj) {
        if(obj) {
            queuedEnemyAdditions.push(obj);
        } else {
            HIKARI_LOG(debug) << "Tried to add a null object (enemy); ignoring.";
        }
    }

    void GameWorld::queueObjectAddition(const std::shared_ptr<Projectile> &obj) {
        if(obj) {
            queuedProjectileAdditions.push(obj);
        } else {
            HIKARI_LOG(debug) << "Tried to add a null object (projectile); ignoring.";
        }
    }

    void GameWorld::queueObjectRemoval(const std::shared_ptr<GameObject> &obj) {
        if(obj) {
            queuedRemovals.push(obj);
        } else {
            HIKARI_LOG(debug) << "Tried to remove a null object (game object); ignoring.";
        }
    }

    void GameWorld::queueObjectRemoval(const std::shared_ptr<CollectableItem> &obj) {
        if(obj) {
            queuedItemRemovals.push(obj);
        } else {
            HIKARI_LOG(debug) << "Tried to remove a null object (item); ignoring.";
        }
    }

    void GameWorld::queueObjectRemoval(const std::shared_ptr<Enemy> &obj) {
        if(obj) {
            queuedEnemyRemovals.push(obj);
        } else {
            HIKARI_LOG(debug) << "Tried to remove a null object (enemy); ignoring.";
        }
    }

    void GameWorld::queueObjectRemoval(const std::shared_ptr<Projectile> &obj) {
        if(obj) {
            queuedProjectileRemovals.push(obj);
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

            queuedAdditions.pop();
        }

        // Collectable Items
        while(!queuedItemAdditions.empty()) {
            auto objectToBeAdded = queuedItemAdditions.front();

            activeItems.push_back(objectToBeAdded);
            objectRegistry.emplace(std::make_pair(objectToBeAdded->getId(), objectToBeAdded));

            objectToBeAdded->setRoom(getCurrentRoom());
            objectToBeAdded->setEventManager(getEventManager());

            queuedItemAdditions.pop();
        }

        // Enemies
        while(!queuedEnemyAdditions.empty()) {
            auto objectToBeAdded = queuedEnemyAdditions.front();

            activeEnemies.push_back(objectToBeAdded);
            objectRegistry.emplace(std::make_pair(objectToBeAdded->getId(), objectToBeAdded));

            objectToBeAdded->setRoom(getCurrentRoom());
            objectToBeAdded->setEventManager(getEventManager());

            queuedEnemyAdditions.pop();
        }

        // Projectiles
        while(!queuedProjectileAdditions.empty()) {
            auto objectToBeAdded = queuedProjectileAdditions.front();

            activeProjectiles.push_back(objectToBeAdded);
            objectRegistry.emplace(std::make_pair(objectToBeAdded->getId(), objectToBeAdded));

            objectToBeAdded->setRoom(getCurrentRoom());
            objectToBeAdded->setEventManager(getEventManager());

            queuedProjectileAdditions.pop();
        }
    }

    void GameWorld::processRemovals() {
        auto eventManagerPtr = eventManager.lock();

        while(!queuedRemovals.empty()) {
            auto objectToBeRemoved = queuedRemovals.front();
            
            activeObjects.erase(
                std::remove(std::begin(activeObjects), std::end(activeObjects), objectToBeRemoved));

            objectRegistry.erase(objectToBeRemoved->getId());
            
            queuedRemovals.pop();

            //objectToBeRemoved->setEventManager(std::weak_ptr<EventManager>());

            if(eventManagerPtr) {
                eventManagerPtr->queueEvent(std::make_shared<ObjectRemovedEventData>(objectToBeRemoved->getId()));
            }
        }

        while(!queuedItemRemovals.empty()) {
            auto objectToBeRemoved = queuedItemRemovals.front();
            
            activeItems.erase(
                std::remove(std::begin(activeItems), std::end(activeItems), objectToBeRemoved));

            objectRegistry.erase(objectToBeRemoved->getId());
            
            queuedItemRemovals.pop();

            objectToBeRemoved->setEventManager(std::weak_ptr<EventManager>());

            if(eventManagerPtr) {
                eventManagerPtr->queueEvent(std::make_shared<ObjectRemovedEventData>(objectToBeRemoved->getId()));
            }
        }

        while(!queuedEnemyRemovals.empty()) {
            auto objectToBeRemoved = queuedEnemyRemovals.front();
            
            activeEnemies.erase(
                std::remove(std::begin(activeEnemies), std::end(activeEnemies), objectToBeRemoved));

            objectRegistry.erase(objectToBeRemoved->getId());
            
            queuedEnemyRemovals.pop();

            objectToBeRemoved->setEventManager(std::weak_ptr<EventManager>());

            if(eventManagerPtr) {
                eventManagerPtr->queueEvent(std::make_shared<ObjectRemovedEventData>(objectToBeRemoved->getId()));
            }
        }

        while(!queuedProjectileRemovals.empty()) {
            auto objectToBeRemoved = queuedProjectileRemovals.front();
            
            activeProjectiles.erase(
                std::remove(std::begin(activeProjectiles), std::end(activeProjectiles), objectToBeRemoved));

            objectRegistry.erase(objectToBeRemoved->getId());
            
            queuedProjectileRemovals.pop();

            objectToBeRemoved->setEventManager(std::weak_ptr<EventManager>());

            if(eventManagerPtr) {
                eventManagerPtr->queueEvent(std::make_shared<ObjectRemovedEventData>(objectToBeRemoved->getId()));
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

    const std::vector<std::shared_ptr<CollectableItem>> & GameWorld::getActiveItems() const {
        return activeItems;
    }

    const std::vector<std::shared_ptr<Enemy>> & GameWorld::getActiveEnemies() const {
        return activeEnemies;
    }

    const std::vector<std::shared_ptr<Projectile>> & GameWorld::getActiveProjectiles() const {
        return activeProjectiles;
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