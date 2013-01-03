#include <hikari/core/game/CollisionInfo.hpp>

namespace hikari {
    
    CollisionInfo::CollisionInfo() 
        : treatLadderTopAsGround(true)
        , isCollisionX(false)
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
        treatLadderTopAsGround = true;
        isCollisionX = false;
        isCollisionY = false;
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