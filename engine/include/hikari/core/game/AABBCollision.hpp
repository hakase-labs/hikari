#ifndef HIKARI_CORE_GAME_AABBCOLLISION
#define HIKARI_CORE_GAME_AABBCOLLISION

namespace hikari {

namespace geom {
    class AABBCollisionInfo;
}

    class AABB;

    class Room;
    class Vector2D;

    class AABBCollision {
    private:
        static bool isInternalCollision(const int& tileX, const int& tileY, const Vector2D& normal, const Room& room);

    public:
        static bool aabbVsAabbInternal(const Vector2D& delta, const Vector2D& aabbCenter, const Vector2D& aabbHalfExtents, const Vector2D& point, geom::AABBCollisionInfo& collisionInfo);
        static bool aabbVsAabb(const AABB& boxA, const AABB& boxB, geom::AABBCollisionInfo& collisionInfo, const int& tileX, const int& tileY, const Room& room, const bool checkInternal = true);
        static bool pointInAabb(const Vector2D& point, const AABB& box);
        static bool aabbVsAabbTopPlane(const AABB& boxA, const AABB& boxB, geom::AABBCollisionInfo& collisionInfo, const int& tileX, const int& tileY, const Room& room);
    };

}

#endif // HIKARI_CORE_GAME_AABBCOLLISION