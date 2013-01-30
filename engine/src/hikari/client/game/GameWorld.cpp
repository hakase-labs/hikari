#include "hikari/client/game/GameWorld.hpp"
#include "hikari/client/game/objects/GameObject.hpp"
#include "hikari/client/game/objects/Hero.hpp"

#include "hikari/core/util/Log.hpp"

// #include <oolua.h>

namespace hikari {

    GameWorld::GameWorld() 
        : gravityEnabled(true)
        , player(nullptr)
    {

    }

    GameWorld::~GameWorld() {
        // no-op
    }
        
    void GameWorld::render(sf::RenderTarget &target) {

    }

    void GameWorld::update(const float &dt) {
        //HIKARI_LOG(debug) << "GameWorld::update begin; dt = " << dt;
        processAdditions();
        processRemovals();
        
        std::for_each(
            activeObjects.begin(), 
            activeObjects.end(), 
            std::bind(&GameObject::update, std::placeholders::_1, std::cref(dt)));

        //HIKARI_LOG(debug) << "GameWorld::update finish";
    }

    void GameWorld::queueObjectAddition(const std::shared_ptr<GameObject> &obj) {
        queuedAdditions.push(obj);
    }

    void GameWorld::queueObjectRemoval(const std::shared_ptr<GameObject> &obj) {
        queuedRemovals.push(obj);
    }

    void GameWorld::processAdditions() {
        while(!queuedAdditions.empty()) {
            auto objectToBeAdded = queuedAdditions.front();

            activeObjects.push_back(objectToBeAdded);

            queuedAdditions.pop();
        }
    }

    void GameWorld::processRemovals() {
        while(!queuedRemovals.empty()) {
            auto objectToBeRemoved = queuedRemovals.front();
            
            activeObjects.erase(
                std::remove(activeObjects.begin(), activeObjects.end(), objectToBeRemoved));
            
            queuedRemovals.pop();
        }
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