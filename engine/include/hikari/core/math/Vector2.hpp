#ifndef HIKARI_CORE_GEOM_VECTOR2
#define HIKARI_CORE_GEOM_VECTOR2

#include <hikari/core/Platform.hpp>
#include <hikari/core/geom/Point2D.hpp>
#include <cmath>
#include <iostream>

namespace hikari {

    template<typename T>
    class HIKARI_API Vector2 {
    private:
        Point2D<T> point;

    public:
        Vector2();
        Vector2(T x, T y);
        Vector2(const Vector2<T>& copy);
        
        const T getX() const;
        const T getY() const;
        const T getLengthSquared() const;
        const T getLength() const;

        Vector2& setX(const T& newX);
        Vector2& setY(const T& newY);

        Vector2 operator-() const;

        Vector2 operator + (const Vector2& rhs) const;
        Vector2& operator += (const Vector2& rhs);

        Vector2 operator - (const Vector2& rhs) const;
        Vector2& operator -= (const Vector2& rhs);

        Vector2 operator * (const Vector2& rhs) const;
        Vector2& operator *= (const Vector2& rhs);

        Vector2 operator * (T k) const;
        Vector2& operator *= (T k);

        Vector2 operator / (const Vector2& rhs) const;
        Vector2& operator /= (const Vector2& rhs);

        Vector2 operator / (T k) const;
        Vector2& operator /= (T k);

        bool operator == (const Vector2& rhs) const;
        bool operator != (const Vector2& rhs) const;

        const T dot(const Vector2& rhs) const;

        Vector2 abs() const;
        
        friend std::ostream &operator<<(std::ostream &stream, const Vector2 &vector) {
            stream << "(";
            stream << vector.getX();
            stream << ", ";
            stream << vector.getY();
            stream << ")";
            return stream;
        }
    };

    template <typename T>
    Vector2<T>::Vector2()
        : point(0, 0) 
    {

    }

    template <typename T>
    Vector2<T>::Vector2(T x, T y)
        : point(x, y)
    {

    }

    template <typename T>
    Vector2<T>::Vector2(const Vector2<T>& copy) 
        : point(copy.point)
    {

    }
        
    template <typename T>
    const T Vector2<T>::getX() const {
        return point.getX();
    }

    template <typename T>
    const T Vector2<T>::getY() const {
        return point.getY();
    }

    template <typename T>
    const T Vector2<T>::getLengthSquared() const {
        return dot(*this);
    }

    template <typename T>
    const T Vector2<T>::getLength() const {
        return std::sqrt(getLengthSquared());
    }

    template <typename T>
    Vector2<T>& Vector2<T>::setX(const T& newX) {
        point.setX(newX);
        return *this;
    }

    template <typename T>
    Vector2<T>& Vector2<T>::setY(const T& newY) {
        point.setY(newY);
        return *this;
    }

    template <typename T>
    Vector2<T> Vector2<T>::operator-() const {
        return Vector2<T>(-getX(), -getY());
    }

    template <typename T>
    Vector2<T> Vector2<T>::operator + (const Vector2& rhs) const {
        Vector2<T> result(*this);

        result += rhs;

        return result;
    }

    template <typename T>
    Vector2<T>& Vector2<T>::operator += (const Vector2& rhs) {
        setX(getX() + rhs.getX());
        setY(getY() + rhs.getY());
        return *this;
    }

    template <typename T>
    Vector2<T> Vector2<T>::operator - (const Vector2& rhs) const {
        Vector2<T> result(*this);

        result -= rhs;

        return result;
    }

    template <typename T>
    Vector2<T>& Vector2<T>::operator -= (const Vector2& rhs) {
        setX(getX() - rhs.getX());
        setY(getY() - rhs.getY());
        return *this;
    }

    template <typename T>
    Vector2<T> Vector2<T>::operator * (const Vector2& rhs) const {
        Vector2<T> result(*this);

        result *= rhs;

        return result;
    }

    template <typename T>
    Vector2<T>& Vector2<T>::operator *= (const Vector2& rhs) {
        setX(getX() * rhs.getX());
        setY(getY() * rhs.getY());
        return *this;
    }

    template <typename T>
    Vector2<T> Vector2<T>::operator * (T k) const {
        Vector2<T> result(*this);

        result *= k;

        return result;
    }

    template <typename T>
    Vector2<T>& Vector2<T>::operator *= (T k) {
        setX(getX() * k);
        setY(getY() * k);
        return *this;
    }

    template <typename T>
    Vector2<T> Vector2<T>::operator / (const Vector2& rhs) const {
        Vector2<T> result(*this);

        result /= rhs;

        return result;
    }

    template <typename T>
    Vector2<T>& Vector2<T>::operator /= (const Vector2& rhs) {
        setX(getX() / rhs.getX());
        setY(getY() / rhs.getY());
        return *this;
    }


    template <typename T>
    Vector2<T> Vector2<T>::operator / (T k) const {
        Vector2<T> result(*this);

        result /= k;

        return result;
    }

    template <typename T>
    Vector2<T>& Vector2<T>::operator /= (T k) {
        setX(getX() / k);
        setY(getY() / k);
        return *this;
    }

    template <typename T>
    bool Vector2<T>::operator == (const Vector2& rhs) const {
        return ((getX() == rhs.getX()) && getY() == rhs.getY());
    }

    template <typename T>
    bool Vector2<T>::operator != (const Vector2& rhs) const {
        return !(*this == rhs);
    }

    template <typename T>
    const T Vector2<T>::dot(const Vector2& rhs) const {
        return getX() * rhs.getX() + getY() * rhs.getY();
    }

    template <typename T>
    Vector2<T> Vector2<T>::abs() const {
        return Vector2(
            std::abs(getX()),
            std::abs(getY())
        );
    }

} // hikari

#endif // HIKARI_CORE_GEOM_VECTOR2