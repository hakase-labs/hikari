#ifndef HIKARI_CLIENT_GAME_OBJECTS_LINEARMOTION
#define HIKARI_CLIENT_GAME_OBJECTS_LINEARMOTION

#include "hikari/client/game/objects/Motion.hpp"

namespace hikari {

    class LinearMotion : public Motion {
    private:
        float angle;
        float speed;
        Vector2<float> velocity;

        void calculateAngle();
        void calculateSpeed();
        void calculateVelocity();

    public:
        LinearMotion();
        LinearMotion(float angle, float speed);
        explicit LinearMotion(const Vector2<float> & velocity);
        LinearMotion(const LinearMotion & proto);
        virtual ~LinearMotion();

        void setAngle(float degrees);
        void setSpeed(float speed);
        void setVelocity(const Vector2<float> & velocity);

        virtual Vector2<float> calculate(float dt, const Vector2<float> & previousVelocity = Vector2<float>());
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_LINEARMOTION
