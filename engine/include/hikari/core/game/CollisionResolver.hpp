#ifndef HIKARI_CORE_GAME_COLLISIONRESOLVER
#define HIKARI_CORE_GAME_COLLISIONRESOLVER

#include <hikari/core/game/Direction.hpp>

namespace hikari {
    
    class CollisionInfo;

    /**
        Interface for classes that can check for and resolve 
        object-to-world collisions.
    */
    class CollisionResolver {
    public:
        virtual ~CollisionResolver() {}
        virtual void checkHorizontalEdge(const int& x, const int& yMin, const int& yMax, const Direction& directionX, CollisionInfo& collisionInfo) = 0;
        virtual void checkVerticalEdge(const int& y, const int& xMin, const int& xMax, const Direction& directionY, CollisionInfo& collisionInfo) = 0;
    };

} // hikari

#endif // HIKARI_CORE_GAME_COLLISIONRESOLVER