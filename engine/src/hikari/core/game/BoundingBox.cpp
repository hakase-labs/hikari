#include <hikari/core/geom/BoundingBox.hpp>

namespace hikari {
    /*
    BoundingBox::BoundingBox(const Vector2D &position, const float &width, const float &height)
        : position(position)
        , origin(0, 0)
        , width(width)
        , height(height) {

    }
        
    BoundingBox::BoundingBox(const float &x, const float &y, const float &width, const float &height)
        : position(x, y)
        , origin(0, 0)
        , width(width)
        , height(height) {

    }
        
    const Vector2D& BoundingBox::getPosition() const {
        return position;
    }

    const Vector2D& BoundingBox::getOrigin() const {
        return origin;
    }
        
    const float& BoundingBox::getWidth() const {
        return width;
    }

    const float& BoundingBox::getHeight() const {
        return height;
    }
        
    const float BoundingBox::getTop() const {
        return position.getY() - origin.getY();
    }

    const float BoundingBox::getLeft() const {
        return position.getX() - origin.getX();
    }

    const float BoundingBox::getRight() const {
        return getLeft() + getWidth();
    }

    const float BoundingBox::getBottom() const {
        return getTop() + getHeight();
    }
        
    const Vector2D BoundingBox::getTopLeft() const {
        return Vector2D(getLeft(), getTop());
    }

    const Vector2D BoundingBox::getTopRight() const {
        return Vector2D(getRight(), getTop());
    }

    const Vector2D BoundingBox::getBottomLeft() const {
        return Vector2D(getLeft(), getBottom());
    }

    const Vector2D BoundingBox::getBottomRight() const {
        return Vector2D(getRight(), getBottom());
    }
        
    BoundingBox& BoundingBox::setPosition(const Vector2D& newPosition) {
        position = newPosition;
        return *this;
    }

    BoundingBox& BoundingBox::setPosition(const float& x, const float& y) {
        position.setX(x).setY(y);
        return *this;
    }

    BoundingBox& BoundingBox::setOrigin(const Vector2D& newOrigin) {
        origin = newOrigin;
        return *this;
    }

    BoundingBox& BoundingBox::setOrigin(const float& x, const float& y) {
        origin.setX(x).setY(y);
        return *this;
    }

    BoundingBox& BoundingBox::setWidth(const float& newWidth) {
        width = newWidth;
        return *this;
    }

    BoundingBox& BoundingBox::setHeight(const float& newHeight) {
        height = newHeight;
        return *this;
    }

    BoundingBox& BoundingBox::setTop(const float& newTop) {
        position.setY(newTop + origin.getY());
        return *this;
    }

    BoundingBox& BoundingBox::setLeft(const float& newLeft) {
        position.setX(newLeft + origin.getX());
        return *this;
    }

    BoundingBox& BoundingBox::setRight(const float& newRight) {
        setLeft(newRight - getWidth());
        return *this;
    }

    BoundingBox& BoundingBox::setBottom(const float& newBottom) {
        setTop(newBottom - getHeight());
        return *this;
    }

    bool BoundingBox::intersects(const BoundingBox& other) const {
        return !(getLeft() > other.getRight() || getRight() < other.getLeft() || 
                getTop() > other.getBottom() || getBottom() < other.getTop());
    }

    bool BoundingBox::contains(const BoundingBox& other) const {
        return (contains(other.getLeft(), other.getTop()) && 
                contains(other.getRight(), other.getTop()) && 
                contains(other.getLeft(), other.getBottom()) && 
                contains(other.getRight(), other.getBottom()));
    }

    bool BoundingBox::contains(const float& x, const float& y) const {
        return (x >= getLeft() && x <= getRight() && 
                y >= getTop() && y <= getBottom());
    }

    std::ostream &operator<<(std::ostream &stream, const BoundingBox &box) {
        stream << "[";
        stream << box.getTop();
        stream << ", ";
        stream << box.getRight();
        stream << ", ";
        stream << box.getBottom();
        stream << ", ";
        stream << box.getLeft();
        stream << "]";

        return stream;
    }
    */
}