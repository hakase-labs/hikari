#include "hikari/client/game/objects/Projectile.hpp"
#include "hikari/client/game/objects/Motion.hpp"
#include "hikari/client/game/objects/motions/LinearMotion.hpp"
#include "hikari/client/game/events/EventBus.hpp"
#include "hikari/client/game/events/EntityDeathEventData.hpp"
#include "hikari/core/game/SpriteAnimator.hpp"
#include "hikari/core/math/Vector2.hpp"
#include "hikari/core/util/Log.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

namespace hikari {

    const std::shared_ptr<Motion> Projectile::DeflectedMotion = std::make_shared<LinearMotion>(Vector2<float>(-4.0f, -4.0f));

    Projectile::Projectile(int id, std::shared_ptr<Room> room) 
        : Entity(id, room) 
        , inert(false)
        , reflectionType(NO_REFLECTION)
    {
        body.setGravitated(false);
        body.setHasWorldCollision(false);
    }

    Projectile::Projectile(const Projectile& proto)
        : Entity(proto)
        , inert(false)
        , reflectionType(proto.reflectionType)
    {
        setActive(false);
    }

    Projectile::~Projectile() {
        
    }

    std::unique_ptr<Projectile> Projectile::clone() const {
        return std::unique_ptr<Projectile>(new Projectile(*this));
    }

    void Projectile::render(sf::RenderTarget &target) {
        Entity::render(target);
    }

    void Projectile::update(float dt) {
        Entity::update(dt);

        if(motion) {
            Vector2<float> newVelocity = motion->calculate(dt, body.getVelocity());

            setVelocityY(newVelocity.getY());

            // Flip horizontal velocity depending on direction
            if(getDirection() == Directions::Left) {
                setVelocityX(-newVelocity.getX());
            } else {
                setVelocityX(newVelocity.getX());
            }
        } else {
            if(getDirection() == Directions::Left) {
                setVelocityX(-4.0f);
            } else {
                setVelocityX(4.0f);
            }
        }
    }

    void Projectile::handleCollision(Movable& body, CollisionInfo& info) {
        if(getReflectionType() == REFLECT_X || getReflectionType() == REFLECT_XY) {
            if(info.isCollisionX) {
                if(info.directionX == Directions::Left) {
                    setDirection(Directions::Right);
                } else if(info.directionX == Directions::Right) {
                    setDirection(Directions::Left);
                }
            }
        } 

        if(getReflectionType() == REFLECT_Y || getReflectionType() == REFLECT_XY) {
            if(info.isCollisionY) {
                if(info.directionY == Directions::Up) {
                    setDirection(Directions::Down);
                } else if(info.directionY == Directions::Down) {
                    setDirection(Directions::Up);
                }
            }
        }
    }

    void Projectile::setMotion(const std::shared_ptr<Motion> motion) {
        this->motion = motion;
    }

    const std::shared_ptr<Motion>& Projectile::getMotion() const {
        return motion;
    }

    void Projectile::setReflectionType(ReflectionType type) {
        reflectionType = type;
    }

    Projectile::ReflectionType Projectile::getReflectionType() const {
        return reflectionType;
    }

    void Projectile::setInert(bool inert) {
        this->inert = inert;
    }

    bool Projectile::isInert() const {
        return inert;
    }

    void Projectile::deflect() {
        setInert(true);
        setMotion(DeflectedMotion);
    }

    void Projectile::onDeath() {
        HIKARI_LOG(debug2) << "Projectile::onDeath()";
        if(auto eventManagetPtr = getEventBus().lock()) {
            // TODO: May want to triggerEvent() instead; test and see.
            eventManagetPtr->queueEvent(EventDataPtr(new EntityDeathEventData(getId(), EntityDeathEventData::Projectile)));
        }
    }

} // hikari