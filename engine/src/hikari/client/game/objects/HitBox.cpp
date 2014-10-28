#include "hikari/client/game/objects/HitBox.hpp"

namespace hikari {

    HitBox::HitBox(const BoundingBox<float> bounds, bool isShield)
      : bounds(bounds)
      , shieldFlag(isShield)
    {

    }

}
