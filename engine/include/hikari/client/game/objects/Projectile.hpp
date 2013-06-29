#ifndef HIKARI_CLIENT_GAME_OBJECTS_PROJECTILE
#define HIKARI_CLIENT_GAME_OBJECTS_PROJECTILE

#include "hikari/client/game/objects/Entity.hpp"
#include "hikari/core/util/Cloneable.hpp"
#include <memory>

namespace hikari {

    // class ProjectileBrain;
    class Motion;

    class Projectile : public Entity, public Cloneable<Projectile> {
    private:
        // std::shared_ptr<ProjectileBrain> brain;
        std::shared_ptr<Motion> motion;
    public:
        Projectile(int id = GameObject::generateObjectId(), std::shared_ptr<Room> room = nullptr);
        Projectile(const Projectile& proto); 
        virtual ~Projectile();

        virtual std::unique_ptr<Projectile> clone() const;

        virtual void update(float dt);
        virtual void render(sf::RenderTarget &target);

        virtual void handleCollision(Movable& body, CollisionInfo& info);

        void setMotion(const std::shared_ptr<Motion> motion);
        const std::shared_ptr<Motion>& getMotion() const;

    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_PROJECTILE
