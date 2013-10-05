﻿#include "hikari/core/game/map/RoomTransition.hpp"

namespace hikari {

    RoomTransition::RoomTransition(int from, int to, int width, int height, int x, int y, Direction dir, bool doorFlag)
        : doorFlag(doorFlag)
        , fromRegion(from)
        , toRegion(to)
        , bounds(x, y, width, height)
        , direction(dir) {
    
    }

    bool RoomTransition::isDoor() const {
        return doorFlag;
    }
    
    int RoomTransition::getFromRegion() const {
        return fromRegion;
    }
    
    int RoomTransition::getToRegion() const {
        return toRegion;
    }
    
    int RoomTransition::getWidth() const {
        return bounds.getWidth();
    }
    
    int RoomTransition::getHeight() const {
        return bounds.getHeight();
    }
    
    int RoomTransition::getX() const {
        return bounds.getX();
    }
    
    int RoomTransition::getY() const {
        return bounds.getY();
    }
    
    RoomTransition::Direction RoomTransition::getDirection() const {
        return direction;
    }

}