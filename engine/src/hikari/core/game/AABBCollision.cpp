#include <hikari/core/game/AABBCollision.hpp>
#include <hikari/core/geom/AABB.hpp>
#include <hikari/core/geom/AABBCollisionInfo.hpp>
#include <hikari/core/geom/Vector2D.hpp>
#include <hikari/core/game/map/Room.hpp>
#include <hikari/core/game/map/Tileset.hpp>
#include <hikari/core/util/Log.hpp>
#include <cmath>

namespace hikari {

    bool AABBCollision::isInternalCollision(const int& tileX, const int& tileY, const Vector2D& normal, const Room& room) {
        int nextTileX = tileX + static_cast<int>(normal.getX());
        int nextTileY = tileY + static_cast<int>(normal.getY());
        //int currentTile = room.getAttributeAt(tileX, tileY);
        int nextTile = room.getAttributeAt(nextTileX, nextTileY);

        // TODO: Shouldn't I check if the current tile is NOT solid too?
        bool isInternalEdge = (nextTile > 0) && ((nextTile & TileAttribute::SOLID) == TileAttribute::SOLID);

        return isInternalEdge;
    }

    bool AABBCollision::aabbVsAabbInternal(const Vector2D& delta, const Vector2D& aabbCenter, const Vector2D& aabbHalfExtents, const Vector2D& point, geom::AABBCollisionInfo& collisionInfo) {
        Vector2D planeNegative = -(delta.getMajorAxis());
        Vector2D planeCenter = (planeNegative * aabbHalfExtents) + aabbCenter;

        Vector2D planeDelta = point - planeCenter;
        float distance = planeDelta.dot(planeNegative);

        collisionInfo.setNormal(planeNegative);
        collisionInfo.setDistance(distance);
        collisionInfo.setPoint(point);

        return true;
    }

    bool AABBCollision::aabbVsAabb(const AABB& boxA, const AABB& boxB, geom::AABBCollisionInfo& collisionInfo, const int& tileX, const int& tileY, const Room& room, const bool checkInternal) {
        Vector2D combinedExtents = boxA.getHalfExtents() + boxB.getHalfExtents();
        Vector2D combinedPosition = boxB.getCenter();
        Vector2D delta = combinedPosition - boxA.getCenter();

        //HIKARI_LOG(info) << "AABBCollision::aabbVsAabb; boxA = " << boxA.getCenter() << ", boxB = " << boxB.getCenter();

        aabbVsAabbInternal(delta, combinedPosition, combinedExtents, boxA.getCenter(), collisionInfo);

        if(checkInternal) {
            return !isInternalCollision(tileX, tileY, collisionInfo.getNormal(), room);
        }

        return true;
    }

    bool AABBCollision::pointInAabb(const Vector2D& point, const AABB& box) {
        Vector2D delta = point - box.getCenter();

        return std::abs(delta.getX()) < box.getHalfExtents().getX() &&
                std::abs(delta.getY()) < box.getHalfExtents().getY();
    }

    bool AABBCollision::aabbVsAabbTopPlane(const AABB& boxA, const AABB& boxB, geom::AABBCollisionInfo& collisionInfo, const int& tileX, const int& tileY, const Room& room) {
        return false;
    }

} // hikari