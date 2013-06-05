#ifndef HIKARI_CLIENT_GAME_OBJECTS_ENEMYBRAIN
#define HIKARI_CLIENT_GAME_OBJECTS_ENEMYBRAIN

#include "hikari/client/game/objects/Enemy.hpp"
#include "hikari/core/util/Cloneable.hpp"

namespace hikari {

    class Movable;
    class CollisionInfo;

    class EnemyBrain : public Cloneable<EnemyBrain> {
    private:
        Enemy* host;

    protected:
        Enemy* const getHost();

    public:
        EnemyBrain();
        EnemyBrain(const EnemyBrain & proto);
        virtual ~EnemyBrain();

        virtual std::unique_ptr<EnemyBrain> clone() const;

        virtual void attach(Enemy* host);
        virtual void detach();

        virtual void handleCollision(Movable& body, CollisionInfo& info);

        virtual void update(float dt);
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_ENEMYBRAIN
