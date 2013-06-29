#include "hikari/client/game/objects/Motion.hpp"

namespace hikari {

    Motion::Motion() {
        
    }

    Motion::Motion(const Motion & proto) {

    }

    Motion::~Motion() {

    }

    Vector2<float> Motion::calculate(float dt, const Vector2<float> & previousVelocity) {
        return previousVelocity;
    }

} // hikari