#include <hikari/core/game/CollisionInfo.hpp>

namespace hikari {
    
    CollisionInfo::CollisionInfo() 
        : isCollisionX(false)
        , isCollisionY(false)
        , worldX(0)
        , worldY(0)
        , directionX(Directions::None)
        , directionY(Directions::None)
        , tileX(0)
        , tileY(0)
        , correctedX(0)
        , correctedY(0)
        , tileType(0)
    {

    }

    void CollisionInfo::clear() {
        isCollisionX = 0;
        isCollisionY = 0;
        worldX = 0;
        worldY = 0;
        directionX = Directions::None;
        directionY = Directions::None;
        tileX = 0;
        tileY = 0;
        correctedX = 0;
        correctedY = 0;
        tileType = 0;
    }

}