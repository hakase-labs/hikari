#ifndef HIKARI_CORE_GEOM_COLLISIONINFO
#define HIKARI_CORE_GEOM_COLLISIONINFO

#include <hikari/core/Platform.hpp>
#include <hikari/core/geom/Vector2D.hpp>

namespace hikari {
namespace geom {

    class AABBCollisionInfo {
    private:
        Vector2D normal;
        Vector2D point;
        float distance;

    public:
        AABBCollisionInfo(const Vector2D& normal, const float& distance, const Vector2D& point);
        const Vector2D& getNormal() const;
        const Vector2D& getPoint() const;
        const float& getDistance() const;

        void setNormal(const Vector2D& newNormal);
        void setPoint(const Vector2D& newPoint);
        void setDistance(const float& newDistance);
    };

} // geom
} // hikari

#endif // HIKARI_CORE_GEOM_COLLISIONINFO