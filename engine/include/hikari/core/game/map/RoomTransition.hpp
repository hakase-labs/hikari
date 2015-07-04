#ifndef HIKARI_CORE_GAME_MAP_ROOMTRANSITION
#define HIKARI_CORE_GAME_MAP_ROOMTRANSITION

#include "hikari/core/Platform.hpp"
#include "hikari/core/geom/Rectangle2D.hpp"

namespace hikari {
    
    class HIKARI_API RoomTransition {
    public:
        enum Direction {
            DirectionUp = 0,
            DirectionForward = 1,
            DirectionDown = 2,
            DirectionBackward = 3,
            DirectionTeleport = 4
        };

        RoomTransition(int from, int to, int width, int height, int x, int y, Direction dir, bool doorFlag, bool ladderOnly);
        
        bool isDoor() const;
        bool isLadderOnly() const;
        int getFromRegion() const;
        int getToRegion() const;
        int getWidth() const;
        int getHeight() const;
        int getX() const;
        int getY() const;
        Direction getDirection() const;
    
    private:
        bool doorFlag;
        bool ladderOnly;
        int fromRegion;
        int toRegion;
        Rectangle2D<int> bounds;
        Direction direction;
    };
    
} // hikari

#endif // HIKARI_CORE_GAME_MAP_ROOMTRANSITION