#include "hikari/client/game/objects/Projectile.hpp"
// #include "hikari/client/game/objects/ProjectileBrain.hpp"
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

        if(getDirection() == Directions::Left) {
            setVelocityX(-1.6f);
        } else {
            setVelocityX(1.6f);
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