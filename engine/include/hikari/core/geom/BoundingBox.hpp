#ifndef HIKARI_CORE_GEOM_BOUNDINGBOX_T
#define HIKARI_CORE_GEOM_BOUNDINGBOX_T

#include "hikari/core/Platform.hpp"
#include "hikari/core/math/Vector2.hpp"
#include <iostream>
#include <utility>

namespace hikari {

    /**
        Represents the location and size of a rectangle used as a bounding box.

        This class provides methods for accessing the coordinates for the edge
        and corner coordinates.

        When possible this class' methods implement a fluent interface and
        allow chaining method calls together.
    */
    template <typename T>
    class HIKARI_API BoundingBox {
    private:
        Vector2<T> position;
        Vector2<T> origin;
        T width;
        T height;

    public:
        BoundingBox(const Vector2<T> &position, const T &width, const T &height);
        BoundingBox(const T &x, const T &y, const T &width, const T &height);
        BoundingBox(const BoundingBox& proto);

        BoundingBox & operator = (BoundingBox other);
        bool operator == (const BoundingBox& rhs) const;
        bool operator != (const BoundingBox& rhs) const;

        void swap(BoundingBox & other);

        //
        // Accessors
        //
        const Vector2<T>& getPosition() const;
        const Vector2<T>& getOrigin() const;

        const T& getWidth() const;
        const T& getHeight() const;

        /**
            Gets the y-coordinate that is the top edge of this BoundingBox.
        */
        const T getTop() const;

        /**
            Gets the x-coordinate that is the left edge of this BoundingBox.
        */
        const T getLeft() const;

        /**
            Gets the x-coordinate that is the sum of the x and width property
            values of this BoundingBox.

            The value of the right edge represents the x-coordinate of the
            first point at the right edge of the rectangle that is not
            contained in the rectangle.

            @see getLeft
            @see getWidth
        */
        const T getRight() const;

        /**
            Gets the y-coordinate that is the sum of the y and height property
            values of this BoundingBox.

            The value of the bottom edge represents the y-coordinate of the
            first point at the bottom edge of the rectangle that is not
            contained in the rectangle.

            @see getTop
            @see getHeight
        */
        const T getBottom() const;

        const Vector2<T> getTopLeft() const;
        const Vector2<T> getTopRight() const;
        const Vector2<T> getBottomLeft() const;
        const Vector2<T> getBottomRight() const;

        //
        // Mutators
        //
        BoundingBox& setPosition(const Vector2<T>& newPosition);
        BoundingBox& setPosition(const T& x, const T& y);
        BoundingBox& setOrigin(const Vector2<T>& newOrigin);
        BoundingBox& setOrigin(const T& x, const T& y);

        BoundingBox& setWidth(const T& newWidth);
        BoundingBox& setHeight(const T& newHeight);
        BoundingBox& setSize(const T& newWidth, const T& newHeight);

        /**
            Adjusts the position of the BoundingBox such that its top edge
            is equal to the specified value.

            @see setPosition
        */
        BoundingBox& setTop(const T& newTop);

        /**
            Adjusts the position of the BoundingBox such that its left edge
            is equal to the specified value.

            @see setPosition
        */
        BoundingBox& setLeft(const T& newLeft);

        /**
            Adjusts the position of the BoundingBox such that its right edge
            is equal to the specified value.

            @see setPosition
        */
        BoundingBox& setRight(const T& newRight);

        /**
            Adjusts the position of the BoundingBox such that its bottom edge
            is equal to the specified value.

            @see setPosition
        */
        BoundingBox& setBottom(const T& newBottom);


        /**
            Tests if two BoundingBox objects' areas intersect.

            @return true if this BoundingBox's area intersects the argument.
        */
        bool intersects(const BoundingBox& other) const;

        /**
            Tests if the specified BoundingBox is fully contained within this
            BoundingBox's area.
        */
        bool contains(const BoundingBox& other) const;

        /**
            Tests if a specified coordinate is inside this BoundingBox's area.
        */
        bool contains(const T& x, const T& y) const;

        /**
         * Determines if this BoundingBox is completely to the left of another one.
         * @param  other the BoundingBox to check against
         * @return       true if this BoundingBox is to the left of other, otherwise false
         */
        bool isLeftOf(const BoundingBox& other) const;

        /**
         * Determines if this BoundingBox is completely to the right of another one.
         * @param  other the BoundingBox to check against
         * @return       true if this BoundingBox is to the right of other, otherwise false
         */
        bool isRightOf(const BoundingBox& other) const;

        /**
         * Determines if this BoundingBox is completely above another one.
         * @param  other the BoundingBox to check against
         * @return       true if this BoundingBox is above other, otherwise false
         */
        bool isAbove(const BoundingBox& other) const;

        /**
         * Determines if this BoundingBox is completely below another one.
         * @param  other the BoundingBox to check against
         * @return       true if this BoundingBox is below other, otherwise false
         */
        bool isBelow(const BoundingBox& other) const;

        //
        // Utilities
        //
        friend std::ostream &operator<<(std::ostream &stream, const BoundingBox &box) {
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
    };


    template <typename T>
    BoundingBox<T>::BoundingBox(const Vector2<T> &position, const T &width, const T &height)
        : position(position)
        , origin(0, 0)
        , width(width)
        , height(height) {

    }

    template <typename T>
    BoundingBox<T>::BoundingBox(const T &x, const T &y, const T &width, const T &height)
        : position(x, y)
        , origin(0, 0)
        , width(width)
        , height(height) {

    }

    template <typename T>
    BoundingBox<T>::BoundingBox(const BoundingBox& proto)
        : position(proto.position)
        , origin(proto.origin)
        , width(proto.width)
        , height(proto.height) {

    }

    template <typename T>
    BoundingBox<T> & BoundingBox<T>::operator = (BoundingBox<T> other) {
        swap(other);
        return *this;
    }

    template <typename T>
    bool BoundingBox<T>::operator == (const BoundingBox<T>& rhs) const {
        return (position == rhs.position &&
            origin == rhs.origin &&
            width == rhs.width &&
            height == rhs.height);
    }

    template <typename T>
    bool BoundingBox<T>::operator != (const BoundingBox<T>& rhs) const {
        return !(*this == rhs);
    }

    template <typename T>
    void BoundingBox<T>::swap(BoundingBox<T> & other) {
        std::swap(position, other.position);
        std::swap(origin, other.origin);
        std::swap(width, other.width);
        std::swap(height, other.height);
    }

    template <typename T>
    const Vector2<T>& BoundingBox<T>::getPosition() const {
        return position;
    }

    template <typename T>
    const Vector2<T>& BoundingBox<T>::getOrigin() const {
        return origin;
    }
    template <typename T>
    const T& BoundingBox<T>::getWidth() const {
        return width;
    }
    template <typename T>
    const T& BoundingBox<T>::getHeight() const {
        return height;
    }
    template <typename T>
    const T BoundingBox<T>::getTop() const {
        return position.getY() - origin.getY();
    }
    template <typename T>
    const T BoundingBox<T>::getLeft() const {
        return position.getX() - origin.getX();
    }
    template <typename T>
    const T BoundingBox<T>::getRight() const {
        return getLeft() + getWidth();
    }
    template <typename T>
    const T BoundingBox<T>::getBottom() const {
        return getTop() + getHeight();
    }
    template <typename T>
    const Vector2<T> BoundingBox<T>::getTopLeft() const {
        return Vector2<T>(getLeft(), getTop());
    }

    template <typename T>
    const Vector2<T> BoundingBox<T>::getTopRight() const {
        return Vector2<T>(getRight(), getTop());
    }

    template <typename T>
    const Vector2<T> BoundingBox<T>::getBottomLeft() const {
        return Vector2<T>(getLeft(), getBottom());
    }

    template <typename T>
    const Vector2<T> BoundingBox<T>::getBottomRight() const {
        return Vector2<T>(getRight(), getBottom());
    }

    template <typename T>
    BoundingBox<T>& BoundingBox<T>::setPosition(const Vector2<T>& newPosition) {
        position = newPosition;
        return *this;
    }

    template <typename T>
    BoundingBox<T>& BoundingBox<T>::setPosition(const T& x, const T& y) {
        position.setX(x).setY(y);
        return *this;
    }

    template <typename T>
    BoundingBox<T>& BoundingBox<T>::setOrigin(const Vector2<T>& newOrigin) {
        origin = newOrigin;
        return *this;
    }

    template <typename T>
    BoundingBox<T>& BoundingBox<T>::setOrigin(const T& x, const T& y) {
        origin.setX(x).setY(y);
        return *this;
    }

    template <typename T>
    BoundingBox<T>& BoundingBox<T>::setWidth(const T& newWidth) {
        width = newWidth;
        return *this;
    }

    template <typename T>
    BoundingBox<T>& BoundingBox<T>::setHeight(const T& newHeight) {
        height = newHeight;
        return *this;
    }

    template <typename T>
    BoundingBox<T>& BoundingBox<T>::setSize(const T& newWidth, const T& newHeight) {
        return setWidth(newWidth).setHeight(newHeight);
    }

    template <typename T>
    BoundingBox<T>& BoundingBox<T>::setTop(const T& newTop) {
        position.setY(newTop + origin.getY());
        return *this;
    }

    template <typename T>
    BoundingBox<T>& BoundingBox<T>::setLeft(const T& newLeft) {
        position.setX(newLeft + origin.getX());
        return *this;
    }

    template <typename T>
    BoundingBox<T>& BoundingBox<T>::setRight(const T& newRight) {
        setLeft(newRight - getWidth());
        return *this;
    }

    template <typename T>
    BoundingBox<T>& BoundingBox<T>::setBottom(const T& newBottom) {
        setTop(newBottom - getHeight());
        return *this;
    }

    template <typename T>
    bool BoundingBox<T>::intersects(const BoundingBox<T>& other) const {
        return !(getLeft() > other.getRight() || getRight() < other.getLeft() ||
                getTop() > other.getBottom() || getBottom() < other.getTop());
    }

    template <typename T>
    bool BoundingBox<T>::contains(const BoundingBox<T>& other) const {
        return (contains(other.getLeft(), other.getTop()) &&
                contains(other.getRight(), other.getTop()) &&
                contains(other.getLeft(), other.getBottom()) &&
                contains(other.getRight(), other.getBottom()));
    }

    template <typename T>
    bool BoundingBox<T>::contains(const T& x, const T& y) const {
        return (x >= getLeft() && x <= getRight() &&
                y >= getTop() && y <= getBottom());
    }

    template <typename T>
    bool BoundingBox<T>::isLeftOf(const BoundingBox<T>& other) const {
        return getRight() <= other.getLeft();
    }

    template <typename T>
	bool BoundingBox<T>::isRightOf(const BoundingBox<T>& other) const {
        return getLeft() >= other.getRight();
    }

    template <typename T>
	bool BoundingBox<T>::isAbove(const BoundingBox<T>& other) const {
        return getBottom() <= other.getTop();
    }

    template <typename T>
	bool BoundingBox<T>::isBelow(const BoundingBox<T>& other) const {
        return getTop() >= other.getBottom();
    }

    typedef BoundingBox<float> BoundingBoxF;
    typedef BoundingBox<int> BoundingBoxI;
}

#endif // HIKARI_CORE_GEOM_BOUNDINGBOX_T
