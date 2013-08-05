#include "catch.hpp"

#include <hikari/core/geom/Point2D.hpp>

//
// Tests for hikari::Point2D<T>
//

//
// Constructors
//
TEST_CASE( "Point2D/constructor/noarg", "Point2Ds are initialized to zero by default" ) {
    hikari::Point2D<float> point0;

    REQUIRE( point0.getX() == 0.0f );
    REQUIRE( point0.getY() == 0.0f );
}

TEST_CASE( "Point2D/constructor/x and y args", "Point2Ds are initialized with X, Y if supplied" ) {
    hikari::Point2D<float> point0(1.0f, 2.0f);

    REQUIRE( point0.getX() == 1.0f );
    REQUIRE( point0.getY() == 2.0f );
}

TEST_CASE( "Point2D/constructor/copy constructor", "Point2Ds copy values from other Point2Ds when copy-constructed" ) {
    hikari::Point2D<float> point0(10.0f, 20.0f);
    hikari::Point2D<float> point1(point0);

    REQUIRE( point0.getX() == point1.getX() );
    REQUIRE( point0.getY() == point1.getY() );
    REQUIRE( point1.getX() == 10.0f );
    REQUIRE( point1.getY() == 20.0f );
}

//
// Accessors
//
TEST_CASE( "Point2D/accessors/getX", "Point2D::getX() returns a Point2D's X value" ) {
    hikari::Point2D<float> point0(3.0f, 4.0f);

    REQUIRE( point0.getX() == 3.0f );
}

TEST_CASE( "Point2D/accessors/getY", "Point2D::getY() returns a Point2D's Y value" ) {
    hikari::Point2D<float> point0(3.0f, 4.0f);

    REQUIRE( point0.getY() == 4.0f );
}

//
// Modifiers
//
TEST_CASE( "Point2D/modifiers/setX", "Point2D::setX() changes a Point2D's X value" ) {
    hikari::Point2D<float> point0;

    point0.setX(7);

    REQUIRE( point0.getX() == 7.0f );
}

TEST_CASE( "Point2D/modifiers/setY", "Point2D::setY() changes a Point2D's Y value" ) {
    hikari::Point2D<float> point0;

    point0.setY(7);

    REQUIRE( point0.getY() == 7.0f );
}
