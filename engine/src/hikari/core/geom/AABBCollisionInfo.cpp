#include <hikari/core/geom/AABBCollisionInfo.hpp>

namespace hikari {
namespace geom {

    AABBCollisionInfo::AABBCollisionInfo(const Vector2D& normal, const float& distance, const Vector2D& point)
        : normal(normal)
        , distance(distance)
        , point(point)
    {

    }

    const Vector2D& AABBCollisionInfo::getNormal() const {
        return normal;
    }

    const Vector2D& AABBCollisionInfo::getPoint() const {
        return point;
    }

    const float& AABBCollisionInfo::getDistance() const {
        return distance;
    }

    void AABBCollisionInfo::setNormal(const Vector2D& newNormal) {
        normal = newNormal;
    }

    void AABBCollisionInfo::setPoint(const Vector2D& newPoint) {
        point = newPoint;
    }

    void AABBCollisionInfo::setDistance(const float& newDistance) {
        distance = newDistance;
    }

} // geom
} // hikari