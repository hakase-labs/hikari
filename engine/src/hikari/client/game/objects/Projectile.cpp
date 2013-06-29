#include "hikari/client/game/objects/Projectile.hpp"
#include "hikari/client/game/objects/Motion.hpp"
#include "hikari/core/game/SpriteAnimator.hpp"
#include "hikari/core/util/Log.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

namespace hikari {

    Projectile::Projectile(int id, std::shared_ptr<Room> room) 
        : Entity(id, room) 
        // , brain(nullptr)
    {
        body.setGravitated(false);
        body.setHasWorldCollision(false);
    }

    Projectile::Projectile(const Projectile& proto)
        : Entity(proto)
        // , brain(nullptr)
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

        // if(proto.brain) {
        //     setBrain(proto.brain->clone());
        // }
    }

    Projectile::~Projectile() {
        
    }

    std::unique_ptr<Projectile> Projectile::clone() const {
        HIKARI_LOG(debug4) << "Cloned a projectile!!!";
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
        
        // if(brain) {
        //     brain->update(dt);
        // }
    }

    void Projectile::handleCollision(Movable& body, CollisionInfo& info) {
        // if(brain) {
        //     brain->handleCollision(body, info);
        // }
    }

    void Projectile::setMotion(const std::shared_ptr<Motion> motion) {
        this->motion = motion;
    }

    const std::shared_ptr<Motion>& Projectile::getMotion() const {
        return motion;
    }

    // void Projectile::setBrain(const std::shared_ptr<ProjectileBrain> newBrain) {
    //     if(newBrain) {
    //         brain = newBrain;
    //         brain->attach(this);
    //     }
    // }

    // const std::shared_ptr<ProjectileBrain>& Projectile::getBrain() const {
    //     return brain;
    // }

} // hikari