#ifndef HIKARI_CORE_GAME_MAP_ROOMTRANSITION
#define HIKARI_CORE_GAME_MAP_ROOMTRANSITION

#include "hikari/core/Platform.hpp"

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
        
        RoomTransition(int from, int to, int width, int height, int x, int y, Direction dir, bool bossEntrance);
        
        bool isBossEntrance() const;
        int getFromRegion() const;
        int getToRegion() const;
        int getWidth() const;
        int getHeight() const;
        int getX() const;
        int getY() const;
        Direction getDirection() const;
    
    private:
        bool bossEntrance;
        int fromRegion;
        int toRegion;
        int width;
        int height;
        int x;
        int y;
        Direction direction;
    };
    
} // hikari

#endif // HIKARI_CORE_GAME_MAP_ROOMTRANSITION