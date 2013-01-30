#ifndef HIKARI_CORE_GEOM_RECTANGLE2D
#define HIKARI_CORE_GEOM_RECTANGLE2D

#include "hikari/core/Platform.hpp"
#include <iostream>

namespace hikari {

    template <typename T>
    class HIKARI_API Rectangle2D {
    private:
        T x;
        T y;
        T width;
        T height;

    public:
        Rectangle2D();
        Rectangle2D(const Rectangle2D &rectangle);
        Rectangle2D(const T &x, const T &y, const T &width, const T &height);

        T getX() const;
        T getY() const;
        T getWidth() const;
        T getHeight() const;

        T getTop() const;
        T getRight() const;
        T getBottom() const;
        T getLeft() const;

        void setX(const T &newX);
        void setY(const T &newY);
        void setWidth(const T &newWidth);
        void setHeight(const T &newHeight);

        template <typename U> 
        bool intersects(const Rectangle2D<U>& rect) const {
            return !(getLeft() > rect.getRight() || getRight() < rect.getLeft() || 
                    getTop() > rect.getBottom() || getBottom() < rect.getTop());
        }

        template <typename U> 
        bool contains(const Rectangle2D<U>& rect) const {
            return (contains(rect.getLeft(), rect.getTop()) && 
                    contains(rect.getRight(), rect.getTop()) && 
                    contains(rect.getLeft(), rect.getBottom()) && 
                    contains(rect.getRight(), rect.getBottom()));
        }

        template <typename U> 
        bool contains(const U &x, const U &y) const {
            return (x >= getLeft() && x <= getRight() && 
                    y >= getTop() && y <= getBottom());
        }

        template <typename U> 
        friend std::ostream &operator<<(std::ostream &stream, const Rectangle2D<U> &rect);
    };

    //
    // Template class definition
    //
    template <typename T> 
    Rectangle2D<T>::Rectangle2D()
        : x(0)
        , y(0)
        , width(0)
        , height(0) {

    }

    template <typename T> 
    Rectangle2D<T>::Rectangle2D(const Rectangle2D &rectangle)
        : x(rectangle.x)
        , y(rectangle.y)
        , width(rectangle.width)
        , height(rectangle.height) {

    }

    template <typename T> 
    Rectangle2D<T>::Rectangle2D(const T &x, const T &y, const T &width, const T &height)
        : x(x)
        , y(y)
        , width(width)
        , height(height) {

    }

    template <typename T> 
    T Rectangle2D<T>::getX() const {
        return x;
    }

    template <typename T> 
    T Rectangle2D<T>::getY() const {
        return y;
    }

    template <typename T> 
    T Rectangle2D<T>::getWidth() const {
        return width;
    }

    template <typename T> 
    T Rectangle2D<T>::getHeight() const {
        return height;
    }

    template <typename T> 
    T Rectangle2D<T>::getTop() const {
        return getY();
    }

    template <typename T> 
    T Rectangle2D<T>::getRight() const {
        return getX() + getWidth();
    }

    template <typename T> 
    T Rectangle2D<T>::getBottom() const {
        return getY() + getHeight();
    }

    template <typename T> 
    T Rectangle2D<T>::getLeft() const {
        return getX();
    }

    template <typename T> 
    void Rectangle2D<T>::setX(const T &newX) {
        x = newX;
    }

    template <typename T> 
    void Rectangle2D<T>::setY(const T &newY) {
        y = newY;
    }

    template <typename T> 
    void Rectangle2D<T>::setWidth(const T &newWidth) {
        width = newWidth;
    }

    template <typename T> 
    void Rectangle2D<T>::setHeight(const T &newHeight) {
        height = newHeight;
    }

    //template <typename T> 
    //bool Rectangle2D<T>::intersects(const Rectangle2D& rect) const {
    //    return !(getLeft() > rect.getRight() || getRight() < rect.getLeft() || 
    //            getTop() > rect.getBottom() || getBottom() < rect.getTop());
    //}

    //template <typename T> 
    //bool Rectangle2D<T>::contains(const Rectangle2D& rect) const {
    //    return (contains(rect.getLeft(), rect.getTop()) && 
    //            contains(rect.getRight(), rect.getTop()) && 
    //            contains(rect.getLeft(), rect.getBottom()) && 
    //            contains(rect.getRight(), rect.getBottom()));
    //}

    template <typename U> 
    std::ostream &operator<<(std::ostream &stream, const Rectangle2D<U> &rect)
    {
        stream << "[";
        stream << rect.getX();
        stream << ", ";
        stream << rect.getY();
        stream << ", ";
        stream << rect.getWidth();
        stream << ", ";
        stream << rect.getHeight();
        stream << "]";
        return stream;
    }

} // hikari

#endif // HIKARI_CORE_GEOM_RECTANGLE2D