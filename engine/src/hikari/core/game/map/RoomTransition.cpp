#include <hikari/core/game/map/RoomTransition.hpp>

namespace hikari {

    RoomTransition::RoomTransition(int from, int to, int width, int height, int x, int y, Direction dir, bool bossEntrance)
        : bossEntrance(bossEntrance)
        , fromRegion(from)
        , toRegion(to)
        , width(width)
        , height(height)
        , x(x)
        , y(y)
        , direction(dir) {
    
    }

    bool RoomTransition::isBossEntrance() const {
        return bossEntrance;
    }
    
    int RoomTransition::getFromRegion() const {
        return fromRegion;
    }
    
    int RoomTransition::getToRegion() const {
        return toRegion;
    }
    
    int RoomTransition::getWidth() const {
        return width;
    }
    
    int RoomTransition::getHeight() const {
        return height;
    }
    
    int RoomTransition::getX() const {
        return x;
    }
    
    int RoomTransition::getY() const {
        return y;
    }
    
    RoomTransition::Direction RoomTransition::getDirection() const {
        return direction;
    }

}