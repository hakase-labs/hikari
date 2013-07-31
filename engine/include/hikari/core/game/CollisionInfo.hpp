#ifndef HIKARI_CORE_GAME_COLLISIONINFO
#define HIKARI_CORE_GAME_COLLISIONINFO

#include "hikari/core/game/Direction.hpp"

namespace hikari {

    class CollisionInfo {
    public:
        bool treatPlatformAsGround; 
        bool isCollisionX;
        bool isCollisionY;
        int worldX;
        int worldY;
        Direction directionX;
        Direction directionY;
        int tileX;
        int tileY;
        int correctedX;
        int correctedY;
        int tileType;

        CollisionInfo();

        /**
            Restores all properties to their default values.
        */
        void clear();
    };

}

#endif // HIKARI_CORE_GAME_COLLISIONINFO