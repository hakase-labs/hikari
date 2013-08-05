#ifndef HIKARI_CLIENT_GAME_OBJECTS_LINEARMOTION
#define HIKARI_CLIENT_GAME_OBJECTS_LINEARMOTION

#include "hikari/client/game/objects/Motion.hpp"

namespace hikari {

    class LinearMotion : public Motion {
    private:
        float angle;
        float speed;
        Vector2<float> velocity;
        bool applyXVelocity;
        bool applyYVelocity;

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

        /**
         * Sets whether this Motion applies its horizontal velocity or not. If
         * the Motion is not applying its velocity then the input velocity is
         * returned unchanged in the horizontal direction.
         * 
         * @param applyVelocity boolean indicating whether velocity should be
         *                      applied in the horizontal direction
         */
        void setApplyHorizontalVelocity(bool applyVelocity);

        /**
         * Sets whether this Motion applies its vertical velocity or not. If
         * the Motion is not applying its velocity then the input velocity is
         * returned unchanged in the vertical direction.
         * 
         * @param applyVelocity boolean indicating whether velocity should be
         *                      applied in the vertical direction
         */
        void setApplyVerticalVelocity(bool applyVelocity);

        virtual Vector2<float> calculate(float dt, const Vector2<float> & previousVelocity = Vector2<float>());
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_LINEARMOTION
