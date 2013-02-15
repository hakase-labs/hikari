#include "hikari/client/game/GameWorld.hpp"
#include "hikari/client/game/objects/GameObject.hpp"
#include "hikari/client/game/objects/CollectableItem.hpp"
#include "hikari/client/game/objects/Hero.hpp"
#include "hikari/client/game/objects/ItemFactory.hpp"

#include "hikari/core/util/Log.hpp"
#include "hikari/core/util/exception/HikariException.hpp"

// #include <oolua.h>

namespace hikari {

    GameWorld::GameWorld() 
        : player(nullptr)
        , itemFactory()
        , queuedAdditions()
        , queuedRemovals()
        , activeObjects()
        , queuedItemAdditions()
        , queuedItemRemovals()
        , activeItems()
        , objectRegistry()
        , gravityEnabled(true)
        
    {

    }

    GameWorld::~GameWorld() {
        // no-op
    }

    void GameWorld::setItemFactory(const std::weak_ptr<ItemFactory> & itemFactory) {
        this->itemFactory = itemFactory;
    }
        
    void GameWorld::render(sf::RenderTarget &target) {

    }

    void GameWorld::update(const float &dt) {
        //HIKARI_LOG(debug) << "GameWorld::update begin; dt = " << dt;
        processAdditions();
        processRemovals();
        
        /*
        std::for_each(
            activeObjects.begin(), 
            activeObjects.end(), 
            std::bind(&GameObject::update, std::placeholders::_1, std::cref(dt)));
        */
        //HIKARI_LOG(debug) << "GameWorld::update finish";
    }

    void GameWorld::queueObjectAddition(const std::shared_ptr<GameObject> &obj) {
        queuedAdditions.push(obj);
    }

    void GameWorld::queueObjectAddition(const std::shared_ptr<CollectableItem> &obj) {
        queuedItemAdditions.push(obj);
    }

    void GameWorld::queueObjectRemoval(const std::shared_ptr<GameObject> &obj) {
        queuedRemovals.push(obj);
    }

    void GameWorld::queueObjectRemoval(const std::shared_ptr<CollectableItem> &obj) {
        queuedItemRemovals.push(obj);
    }

    void GameWorld::processAdditions() {
        while(!queuedAdditions.empty()) {
            auto objectToBeAdded = queuedAdditions.front();

            activeObjects.push_back(objectToBeAdded);
            objectRegistry.emplace(std::make_pair(objectToBeAdded->getId(), objectToBeAdded));

            queuedAdditions.pop();
        }

        while(!queuedItemAdditions.empty()) {
            auto objectToBeAdded = queuedItemAdditions.front();

            activeItems.push_back(objectToBeAdded);
            objectRegistry.emplace(std::make_pair(objectToBeAdded->getId(), objectToBeAdded));

            queuedItemAdditions.pop();
        }
    }

    void GameWorld::processRemovals() {
        while(!queuedItemRemovals.empty()) {
            auto objectToBeRemoved = queuedRemovals.front();
            
            activeItems.erase(
                std::remove(std::begin(activeItems), std::end(activeItems), objectToBeRemoved));

            objectRegistry.erase(objectToBeRemoved->getId());
            
            queuedRemovals.pop();
        }
    }

    std::shared_ptr<CollectableItem> GameWorld::spawnCollectableItem(const std::string & name /* CollectableItemInstanceConfig instanceConfig */) const {
        if(auto itemFactoryPtr = itemFactory.lock()) {
            try {
                return itemFactoryPtr->createItem(name);
            } catch(HikariException & ex) {
                return std::shared_ptr<CollectableItem>();
            }
        }

        return std::shared_ptr<CollectableItem>();
    }

    const std::vector<std::shared_ptr<CollectableItem>> & GameWorld::getActiveItems() const {
        return activeItems;
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