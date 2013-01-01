#include <hikari/core/geom/RectangleAABB.hpp>

namespace hikari {

    RectangleAABB::RectangleAABB(const Vector2D& center, const Vector2D& halfExtents)
        : center(center)
        , halfExtents(halfExtents)
    {

    }

    RectangleAABB::~RectangleAABB() {

    }

    const Vector2D& RectangleAABB::getCenter() const {
        return center;
    }

    const Vector2D& RectangleAABB::getHalfExtents() const {
        return halfExtents;
    }
        
    void RectangleAABB::setCenter(const Vector2D& newCenter) {
        center = newCenter;
    }

    void RectangleAABB::setHalfExtents(const Vector2D& newHalfExtents) {
        halfExtents = newHalfExtents;
    }

    Vector2D RectangleAABB::getBottomLeft() {
        return center + Vector2D(-getHalfExtents().getX(), getHalfExtents().getY());
    }

    Vector2D RectangleAABB::getBottomRight() {
        return center + Vector2D(getHalfExtents().getX(), getHalfExtents().getY());
    }

    Vector2D RectangleAABB::getTopLeft() {
        return center + Vector2D(-getHalfExtents().getX(), -getHalfExtents().getY());
    }

    Vector2D RectangleAABB::getTopRight() {
        return center + Vector2D(getHalfExtents().getX(), -getHalfExtents().getY());
    }
        
    Vector2D RectangleAABB::getMin() {
        return getTopLeft();
    }

    Vector2D RectangleAABB::getMax() {
        return getBottomRight();
    }

} // hikari