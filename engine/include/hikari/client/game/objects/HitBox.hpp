#ifndef HIKARI_CLIENT_GAME_OBJECT_HITBOX
#define HIKARI_CLIENT_GAME_OBJECT_HITBOX

#include "hikari/core/geom/BoundingBox.hpp"

namespace hikari {

    struct HitBox {
        BoundingBox<float> bounds;
        bool shieldFlag;

        HitBox(const BoundingBox<float> bounds = BoundingBox<float>(0, 0, 0, 0), bool isShield = false);
    };


}

#endif // HIKARI_CLIENT_GAME_OBJECT_HITBOX
