#include "hikari/core/game/map/Force.hpp"

namespace hikari {

    Force::Force(const BoundingBox<float> & bounds, const Vector2<float> & velocity)
        : bounds(bounds)
        , velocity(velocity)
        , activeFlag(true)
    {

    }

    void Force::setVelocity(const Vector2<float> & vel) {
        velocity = vel;
    }

    const BoundingBox<float> & Force::getBounds() const {
        return bounds;
    }

    const Vector2<float> & Force::getVelocity() const {
        return velocity;
    }

    bool Force::isActive() const {
        return activeFlag;
    }

    void Force::enable() {
        activeFlag = true;
    }

    void Force::disable() {
        activeFlag = false;
    }

} // hikari
