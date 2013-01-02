#include <hikari/client/game/objects/Enemy.hpp>
#include <hikari/client/game/objects/EnemyBrain.hpp>
#include <hikari/core/game/SpriteAnimator.hpp>
#include <hikari/core/util/Log.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace hikari {

    Enemy::Enemy(const int &id, std::shared_ptr<Room> room) 
        : Entity(id, room) 
        , brain(nullptr)
    {
        body.setGravitated(true);
        body.setHasWorldCollision(true);
    }

    Enemy::~Enemy() {

    }

    void Enemy::render(sf::RenderTarget &target) {
        Entity::render(target);
    }

    void Enemy::update(const float &dt) {
        Entity::update(dt);
        
        if(brain) {
            brain->update(dt);
        }
    }

    void Enemy::handleCollision(Movable& body, CollisionInfo& info) {
        if(brain) {
            brain->handleCollision(body, info);
        }
    }

    void Enemy::setBrain(const std::shared_ptr<EnemyBrain> newBrain) {
        if(newBrain) {
            brain = newBrain;
            brain->attach(this);
        }
    }

    const std::shared_ptr<EnemyBrain>& Enemy::getBrain() const {
        return brain;
    }

} // hikari