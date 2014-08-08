#ifndef HIKARI_CLIENT_GAME_MAP_WORLDCOLLISIONRESOLVER
#define HIKARI_CLIENT_GAME_MAP_WORLDCOLLISIONRESOLVER

#include "hikari/core/game/CollisionResolver.hpp"
#include "hikari/core/geom/BoundingBox.hpp"
#include <memory>

namespace hikari {

    class GameWorld;

    /**
     * A collision resolver which takes care of resolving both tile and obstacle
     * collisions within a GameWorld. The GameWorld's current room is used to
     * determine if collision are taking place between other things.
     */
    class WorldCollisionResolver : public CollisionResolver {
    private:
        GameWorld * world; // Non-owning pointer
        BoundingBox<int> tileBounds;

        void sweepHorizontalEdge(const int& x, const int& yMin, const int& yMax, const Direction& directionX, CollisionInfo& collisionInfo);
        void sweepVerticalEdge(const int& y, const int& xMin, const int& xMax, const Direction& directionY, CollisionInfo& collisionInfo);
        void determineTileCorrection(const Direction& direction, CollisionInfo& collisionInfo);

        bool tileIsSolid(const int& tileAttribute) const;
        bool tileIsPlatform(const int& tileAttribute) const;

        void updateTileBounds(int tileX, int tileY);

    public:
        WorldCollisionResolver();
        virtual ~WorldCollisionResolver();
        virtual void checkHorizontalEdge(const int& x, const int& yMin, const int& yMax, const Direction& directionX, CollisionInfo& collisionInfo);
        virtual void checkVerticalEdge(const int& y, const int& xMin, const int& xMax, const Direction& directionY, CollisionInfo& collisionInfo);

        void setWorld(GameWorld * newWorld);
        GameWorld * getWorld() const;
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_MAP_WORLDCOLLISIONRESOLVER
