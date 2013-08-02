#ifndef HIKARI_CLIENT_GAME_MAP_TILEMAPCOLLISIONRESOLVER
#define HIKARI_CLIENT_GAME_MAP_TILEMAPCOLLISIONRESOLVER

#include "hikari/core/game/CollisionResolver.hpp"
#include "hikari/core/geom/BoundingBox.hpp"
#include <memory>

namespace hikari {

    class Room;

    class TileMapCollisionResolver : public CollisionResolver {
    private:
        std::weak_ptr<Room> map;
        BoundingBox<int> tileBounds;

        void sweepHorizontalEdge(const int& x, const int& yMin, const int& yMax, const Direction& directionX, CollisionInfo& collisionInfo);
        void sweepVerticalEdge(const int& y, const int& xMin, const int& xMax, const Direction& directionY, CollisionInfo& collisionInfo);
        void determineCorrection(const Direction& direction, CollisionInfo& collisionInfo);

        bool tileIsSolid(const int& tileAttribute) const;
        bool tileIsPlatform(const int& tileAttribute) const;

    public:
        TileMapCollisionResolver();
        virtual ~TileMapCollisionResolver();
        virtual void checkHorizontalEdge(const int& x, const int& yMin, const int& yMax, const Direction& directionX, CollisionInfo& collisionInfo);
        virtual void checkVerticalEdge(const int& y, const int& xMin, const int& xMax, const Direction& directionY, CollisionInfo& collisionInfo);

        void setRoom(std::weak_ptr<Room> newRoom);
        std::weak_ptr<Room> getRoom() const;
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_MAP_TILEMAPCOLLISIONRESOLVER