#ifndef HIKARI_CLIENT_GAME_OBJECTS_ENEMYBRAIN
#define HIKARI_CLIENT_GAME_OBJECTS_ENEMYBRAIN

#include "hikari/client/game/objects/Enemy.hpp"

/* begin instance-level stuff */
#include "hikari/core/util/Timer.hpp"
/* end instance-level stuff */

namespace hikari {

    class Movable;
    class CollisionInfo;

    class EnemyBrain {
    private:
        Enemy* host;

    protected:
        Enemy* const getHost();

    public:
        EnemyBrain();
        virtual ~EnemyBrain();

        virtual void attach(Enemy* host);
        virtual void detach();

        virtual void handleCollision(Movable& body, CollisionInfo& info);

        virtual void update(float dt);
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_ENEMYBRAIN
