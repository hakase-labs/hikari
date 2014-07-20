#include "catch.hpp"

#include <hikari/core/geom/Rectangle2D.hpp>

//
// Tests for hikari::Rectangle2D<T>
//

//
// Constructors
//
TEST_CASE( "Rectangle2D/constructor/noarg", "Rectangle2Ds are initialized to zero by default" ) {
    hikari::Rectangle2D<float> rectangle0;

    REQUIRE( rectangle0.getX() == 0.0f );
    REQUIRE( rectangle0.getY() == 0.0f );
    REQUIRE( rectangle0.getWidth() == 0.0f );
    REQUIRE( rectangle0.getHeight() == 0.0f );
}

TEST_CASE( "Rectangle2D/constructor/x, y, w, h args", "Rectangle2Ds are initialized with X, Y, Width, Height if supplied" ) {
    hikari::Rectangle2D<float> rectangle0(1.0f, 2.0f, 10.0f, 20.0f);

    REQUIRE( rectangle0.getX() == 1.0f );
    REQUIRE( rectangle0.getY() == 2.0f );
    REQUIRE( rectangle0.getWidth() == 10.0f );
    REQUIRE( rectangle0.getHeight() == 20.0f );
}

TEST_CASE( "Rectangle2D/constructor/copy constructor", "Rectangle2Ds copy values from other Rectangle2Ds when copy-constructed" ) {
    hikari::Rectangle2D<float> rectangle0(1.0f, 2.0f, 10.0f, 20.0f);
    hikari::Rectangle2D<float> rectangle1(rectangle0);

    REQUIRE( rectangle0.getX() == rectangle1.getX() );
    REQUIRE( rectangle0.getY() == rectangle1.getY() );
    REQUIRE( rectangle0.getWidth() == rectangle1.getWidth() );
    REQUIRE( rectangle0.getHeight() == rectangle1.getHeight() );
    REQUIRE( rectangle1.getX() == 1.0f );
    REQUIRE( rectangle1.getY() == 2.0f );
    REQUIRE( rectangle1.getWidth() == 10.0f );
    REQUIRE( rectangle1.getHeight() == 20.0f );
}

//
// Accessors
//
TEST_CASE( "Rectangle2D/accessors/getX", "Rectangle2D::getX() returns a Rectangle2D's X value" ) {
    hikari::Rectangle2D<float> rectangle0(3.0f, 4.0f, 10.0f, 10.0f);

    REQUIRE( rectangle0.getX() == 3.0f );
}

TEST_CASE( "Rectangle2D/accessors/getY", "Rectangle2D::getY() returns a Rectangle2D's Y value" ) {
    hikari::Rectangle2D<float> rectangle0(3.0f, 4.0f, 10.0f, 10.0f);

    REQUIRE( rectangle0.getY() == 4.0f );
}

TEST_CASE( "Rectangle2D/accessors/getWidth", "Rectangle2D::getWidth() returns a Rectangle2D's width" ) {
    hikari::Rectangle2D<float> rectangle0(3.0f, 4.0f, 15.0f, 10.0f);

    REQUIRE( rectangle0.getWidth() == 15.0f );
}

TEST_CASE( "Rectangle2D/accessors/getHeight", "Rectangle2D::getHeight() returns a Rectangle2D's height" ) {
    hikari::Rectangle2D<float> rectangle0(3.0f, 4.0f, 15.0f, 10.0f);

    REQUIRE( rectangle0.getHeight() == 10.0f );
}

TEST_CASE( "Rectangle2D/accessors/getTop", "Rectangle2D::getTop() returns a Rectangle2D's top Y value" ) {
    hikari::Rectangle2D<float> rectangle0(3.0f, -2.0f, 10.0f, 10.0f);

    REQUIRE( rectangle0.getTop() == -2.0f );
}

TEST_CASE( "Rectangle2D/accessors/getRight", "Rectangle2D::getRight() returns a Rectangle2D's right X value" ) {
    hikari::Rectangle2D<float> rectangle0(3.0f, 4.0f, 10.0f, 10.0f);

    REQUIRE( rectangle0.getRight() == 13.0f );
}

TEST_CASE( "Rectangle2D/accessors/getBottom", "Rectangle2D::getY() returns a Rectangle2D's bottom Y value" ) {
    hikari::Rectangle2D<float> rectangle0(3.0f, 4.0f, 10.0f, 10.0f);

    REQUIRE( rectangle0.getBottom() == 14.0f );
}

TEST_CASE( "Rectangle2D/accessors/getLeft", "Rectangle2D::getLeft() returns a Rectangle2D's left X value" ) {
    hikari::Rectangle2D<float> rectangle0(3.0f, 4.0f, 10.0f, 10.0f);

    REQUIRE( rectangle0.getLeft() == 3.0f );
}

//
// Modifiers
//
TEST_CASE( "Rectangle2D/modifiers/setX", "Rectangle2D::setX() changes a Rectangle2D's X value" ) {
    hikari::Rectangle2D<float> rectangle0;

    rectangle0.setX(7);

    REQUIRE( rectangle0.getX() == 7.0f );
}

TEST_CASE( "Rectangle2D/modifiers/setY", "Rectangle2D::setY() changes a Rectangle2D's Y value" ) {
    hikari::Rectangle2D<float> rectangle0;

    rectangle0.setY(7);

    REQUIRE( rectangle0.getY() == 7.0f );
}

TEST_CASE( "Rectangle2D/modifiers/setWidth", "Rectangle2D::setWidth() changes a Rectangle2D's width" ) {
    hikari::Rectangle2D<float> rectangle0;

    rectangle0.setWidth(7);

    REQUIRE( rectangle0.getWidth() == 7.0f );
}

TEST_CASE( "Rectangle2D/modifiers/setHeight", "Rectangle2D::setHeight() changes a Rectangle2D's height" ) {
    hikari::Rectangle2D<float> rectangle0;

    rectangle0.setHeight(7);

    REQUIRE( rectangle0.getHeight() == 7.0f );
}

//
// Methods
//
TEST_CASE( "Rectangle2D/methods/intersects (Rectangle2D)", "Rectangle2D::intersects() is true for rectangles that intersect" ) {
    hikari::Rectangle2D<float> rectangle0(0.0f, 0.0f, 10.0f, 10.0f);
    hikari::Rectangle2D<float> rectangle1(-5.0f, -5.0f, 10.0f, 10.0f);

    REQUIRE( rectangle0.intersects(rectangle1) );
}

TEST_CASE( "Rectangle2D/methods/intersects (Rectangle2D, fully contained)", "Rectangle2D::intersects() is true if the rectangle is fully contained" ) {
    hikari::Rectangle2D<float> rectangle0(0.0f, 0.0f, 10.0f, 10.0f);
    hikari::Rectangle2D<float> rectangle1(1.0f, 1.0f, 3.0f, 3.0f);

    REQUIRE( rectangle0.intersects(rectangle1) );
}

TEST_CASE( "Rectangle2D/methods/non-intersects (Rectangle2D)", "Rectangle2D::intersects() is false for rectangles that do not intersect" ) {
    hikari::Rectangle2D<float> rectangle0(0.0f, 0.0f, 10.0f, 10.0f);
    hikari::Rectangle2D<float> rectangle1(-5.0f, -5.0f, 1.0f, 1.0f);

    REQUIRE_FALSE( rectangle0.intersects(rectangle1) );
}

TEST_CASE( "Rectangle2D/methods/contains (Rectangle2D)", "Rectangle2D::contains() is true if the rectangle is fully contained" ) {
    hikari::Rectangle2D<float> rectangle0(0.0f, 0.0f, 10.0f, 10.0f);
    hikari::Rectangle2D<float> rectangle1(1.0f, 1.0f, 3.0f, 3.0f);

    REQUIRE( rectangle0.contains(rectangle1) );
}

TEST_CASE( "Rectangle2D/methods/non-contains (Rectangle2D, overlap only)", "Rectangle2D::contains() is false if the rectangles only overlap" ) {
    hikari::Rectangle2D<float> rectangle0(0.0f, 0.0f, 10.0f, 10.0f);
    hikari::Rectangle2D<float> rectangle1(-5.0f, -5.0f, 10.0f, 10.0f);

    REQUIRE_FALSE( rectangle0.contains(rectangle1) );
}

TEST_CASE( "Rectangle2D/methods/non-contains (Rectangle2D, no overlap)", "Rectangle2D::contains() is false if the rectangle is not contained" ) {
    hikari::Rectangle2D<float> rectangle0(0.0f, 0.0f, 10.0f, 10.0f);
    hikari::Rectangle2D<float> rectangle1(-5.0f, -5.0f, 1.0f, 1.0f);

    REQUIRE_FALSE( rectangle0.contains(rectangle1) );
}

TEST_CASE( "Rectangle2D/methods/contains (x, y)", "Rectangle2D::contains() is true if the point at (x, y) is contained in the rectangle" ) {
    hikari::Rectangle2D<float> rectangle0(0.0f, 0.0f, 10.0f, 10.0f);

    REQUIRE( rectangle0.contains(2.0f, 2.0f) );
}

TEST_CASE( "Rectangle2D/methods/non-contains (x, y)", "Rectangle2D::contains() is false if the point at (x, y) is not contained in the rectangle" ) {
    hikari::Rectangle2D<float> rectangle0(0.0f, 0.0f, 10.0f, 10.0f);

    REQUIRE_FALSE( rectangle0.contains(-2.0f, -2.0f) );
}