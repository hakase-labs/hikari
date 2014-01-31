#include "hikari/client/game/objects/Enemy.hpp"
#include "hikari/client/game/objects/EnemyBrain.hpp"
#include "hikari/client/game/events/EventBus.hpp"
#include "hikari/client/game/events/EntityDeathEventData.hpp"
#include "hikari/core/game/SpriteAnimator.hpp"
#include "hikari/core/util/Log.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

namespace hikari {

    Enemy::Enemy(int id, std::shared_ptr<Room> room) 
        : Entity(id, room) 
        , brain(nullptr)
        , hitPoints(0.0)
        , damageTickCounter(0)
    {
        setAgeless(true);
        setGravitated(true);
        setPhasing(false);
        setFaction(Factions::Enemy);
        setDeathType(EntityDeathType::Small);
    }

    Enemy::Enemy(const Enemy& proto)
        : Entity(proto)
        , brain(nullptr)
        , hitPoints(proto.hitPoints)
        , damageTickCounter(0)
    {   
        setAgeless(true);
        setActive(false);

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
        if(damageTickCounter == 0) {
            Entity::render(target);
        }
    }

    void Enemy::update(float dt) {
        Entity::update(dt);

        if(getHitPoints() <= 0.0f) {
            if(auto eventManagetPtr = getEventBus().lock()) {
                eventManagetPtr->queueEvent(EventDataPtr(new EntityDeathEventData(getId(), EntityDeathEventData::Enemy)));
            }
        }
        
        if(brain) {
            brain->update(dt);
        }

        if(damageTickCounter > 0) {
            // setVulnerable(false);
            damageTickCounter++;
            damageTickCounter %= 2;
        } else {
            // setVulnerable(true);
        }
    }

    void Enemy::handleCollision(Movable& body, CollisionInfo& info) {
        if(brain) {
            brain->handleCollision(body, info);
        }
    }

    void Enemy::handleObjectTouch(int otherId) {
        // HIKARI_LOG(debug4) << "I'm being touched by something! My ID = " << getId() << ", Other ID = " << otherId;
        if(brain) {
            brain->handleObjectTouch(otherId);
        }
    }

    void Enemy::setBrain(const std::shared_ptr<EnemyBrain> & newBrain) {
        if(newBrain) {
            brain = newBrain;
            brain->attach(this);
        }
    }

    const std::shared_ptr<EnemyBrain>& Enemy::getBrain() const {
        return brain;
    }

    void Enemy::setHitPoints(float hp) {
        hitPoints = hp;
    }

    float Enemy::getHitPoints() const {
        return hitPoints;
    }

    void Enemy::takeDamage(float amount) {
        setHitPoints(getHitPoints() - amount);
        damageTickCounter = 1;
    }

} // hikari