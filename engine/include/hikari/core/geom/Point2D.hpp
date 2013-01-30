#ifndef HIKARI_CORE_GEOM_POINT2D
#define HIKARI_CORE_GEOM_POINT2D

#include "hikari/core/Platform.hpp"
#include <iostream>

namespace hikari {

    template <typename T>
    class Point2D {
    private:
        T x;
        T y;
    
    public:
        Point2D();
        Point2D(const Point2D &point);
        Point2D(const T &x, const T &y);
    
        T getX() const;
        T getY() const;

        void setX(const T &x);
        void setY(const T &y);

        friend std::ostream &operator<<(std::ostream &stream, const Point2D<T> &point) {
            stream << "(";
            stream << point.getX();
            stream << ", ";
            stream << point.getY();
            stream << ")";
            return stream;
        }
    };

    //
    // Template class definition
    //
    template <typename T>
    Point2D<T>::Point2D()
        : x(0)
        , y(0) {

    }

    template <typename T>
    Point2D<T>::Point2D(const Point2D &point)
        : x(point.x)
        , y(point.y) {

    }

    template <typename T> 
    Point2D<T>::Point2D(const T &x, const T &y)
        : x(x)
        , y(y) {

    }

    template <typename T> 
    T Point2D<T>::getX() const {
        return x;
    }

    template <typename T> 
    T Point2D<T>::getY() const {
        return y;
    }

    template <typename T> 
    void Point2D<T>::setX(const T &newX) {
        x = newX;
    }

    template <typename T> 
    void Point2D<T>::setY(const T &newY) {
        y = newY;
    }

}

#endif // HIKARI_CORE_GEOM_POINT2D