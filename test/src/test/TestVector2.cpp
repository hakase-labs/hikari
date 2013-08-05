#include "catch.hpp"

#include <hikari/core/math/Vector2.hpp>
#include <hikari/core/geom/Point2D.hpp>

//
// Tests for hikari::Vector2<T>
//

//
// Constructors
//
TEST_CASE( "Vector2/constructor/noarg", "Vectors are initialized to zero by default" ) {
    hikari::Vector2<float> vec0;

    REQUIRE( vec0.getX() == 0 );
    REQUIRE( vec0.getY() == 0 );
}

TEST_CASE( "Vector2/constructor/x and y args", "Vectors are initialized with X, Y if supplied" ) {
    hikari::Vector2<float> vec0(1, 2);

    REQUIRE( vec0.getX() == 1 );
    REQUIRE( vec0.getY() == 2 );
}

TEST_CASE( "Vector2/constructor/copy constructor", "Vectors copy values from other Vector2s when copy-constructed" ) {
    hikari::Vector2<float> vec0(10, 20);
    hikari::Vector2<float> vec1(vec0);

    REQUIRE( vec0.getX() == vec1.getX() );
    REQUIRE( vec0.getY() == vec1.getY() );
    REQUIRE( vec1.getX() == 10 );
    REQUIRE( vec1.getY() == 20 );
}

TEST_CASE( "Vector2/constructor/conversion from Point2D", "Vectors can be constructed from Point2D<T>s" ) {
    hikari::Point2D<float> point0(15, 25);
    hikari::Vector2<float> vec0(point0);

    REQUIRE( vec0.getX() == point0.getX() );
    REQUIRE( vec0.getY() == point0.getY() );
    REQUIRE( vec0.getX() == 15 );
    REQUIRE( vec0.getY() == 25 );
}

//
// Accessors
//
TEST_CASE( "Vector2/accessors/getX", "Vector2::getX() returns a Vector's X value" ) {
    hikari::Vector2<float> vec0(3, 4);

    REQUIRE( vec0.getX() == 3 );
}

TEST_CASE( "Vector2/accessors/getY", "Vector2::getY() returns a Vector's Y value" ) {
    hikari::Vector2<float> vec0(3, 4);

    REQUIRE( vec0.getY() == 4 );
}

TEST_CASE( "Vector2/accessors/getLengthSquared", "Vector2::getLengthSquared() returns a Vector's length squared" ) {
    hikari::Vector2<float> vec0(3, 4);

    // Pythagorean triple (3, 4, 5)
    REQUIRE( vec0.getLengthSquared() == 25 );
}

TEST_CASE( "Vector2/accessors/getLength", "Vector2::getLength() returns a Vector's length" ) {
    hikari::Vector2<float> vec0(3, 4);

    // Pythagorean triple (3, 4, 5)
    REQUIRE( vec0.getLength() == 5 );
}

//
// Modifiers
//
TEST_CASE( "Vector2/modifiers/setX", "Vector2::setX() changes a Vector's X value" ) {
    hikari::Vector2<float> vec0;

    vec0.setX(7);

    // Pythagorean triple (3, 4, 5)
    REQUIRE( vec0.getX() == 7 );
}

TEST_CASE( "Vector2/modifiers/setY", "Vector2::setY() changes a Vector's Y value" ) {
    hikari::Vector2<float> vec0;

    vec0.setY(7);

    // Pythagorean triple (3, 4, 5)
    REQUIRE( vec0.getY() == 7 );
}

//
// Operators
//
TEST_CASE( "Vector2/operators/unary negation", "Vector's components are negated'" ) {
    hikari::Vector2<float> vec0(1, 2);

    vec0 = -vec0;

    REQUIRE( vec0.getX() == -1 );
    REQUIRE( vec0.getY() == -2 );
}

TEST_CASE( "Vector2/operators/plus", "Vectors' components are added together" ) {
    hikari::Vector2<float> vec0(1, 2);
    hikari::Vector2<float> vec1(3, 4);
    hikari::Vector2<float> vec2 = vec0 + vec1;

    REQUIRE( vec2.getX() == 4 );
    REQUIRE( vec2.getY() == 6 );
}

TEST_CASE( "Vector2/operators/plus equals", "Vectors' components are added together, modifiying the original" ) {
    hikari::Vector2<float> vec0(1, 2);
    hikari::Vector2<float> vec1(3, 4);

    vec0 += vec1;

    REQUIRE( vec0.getX() == 4 );
    REQUIRE( vec0.getY() == 6 );
}

TEST_CASE( "Vector2/operators/minus", "Vectors' components are subtracted" ) {
    hikari::Vector2<float> vec0(1, 2);
    hikari::Vector2<float> vec1(3, 4);
    hikari::Vector2<float> vec2 = vec0 - vec1;

    REQUIRE( vec2.getX() == -2 );
    REQUIRE( vec2.getY() == -2 );
}

TEST_CASE( "Vector2/operators/minus equals", "Vectors' components are subtracted, modifiying the original" ) {
    hikari::Vector2<float> vec0(1, 2);
    hikari::Vector2<float> vec1(3, 4);

    vec0 -= vec1;

    REQUIRE( vec0.getX() == -2 );
    REQUIRE( vec0.getY() == -2 );
}

TEST_CASE( "Vector2/operators/multiply", "Vectors' components are multiplied" ) {
    hikari::Vector2<float> vec0(1, 2);
    hikari::Vector2<float> vec1(3, 4);
    hikari::Vector2<float> vec2 = vec0 * vec1;

    REQUIRE( vec2.getX() == 3 );
    REQUIRE( vec2.getY() == 8 );
}

TEST_CASE( "Vector2/operators/multiply equals", "Vectors' components are multiplied, modifiying the original" ) {
    hikari::Vector2<float> vec0(1, 2);
    hikari::Vector2<float> vec1(3, 4);

    vec0 *= vec1;

    REQUIRE( vec0.getX() == 3 );
    REQUIRE( vec0.getY() == 8 );
}

TEST_CASE( "Vector2/operators/multiply with scalar", "Vectors' components are multiplied by the scalar" ) {
    hikari::Vector2<float> vec0(1, 2);
    hikari::Vector2<float> vec2 = vec0 * 3;

    REQUIRE( vec2.getX() == 3 );
    REQUIRE( vec2.getY() == 6 );
}

TEST_CASE( "Vector2/operators/multiply equals with scalar", "Vectors' components are multiplied by the scalar, modifiying the original" ) {
    hikari::Vector2<float> vec0(1, 2);

    vec0 *= 3;

    REQUIRE( vec0.getX() == 3 );
    REQUIRE( vec0.getY() == 6 );
}

TEST_CASE( "Vector2/operators/divide", "Vectors' components are divided" ) {
    hikari::Vector2<float> vec0(10, 30);
    hikari::Vector2<float> vec1(2, 5);
    hikari::Vector2<float> vec2 = vec0 / vec1;

    REQUIRE( vec2.getX() == 5 );
    REQUIRE( vec2.getY() == 6 );
}

TEST_CASE( "Vector2/operators/divide equals", "Vectors' components are divided, modifiying the original" ) {
    hikari::Vector2<float> vec0(10, 30);
    hikari::Vector2<float> vec1(2, 5);

    vec0 /= vec1;

    REQUIRE( vec0.getX() == 5 );
    REQUIRE( vec0.getY() == 6 );
}

TEST_CASE( "Vector2/operators/divide with scalar", "Vectors' components are divided by the scalar" ) {
    hikari::Vector2<float> vec0(10, 30);
    hikari::Vector2<float> vec2 = vec0 / 5;

    REQUIRE( vec2.getX() == 2 );
    REQUIRE( vec2.getY() == 6 );
}

TEST_CASE( "Vector2/operators/divide equals with scalar", "Vectors' components are divided by the scalar, modifiying the original" ) {
    hikari::Vector2<float> vec0(10, 30);

    vec0 /= 5;

    REQUIRE( vec0.getX() == 2 );
    REQUIRE( vec0.getY() == 6 );
}

TEST_CASE( "Vector2/operators/equality", "Vectors are equal if their components are equal" ) {
    hikari::Vector2<float> vec0(1, 2);
    hikari::Vector2<float> vec1(1, 2);

    REQUIRE( vec0 == vec1 );
}

TEST_CASE( "Vector2/operators/equality (self)", "Vectors are equal to themselves" ) {
    hikari::Vector2<float> vec0(1, 2);

    REQUIRE( vec0 == vec0 );
}

TEST_CASE( "Vector2/operators/inequality", "Vectors are not equal if their components are not equal" ) {
    hikari::Vector2<float> vec0(1, 2);
    hikari::Vector2<float> vec1(2, 1);

    REQUIRE( vec0 != vec1 );
}

//
// Methods
//
TEST_CASE( "Vector2/methods/dot (perpendicular)", "Vector2::dot() is zero if the two vectors are perpendicular" ) {
    hikari::Vector2<float> vec0(-12, 16); // Lean back
    hikari::Vector2<float> vec1(12, 9);   // Lean forward

    REQUIRE( vec0.dot(vec1) == 0 );
}

TEST_CASE( "Vector2/methods/dot", "Vector2::dot() returns the magnitude of two Vectors multipled together" ) {
    hikari::Vector2<float> vec0(-6, 8);
    hikari::Vector2<float> vec1(5, 12);

    REQUIRE( vec0.dot(vec1) == 66 );
}

TEST_CASE( "Vector2/methods/abs", "Vector2::abs() returns a new Vector2 with the absolute values of the original Vector2" ) {
    hikari::Vector2<float> vec0(-12, -16);
    hikari::Vector2<float> vec1 = vec0.abs();

    REQUIRE( vec1.getX() == 12 );
    REQUIRE( vec1.getY() == 16 );
}

TEST_CASE( "Vector2/methods/abs (mixed values)", "Vector2::abs() leaves positive values unchanged" ) {
    hikari::Vector2<float> vec0(8, 9);
    hikari::Vector2<float> vec1 = vec0.abs();

    REQUIRE( vec1.getX() == 8 );
    REQUIRE( vec1.getY() == 9 );
}

TEST_CASE( "Vector2/methods/floor (positive number)", "Vector2::floor() rounds values down to the closest integer, modifying the original" ) {
    hikari::Vector2<float> vec0(12.5, 16.125);

    vec0.floor();

    REQUIRE( vec0.getX() == 12.0f );
    REQUIRE( vec0.getY() == 16.0f );
}

TEST_CASE( "Vector2/methods/floor (negative number)", "Vector2::floor() rounds values down to the closest integer, modifying the original" ) {
    hikari::Vector2<float> vec0(-12.5, 16.125);

    vec0.floor();

    REQUIRE( vec0.getX() == -13.0f );
    REQUIRE( vec0.getY() == 16.0f );
}

TEST_CASE( "Vector2/methods/toFloor (positive number)", "Vector2::toFloor() rounds values down to the closest integer, returning a new Vector2" ) {
    hikari::Vector2<float> vec0(12.5, 16.125);
    hikari::Vector2<float> vec1 = vec0.toFloor();

    REQUIRE( vec1.getX() == 12.0f );
    REQUIRE( vec1.getY() == 16.0f );
}

TEST_CASE( "Vector2/methods/toFloor (negative number)", "Vector2::toFloor() rounds values down to the closest integer, returning a new Vector2" ) {
    hikari::Vector2<float> vec0(-12.5, 16.125);
    hikari::Vector2<float> vec1 = vec0.toFloor();

    REQUIRE( vec1.getX() == -13.0f );
    REQUIRE( vec1.getY() == 16.0f );
}

TEST_CASE( "Vector2/methods/toFloor unchanged", "Vector2::toFloor() does not modify the original Vector2" ) {
    hikari::Vector2<float> vec0(-12.5, 16.125);
    
    vec0.toFloor();

    REQUIRE( vec0.getX() == -12.5f );
    REQUIRE( vec0.getY() == 16.125f );
}

TEST_CASE( "Vector2/methods/ceil (positive number)", "Vector2::ceil() rounds values up to the closest integer, modifying the original" ) {
    hikari::Vector2<float> vec0(12.5, 16.125);

    vec0.ceil();

    REQUIRE( vec0.getX() == 13.0f );
    REQUIRE( vec0.getY() == 17.0f );
}

TEST_CASE( "Vector2/methods/ceil (negative number)", "Vector2::ceil() rounds values up to the closest integer, modifying the original" ) {
    hikari::Vector2<float> vec0(-12.5, 16.125);

    vec0.ceil();

    REQUIRE( vec0.getX() == -12.0f );
    REQUIRE( vec0.getY() == 17.0f );
}

TEST_CASE( "Vector2/methods/toCeil (positive number)", "Vector2::toCeil() rounds values up to the closest integer, returning a new Vector2" ) {
    hikari::Vector2<float> vec0(12.5f, 16.125f);
    hikari::Vector2<float> vec1 = vec0.toCeil();

    REQUIRE( vec1.getX() == 13.0f );
    REQUIRE( vec1.getY() == 17.0f );
}

TEST_CASE( "Vector2/methods/toCeil (negative number)", "Vector2::toCeil() rounds values up to the closest integer, returning a new Vector2" ) {
    hikari::Vector2<float> vec0(-12.5f, 16.5f);
    hikari::Vector2<float> vec1 = vec0.toCeil();

    REQUIRE( vec1.getX() == -12.0f );
    REQUIRE( vec1.getY() == 17.0f );
}

TEST_CASE( "Vector2/methods/toCeil unchanged", "Vector2::toCeil() does not modify the original Vector2" ) {
    hikari::Vector2<float> vec0(12.5f, 16.125f);
    
    vec0.toCeil();

    REQUIRE( vec0.getX() == 12.5f );
    REQUIRE( vec0.getY() == 16.125f );
}