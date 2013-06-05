#include "hikari/client/game/objects/Enemy.hpp"
#include "hikari/client/game/objects/EnemyBrain.hpp"
#include "hikari/core/game/SpriteAnimator.hpp"
#include "hikari/core/util/Log.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

namespace hikari {

    Enemy::Enemy(int id, std::shared_ptr<Room> room) 
        : Entity(id, room) 
        , brain(nullptr)
    {
        body.setGravitated(true);
        body.setHasWorldCollision(true);
    }

    Enemy::Enemy(const Enemy& proto)
        : Entity(proto)
        , brain(nullptr)
    {
        setActive(false);
        setGravitated(proto.isGravitated());
        setObstacle(proto.isObstacle());
        setCurrentAnimation(proto.getCurrentAnimation());
        setDirection(proto.getDirection());
        setPhasing(proto.isPhasing());
        setPosition(proto.getPosition());
        setBoundingBox(proto.getBoundingBox());
        setAnimationSet(proto.getAnimationSet());

        if(proto.brain) {
            setBrain(proto.brain->clone());
        }
    }

    Enemy::~Enemy() {
    }

    std::unique_ptr<Enemy> Enemy::clone() const {
        return std::unique_ptr<Enemy>(new Enemy(*this));
    }

    void Enemy::render(sf::RenderTarget &target) {
        Entity::render(target);
    }

    void Enemy::update(float dt) {
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