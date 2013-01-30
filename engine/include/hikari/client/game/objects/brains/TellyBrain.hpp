#ifndef HIKARI_CLIENT_GAME_OBJECTS_BRAINS_TELLYBRAIN
#define HIKARI_CLIENT_GAME_OBJECTS_BRAINS_TELLYBRAIN

#include "hikari/client/game/objects/EnemyBrain.hpp"
#include "hikari/core/math/Vector2.hpp"

namespace hikari {

    class GameWorld;

    class TellyBrain : public EnemyBrain {
    private:
        const GameWorld * world;
        Timer timer;
        Vector2<float> targetPosition;

        void adjustCourse();

    public:
        TellyBrain(const GameWorld * const world);
        virtual ~TellyBrain();

        virtual void handleCollision(Movable& body, CollisionInfo& info);

        virtual void update(const float& dt);
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_BRAINS_TELLYBRAIN