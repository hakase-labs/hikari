#ifndef HIKARI_CLIENT_GAME_OBJECTS_ENEMY
#define HIKARI_CLIENT_GAME_OBJECTS_ENEMY

#include "hikari/client/game/objects/Entity.hpp"
#include "hikari/core/util/Cloneable.hpp"
#include <memory>

namespace hikari {

    class EnemyBrain;

    class Enemy : public Entity, public Cloneable<Enemy> {
    private:
        std::shared_ptr<EnemyBrain> brain;
        float hitPoints;

        /**
         * After taking damage this counts how many ticks that elapse.
         */
        int damageTickCounter;

    public:
        Enemy(int id = GameObject::generateObjectId(), std::shared_ptr<Room> room = nullptr);
        Enemy(const Enemy& proto); 
        virtual ~Enemy();

        virtual std::unique_ptr<Enemy> clone() const;

        virtual void update(float dt);
        virtual void render(sf::RenderTarget &target);

        virtual void handleCollision(Movable& body, CollisionInfo& info);

        void setBrain(const std::shared_ptr<EnemyBrain> brain);
        const std::shared_ptr<EnemyBrain>& getBrain() const;

        void setHitPoints(float hp);
        float getHitPoints() const;
        void takeDamage(float amount);

    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_ENEMY
