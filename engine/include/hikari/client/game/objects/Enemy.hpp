#ifndef HIKARI_CLIENT_GAME_OBJECTS_ENEMY
#define HIKARI_CLIENT_GAME_OBJECTS_ENEMY

#include <hikari/client/game/objects/Entity.hpp>
#include <memory>

namespace hikari {

    class EnemyBrain;

    class Enemy : public Entity {
    private:
        std::shared_ptr<EnemyBrain> brain;
    public:
        Enemy(const int& id = GameObject::generateObjectId(), std::shared_ptr<Room> room = nullptr);
        virtual ~Enemy();

        virtual void update(const float& dt);
        virtual void render(sf::RenderTarget &target);

        virtual void handleCollision(Movable& body, CollisionInfo& info);

        void setBrain(const std::shared_ptr<EnemyBrain> brain);
        const std::shared_ptr<EnemyBrain>& getBrain() const;
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_ENEMY
