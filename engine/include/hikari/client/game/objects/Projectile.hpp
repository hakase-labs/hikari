#ifndef HIKARI_CLIENT_GAME_OBJECTS_PROJECTILE
#define HIKARI_CLIENT_GAME_OBJECTS_PROJECTILE

#include "hikari/client/game/objects/Entity.hpp"
#include "hikari/core/util/Cloneable.hpp"
#include <memory>

namespace hikari {

    class Motion;

    class Projectile : public Entity, public Cloneable<Projectile> {
    private:
        static const std::shared_ptr<Motion> DeflectedMotion;
        std::shared_ptr<Motion> motion;
        bool inert;

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

        /**
         * Sets whether the projectile is inert or not. When a projectile is
         * inert it is no longer effective (like when it has been deflected, for
         * example).
         * 
         * @param inert true to make the projectile inert, false otherwise
         */
        void setInert(bool inert);

        /**
         * Gets whether the projectile is inert or not.
         * 
         * @return true if it is inert, false otherwise
         */
        bool isInert() const;

        /**
         * Causes the projectile to be "deflected". This also causes the
         * projectile to become inert.
         */
        void deflect();

    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_PROJECTILE
