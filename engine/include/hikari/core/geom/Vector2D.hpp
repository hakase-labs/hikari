#ifndef HIKARI_CORE_GEOM_VECTOR2D
#define HIKARI_CORE_GEOM_VECTOR2D

#include "hikari/core/Platform.hpp"
#include "hikari/core/geom/Point2D.hpp"
#include "hikari/core/math/RetroVector.hpp"
#include <iostream>

namespace hikari {

    class HIKARI_API Vector2D {
    private:
        static const float EPSILON;
        static const float TWO_PI;
        Point2D<float> point;

    public:
        Vector2D();
        Vector2D(float x, float y);
        Vector2D(const Vector2D& copy);

        const float getX() const;
        const float getY() const;
        const float getLengthSquared() const;
        const float getLength() const;

        Vector2D& setX(const float& newX);
        Vector2D& setY(const float& newY);

        Vector2D operator-() const;

        Vector2D operator + (const Vector2D& rhs) const;
        Vector2D& operator += (const Vector2D& rhs);

        Vector2D operator - (const Vector2D& rhs) const;
        Vector2D& operator -= (const Vector2D& rhs);

        Vector2D operator * (const Vector2D& rhs) const;
        Vector2D& operator *= (const Vector2D& rhs);

        Vector2D operator * (float k) const;
        Vector2D& operator *= (float k);

        Vector2D operator / (const Vector2D& rhs) const;
        Vector2D& operator /= (const Vector2D& rhs);

        Vector2D operator / (float k) const;
        Vector2D& operator /= (float k);

        bool operator == (const Vector2D& rhs) const;
        bool operator != (const Vector2D& rhs) const;

        const float dot(const Vector2D& rhs) const;

        Vector2D abs() const;
        Vector2D unit() const;

        Vector2D min(const Vector2D& rhs) const;
        Vector2D& minInto(const Vector2D& rhs);
        Vector2D max(const Vector2D& rhs) const;
        Vector2D& maxInto(const Vector2D& rhs);

        Vector2D clamp(const Vector2D& min, const Vector2D& max) const;
        Vector2D& clampTo(const Vector2D& min, const Vector2D& max);

        Vector2D& clone(const Vector2D& source);

        Vector2D floor() const;
        Vector2D& floorTo();

        Vector2D perpendicular() const;

        static Vector2D fromAngle(const float& angleInRadians);
        float toAngle() const;

        Vector2D& clear();

        Vector2D getMajorAxis() const;

        friend std::ostream &operator<<(std::ostream &stream, const Vector2D &vector);
    };

} // hikari

#endif // HIKARI_CORE_GEOM_VECTOR2D
