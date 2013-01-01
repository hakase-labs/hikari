#include <hikari/core/geom/Vector2D.hpp>
#include <hikari/core/math/Constants.hpp>
#include <hikari/core/math/MathUtils.hpp>
#include <cmath>

namespace hikari {

    const float Vector2D::EPSILON = 0.0001f;
    const float Vector2D::TWO_PI = 2 * hikari::math::constants::PI;

    Vector2D::Vector2D()
        : point(0, 0)
    {

    }

    Vector2D::Vector2D(float x, float y)
        : point(x, y)
    {

    }

    Vector2D::Vector2D(const Vector2D& copy)
        : point(copy.point)
    {

    }

    /**
    */
    const float Vector2D::getX() const {
        return point.getX();
    }

    /**
    */
    const float Vector2D::getY() const {
        return point.getY();
    }

    
    /**
        Gets the length of the vector squared.
    */
    const float Vector2D::getLengthSquared() const {
        return dot(*this);
    }

    /**
        Gets the length of the vector.
    */
    const float Vector2D::getLength() const {
        return std::sqrt(getLengthSquared());
    }

    /**

    */
    Vector2D& Vector2D::setX(const float& newX) {
        point.setX(newX);
        return *this;
    }

    /**
    */
    Vector2D& Vector2D::setY(const float& newY) {
        point.setY(newY);
        return *this;
    }

    /**
        Creates a vector by negating a given vector.
    */
    Vector2D Vector2D::operator-() const {
        return Vector2D(-getX(), -getY());
    }

    /**
        Adds two vectors together to create a new vector.
    */
    Vector2D Vector2D::operator + (const Vector2D& rhs) const {
        Vector2D result(*this);

        result += rhs;

        return result;
    }

    /**
        Adds the right-hand vector to the left-hand vector.
        The left-hand vector is mutated.
    */
    Vector2D& Vector2D::operator += (const Vector2D& rhs) {
        setX(getX() + rhs.getX());
        setY(getY() + rhs.getY());
        return *this;
    }

    /**
        Subtracts two vectors to create a new vector.
    */
    Vector2D Vector2D::operator - (const Vector2D& rhs) const {
        Vector2D result(*this);

        result -= rhs;

        return result;
    }

    /**
        Subtracts the right-hand vector from the left-hand vector.
        The left-hand vector is mutated.
    */
    Vector2D& Vector2D::operator -= (const Vector2D& rhs) {
        setX(getX() - rhs.getX());
        setY(getY() - rhs.getY());
        return *this;
    }

    /**
        Multiplies two vectors creating a new vector.
    */
    Vector2D Vector2D::operator * (const Vector2D& rhs) const {
        return Vector2D(
            getX() * rhs.getX(),
            getY() * rhs.getY()
        );
    }

    /**
        Multiplies the left-hand vector by the right-hand vector.
        The left-hand vector is mutated.
    */
    Vector2D& Vector2D::operator *= (const Vector2D& rhs) {
        setX(getX() * rhs.getX());
        setY(getY() * rhs.getY());
        return *this;
    }

    /**
        Multiplies a vector by a scalar creating a new vector.
    */
    Vector2D Vector2D::operator * (float k) const {
        return Vector2D(
            getX() * k, 
            getY() * k
        );
    }

    /**
        Multiplies a vector by a scalar. The vector is mutated.
    */
    Vector2D& Vector2D::operator *= (float k) {
        setX(getX() * k);
        setY(getY() * k);
        return *this;
    }

    /**
        Divides two vectors creating a new vector.
    */
    Vector2D Vector2D::operator / (const Vector2D& rhs) const {
        return Vector2D(
            getX() / rhs.getX(),
            getY() / rhs.getY()
        );
    }

    /**
        Divides the left-hand vector by the right-hand vector.
        The left-hand vector is mutated.
    */
    Vector2D& Vector2D::operator /= (const Vector2D& rhs) {
        setX(getX() / rhs.getX());
        setY(getY() / rhs.getY());
        return *this;
    }

    /**
        Divides a vector by a scalar creating a new vector.
    */
    Vector2D Vector2D::operator / (float k) const {
        return Vector2D(
            getX() / k,
            getY() / k
        );
    }

    /**
        Divides the left-hand vector by the right-hand vector.
        The left-hand vector is mutated.
    */
    Vector2D& Vector2D::operator /= (float k) {
        setX(getX() / k);
        setY(getY() / k);
        return *this;
    }

    /**
        Tests if two vectors are equal. Equality is approximate since 
        floating-point numbers are used.
    */
    bool Vector2D::operator == (const Vector2D& rhs) const {
        bool xComponentIsApproximatelyEqual = std::abs(getX() - rhs.getX()) <= EPSILON;
        bool yComponentIsApproximatelyEqual = std::abs(getY() - rhs.getY()) <= EPSILON;
        return xComponentIsApproximatelyEqual && yComponentIsApproximatelyEqual;
    }

    /**
        Tests if two vectors are not equal. Equality is approximate since
        floating-point numbers are used.
    */
    bool Vector2D::operator != (const Vector2D& rhs) const {
        return !(*this == rhs);
    }

    /**
        Calculates the dot products of two vectors. The dot products is the
        cosine of the angle between two vectors.
    */
    const float Vector2D::dot(const Vector2D& rhs) const {
        return getX() * rhs.getX() + getY() * rhs.getY();
    }

    /**
        Creates a vector representing the absolute-value of a given vector.
    */
    Vector2D Vector2D::abs() const {
        return Vector2D(
            std::abs(getX()),
            std::abs(getY())
        );
    }

    /**
        Creates a vector that is the unit vector of a given vector.
    */
    Vector2D Vector2D::unit() const {
        float inverseLength = 1.0f / getLength();
        return (*this) * inverseLength;
    }

    /**
        Creates a vector from the smallest components of this vector
        and another.
    */
    Vector2D Vector2D::min(const Vector2D& rhs) const {
        return Vector2D(*this).minInto(rhs);
    }

    /**
        Mutates this vector so its components are the smallest components
        from itself and another vector.
    */
    Vector2D& Vector2D::minInto(const Vector2D& rhs) {
        setX(std::min(getX(), rhs.getX()));
        setY(std::min(getY(), rhs.getY()));

        return *this;
    }

    /**
        Creates a vector from the largest components of this vector 
        and another.
    */
    Vector2D Vector2D::max(const Vector2D& rhs) const {
        return Vector2D(*this).maxInto(rhs);
    }

    /**
        Mutates this vector so its components are the largest components
        from itself and another vector.
    */
    Vector2D& Vector2D::maxInto(const Vector2D& rhs) {
        setX(std::max(getX(), rhs.getX()));
        setY(std::max(getY(), rhs.getY()));

        return *this;
    }

    /**
        Creates a vector representing a given vector who's components have
        been rounded down to the nearest whole number.
    */
    Vector2D Vector2D::floor() const {
        return Vector2D(
            std::floor(getX()),
            std::floor(getY())
        );
    }

    Vector2D& Vector2D::floorTo() {
        return setX( std::floor(getX()) ).setY( std::floor(getY()) );
    }

    /**
        Creates a vector representing a given vector who's values have
        been clamped between two vectors.
    */
    Vector2D Vector2D::clamp(const Vector2D& min, const Vector2D& max) const {
        return Vector2D(*this).clampTo(min, max);
    }

    /**
        Clamps a given vector's components between two vectors' components.
    */
    Vector2D& Vector2D::clampTo(const Vector2D& min, const Vector2D& max) {
        setX(
            std::max(
                std::min(getX(), max.getX()), 
                min.getX() 
            )    
        );
        setY(
            std::max(
                std::min(getY(), max.getY()),
                min.getY()
            )
        );
        return *this;
    }

    Vector2D& Vector2D::clone(const Vector2D& source) {
        return setX(source.getX()).setY(source.getY());
    }

    /**
        Creates a vector that is perpendicular to a given vector.
    */
    Vector2D Vector2D::perpendicular() const {
        return Vector2D(-getY(), getX());
    }

    /**
        Creates a vector pointing in the direction specified.
    */
    Vector2D Vector2D::fromAngle(const float& angleInRadians) {
        return Vector2D(
            std::cos(angleInRadians), 
            std::sin(angleInRadians)
        );
    }

    /**
        Calculates the angle of a given vector and returns it.
    */
    float Vector2D::toAngle() const {
        float angle = std::atan2(getX(), getY());

        // ensure that the range is between 0 -> 2*PI
        if(angle < 0.0f) {
            angle += TWO_PI;
        }

        return angle;
    }

    /**
        Resets a given vector to (0, 0).
    */
    Vector2D& Vector2D::clear() {
        setX(0.0f);
        setY(0.0f);
        return *this;
    }

    /**
        Gets the largest components of a vector and creates a signed unit
        vector containing only that component.
    */
    Vector2D Vector2D::getMajorAxis() const {
        if(std::abs(getX()) > std::abs(getY())) {
            // x is the major axis
            return Vector2D(hikari::math::sign<float>(getX()), 0.0f);
        } else {
            // y is the major axis
            return Vector2D(0.0f, hikari::math::sign<float>(getY()));
        }
    }

    /**
        Prints a vector to a stream as a string.
    */
    std::ostream &operator<<(std::ostream &stream, const Vector2D &vector) {
        stream << "(";
        stream << vector.getX();
        stream << ", ";
        stream << vector.getY();
        stream << ")";
        return stream;
    }
}