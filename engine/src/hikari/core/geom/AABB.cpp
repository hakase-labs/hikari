#include <hikari/core/geom/AABB.hpp>
#include <hikari/core/geom/Vector2D.hpp>

namespace hikari {

    AABB::~AABB() {

    }

    bool AABB::overlap(const AABB& left, const AABB& right)    {
        Vector2D centerDelta = (right.getCenter() - left.getCenter()).abs();
        Vector2D halfExtentsSum = left.getHalfExtents() + right.getHalfExtents();

        centerDelta -= halfExtentsSum;

        return centerDelta.getX() < 0 && centerDelta.getY() < 0;
    }

}