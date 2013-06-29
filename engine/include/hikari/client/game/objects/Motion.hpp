#ifndef HIKARI_CLIENT_GAME_OBJECTS_MOTION
#define HIKARI_CLIENT_GAME_OBJECTS_MOTION

#include "hikari/core/math/Vector2.hpp"

namespace hikari {

    class Motion {
    public:
        Motion();
        Motion(const Motion & proto);
        virtual ~Motion();

        virtual Vector2<float> calculate(float dt, const Vector2<float> & previousVelocity = Vector2<float>());
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_MOTION
