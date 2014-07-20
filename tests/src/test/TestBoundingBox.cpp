#include "catch.hpp"

#include <hikari/core/geom/BoundingBox.hpp>
#include <hikari/core/math/Vector2.hpp>

//
// Tests for hikari::BoundingBox<T>
//

//
// Constructors
//
TEST_CASE( "BoundingBox/constructor/(x, y), w, h args", "BoundingBoxes are initialized with X, Y, Width, Height if supplied" ) {
    hikari::BoundingBox<float> bbox0(hikari::Vector2<float>(1.0f, 2.0f), 10.0f, 20.0f);

    REQUIRE( bbox0.getPosition() == hikari::Vector2<float>(1.0f, 2.0f) );
    REQUIRE( bbox0.getOrigin() == hikari::Vector2<float>(0.0f, 0.0f) );
    REQUIRE( bbox0.getWidth() == 10.0f );
    REQUIRE( bbox0.getHeight() == 20.0f );
}

TEST_CASE( "BoundingBox/constructor/x, y, w, h args", "BoundingBoxes are initialized with X, Y, Width, Height if supplied" ) {
    hikari::BoundingBox<float> bbox0(1.0f, 2.0f, 10.0f, 20.0f);

    REQUIRE( bbox0.getPosition() == hikari::Vector2<float>(1.0f, 2.0f) );
    REQUIRE( bbox0.getOrigin() == hikari::Vector2<float>(0.0f, 0.0f) );
    REQUIRE( bbox0.getWidth() == 10.0f );
    REQUIRE( bbox0.getHeight() == 20.0f );
}

TEST_CASE( "BoundingBox/constructor/copy constructor", "BoundingBoxes copy values from other BoundingBoxes when copy-constructed" ) {
    hikari::BoundingBox<float> bbox0(1.0f, 2.0f, 10.0f, 20.0f);
    hikari::BoundingBox<float> bbox1(bbox0);

    REQUIRE( bbox0.getPosition() == bbox1.getPosition() );
    REQUIRE( bbox0.getOrigin() == bbox1.getOrigin() );
    REQUIRE( bbox0.getWidth() == bbox1.getWidth() );
    REQUIRE( bbox0.getHeight() == bbox1.getHeight() );
    REQUIRE( bbox1.getPosition() == hikari::Vector2<float>(1.0f, 2.0f) );
    REQUIRE( bbox1.getOrigin() == hikari::Vector2<float>(0.0f, 0.0f) );
    REQUIRE( bbox1.getWidth() == 10.0f );
    REQUIRE( bbox1.getHeight() == 20.0f );
}


//
// Accessors
//
TEST_CASE( "BoundingBox/accessors/getPosition", "BoundingBox::getPosition() returns a BoundingBox's position" ) {
    hikari::BoundingBox<float> bbox0(3.0f, 4.0f, 10.0f, 10.0f);

    REQUIRE( bbox0.getPosition() == hikari::Vector2<float>(3.0f, 4.0f) );
}

TEST_CASE( "BoundingBox/accessors/getOrigin", "BoundingBox::getOrigin() returns a BoundingBox's origin" ) {
    hikari::BoundingBox<float> bbox0(3.0f, 4.0f, 10.0f, 10.0f);

    REQUIRE( bbox0.getOrigin() == hikari::Vector2<float>(0.0f, 0.0f) );
}

TEST_CASE( "BoundingBox/accessors/getOrigin (modified origin)", "BoundingBox::getOrigin() returns a BoundingBox's origin" ) {
    hikari::BoundingBox<float> bbox0(3.0f, 4.0f, 10.0f, 10.0f);
    bbox0.setOrigin(hikari::Vector2<float>(77.0f, 144.0f));

    REQUIRE( bbox0.getOrigin() == hikari::Vector2<float>(77.0f, 144.0f) );
}

TEST_CASE( "BoundingBox/accessors/getWidth", "BoundingBox::getWidth() returns a BoundingBox's width" ) {
    hikari::BoundingBox<float> bbox0(3.0f, 4.0f, 15.0f, 10.0f);

    REQUIRE( bbox0.getWidth() == 15.0f );
}

TEST_CASE( "BoundingBox/accessors/getHeight", "BoundingBox::getHeight() returns a BoundingBox's height" ) {
    hikari::BoundingBox<float> bbox0(3.0f, 4.0f, 15.0f, 10.0f);

    REQUIRE( bbox0.getHeight() == 10.0f );
}

TEST_CASE( "BoundingBox/accessors/getTop", "BoundingBox::getTop() returns a BoundingBox's top Y value" ) {
    hikari::BoundingBox<float> bbox0(3.0f, -2.0f, 10.0f, 10.0f);

    REQUIRE( bbox0.getTop() == -2.0f );
}

TEST_CASE( "BoundingBox/accessors/getRight", "BoundingBox::getRight() returns a BoundingBox's right X value" ) {
    hikari::BoundingBox<float> bbox0(3.0f, 4.0f, 10.0f, 10.0f);

    REQUIRE( bbox0.getRight() == 13.0f );
}

TEST_CASE( "BoundingBox/accessors/getBottom", "BoundingBox::getY() returns a BoundingBox's bottom Y value" ) {
    hikari::BoundingBox<float> bbox0(3.0f, 4.0f, 10.0f, 10.0f);

    REQUIRE( bbox0.getBottom() == 14.0f );
}

TEST_CASE( "BoundingBox/accessors/getLeft", "BoundingBox::getLeft() returns a BoundingBox's left X value" ) {
    hikari::BoundingBox<float> bbox0(3.0f, 4.0f, 10.0f, 10.0f);

    REQUIRE( bbox0.getLeft() == 3.0f );
}

TEST_CASE( "BoundingBox/accessors/getTopLeft", "BoundingBox::getTopLeft() returns a BoundingBox's top-left corner position" ) {
    hikari::BoundingBox<float> bbox0(3.0f, 4.0f, 10.0f, 10.0f);

    REQUIRE( bbox0.getTopLeft() == hikari::Vector2<float>(3.0f, 4.0f) );
}

TEST_CASE( "BoundingBox/accessors/getTopRight", "BoundingBox::getTopRight() returns a BoundingBox's top-right corner position" ) {
    hikari::BoundingBox<float> bbox0(3.0f, 4.0f, 10.0f, 10.0f);

    REQUIRE( bbox0.getTopRight() == hikari::Vector2<float>(13.0f, 4.0f) );
}

TEST_CASE( "BoundingBox/accessors/getBottomLeft", "BoundingBox::getBottomLeft() returns a BoundingBox's bottom-left corner position" ) {
    hikari::BoundingBox<float> bbox0(3.0f, 4.0f, 10.0f, 10.0f);

    REQUIRE( bbox0.getBottomLeft() == hikari::Vector2<float>(3.0f, 14.0f) );
}

TEST_CASE( "BoundingBox/accessors/getBottomRight", "BoundingBox::getBottomRight() returns a BoundingBox's bottom-right corner position" ) {
    hikari::BoundingBox<float> bbox0(3.0f, 4.0f, 10.0f, 10.0f);

    REQUIRE( bbox0.getBottomRight() == hikari::Vector2<float>(13.0f, 14.0f) );
}


//
// Modifiers
//
TEST_CASE( "BoundingBox/modifiers/setPosition (Vector2)", "BoundingBox::setPosition() changes a BoundingBox's position" ) {
    hikari::BoundingBox<float> bbox0(0.0f, 0.0f, 10.0f, 5.0f);

    bbox0.setPosition(hikari::Vector2<float>(13.0f, 14.0f));

    REQUIRE( bbox0.getPosition() == hikari::Vector2<float>(13.0f, 14.0f) );
}

TEST_CASE( "BoundingBox/modifiers/setPosition (x, y)", "BoundingBox::setPosition() changes a BoundingBox's position" ) {
    hikari::BoundingBox<float> bbox0(0.0f, 0.0f, 10.0f, 5.0f);

    bbox0.setPosition(13.0f, 14.0f);

    REQUIRE( bbox0.getPosition() == hikari::Vector2<float>(13.0f, 14.0f) );
}

TEST_CASE( "BoundingBox/modifiers/setOrigin (Vector2)", "BoundingBox::setOrigin() changes a BoundingBox's origin" ) {
    hikari::BoundingBox<float> bbox0(0.0f, 0.0f, 10.0f, 5.0f);

    bbox0.setOrigin(hikari::Vector2<float>(13.0f, 14.0f));

    REQUIRE( bbox0.getOrigin() == hikari::Vector2<float>(13.0f, 14.0f) );
}

TEST_CASE( "BoundingBox/modifiers/setOrigin (x, y)", "BoundingBox::setOrigin() changes a BoundingBox's origin" ) {
    hikari::BoundingBox<float> bbox0(0.0f, 0.0f, 10.0f, 5.0f);

    bbox0.setOrigin(13.0f, 14.0f);

    REQUIRE( bbox0.getOrigin() == hikari::Vector2<float>(13.0f, 14.0f) );
}

TEST_CASE( "BoundingBox/modifiers/setOrigin (modifies bounds)", "BoundingBox::setOrigin() changes a BoundingBox's calculated bounds" ) {
    hikari::BoundingBox<float> bbox0(0.0f, 0.0f, 10.0f, 5.0f);

    bbox0.setOrigin(1.0f, 1.0f);

    REQUIRE( bbox0.getTop() == -1.0f );
    REQUIRE( bbox0.getRight() == 9.0f );
    REQUIRE( bbox0.getBottom() == 4.0f );
    REQUIRE( bbox0.getLeft() == -1.0f );
}

TEST_CASE( "BoundingBox/modifiers/setOrigin (origin vs position)", "BoundingBox::setOrigin() does not change a BoundingBox's position" ) {
    hikari::BoundingBox<float> bbox0(2.0f, 3.0f, 10.0f, 5.0f);

    bbox0.setOrigin(13.0f, 14.0f);

    REQUIRE( bbox0.getPosition() == hikari::Vector2<float>(2.0f, 3.0f) );
}

TEST_CASE( "BoundingBox/modifiers/setWidth", "BoundingBox::setWidth() changes a BoundingBox's width" ) {
    hikari::BoundingBox<float> bbox0(2.0f, 3.0f, 10.0f, 5.0f);

    bbox0.setWidth(99.0f);

    REQUIRE( bbox0.getWidth() == 99.0f );
}

TEST_CASE( "BoundingBox/modifiers/setHeight", "BoundingBox::setHeight() changes a BoundingBox's height" ) {
    hikari::BoundingBox<float> bbox0(2.0f, 3.0f, 10.0f, 5.0f);

    bbox0.setHeight(99.0f);

    REQUIRE( bbox0.getHeight() == 99.0f );
}

TEST_CASE( "BoundingBox/modifiers/setSize", "BoundingBox::setSize() changes a BoundingBox's width and height" ) {
    hikari::BoundingBox<float> bbox0(2.0f, 3.0f, 1.0f, 1.0f);

    bbox0.setSize(45.0f, 105.0f);

    REQUIRE( bbox0.getWidth() == 45.0f );
    REQUIRE( bbox0.getHeight() == 105.0f );
}

TEST_CASE( "BoundingBox/modifiers/setTop", "BoundingBox::setTop() changes a BoundingBox's top Y position" ) {
    hikari::BoundingBox<float> bbox0(0.0f, 0.0f, 10.0f, 5.0f);

    bbox0.setTop(99.0f);

    REQUIRE( bbox0.getTop() == 99.0f );
}

TEST_CASE( "BoundingBox/modifiers/setTop (moves the box)", "BoundingBox::setTop() moves a BoundingBox's position if necessary" ) {
    hikari::BoundingBox<float> bbox0(0.0f, 0.0f, 10.0f, 5.0f);

    bbox0.setTop(99.0f);

    REQUIRE( bbox0.getPosition() == hikari::Vector2<float>(0.0f, 99.0f) );
}

TEST_CASE( "BoundingBox/modifiers/setRight", "BoundingBox::setRight() changes a BoundingBox's right X position" ) {
    hikari::BoundingBox<float> bbox0(0.0f, 0.0f, 10.0f, 5.0f);

    bbox0.setRight(99.0f);

    REQUIRE( bbox0.getRight() == 99.0f );
}

TEST_CASE( "BoundingBox/modifiers/setRight (moves the box)", "BoundingBox::setRight() moves a BoundingBox's position if necessary" ) {
    hikari::BoundingBox<float> bbox0(0.0f, 0.0f, 10.0f, 5.0f);

    bbox0.setRight(99.0f);

    REQUIRE( bbox0.getPosition() == hikari::Vector2<float>(89.0f, 0.0f) );
}

TEST_CASE( "BoundingBox/modifiers/setBottom", "BoundingBox::setBottom() changes a BoundingBox's bottom Y position" ) {
    hikari::BoundingBox<float> bbox0(0.0f, 0.0f, 10.0f, 5.0f);

    bbox0.setBottom(99.0f);

    REQUIRE( bbox0.getBottom() == 99.0f );
}

TEST_CASE( "BoundingBox/modifiers/setBottom (moves the box)", "BoundingBox::setBottom() moves a BoundingBox's position if necessary" ) {
    hikari::BoundingBox<float> bbox0(0.0f, 0.0f, 10.0f, 5.0f);

    bbox0.setBottom(99.0f);

    REQUIRE( bbox0.getPosition() == hikari::Vector2<float>(0.0f, 94.0f) );
}

TEST_CASE( "BoundingBox/modifiers/setLeft", "BoundingBox::setLeft() changes a BoundingBox's left X position" ) {
    hikari::BoundingBox<float> bbox0(0.0f, 0.0f, 10.0f, 5.0f);

    bbox0.setLeft(99.0f);

    REQUIRE( bbox0.getLeft() == 99.0f );
}

TEST_CASE( "BoundingBox/modifiers/setLeft (moves the box)", "BoundingBox::setLeft() moves a BoundingBox's position if necessary" ) {
    hikari::BoundingBox<float> bbox0(0.0f, 0.0f, 10.0f, 5.0f);

    bbox0.setLeft(99.0f);

    REQUIRE( bbox0.getPosition() == hikari::Vector2<float>(99.0f, 0.0f) );
}

//
// Methods
//
TEST_CASE( "BoundingBox/methods/intersects (BoundingBox)", "BoundingBox::intersects() is true for rectangles that intersect" ) {
    hikari::BoundingBox<float> bbox0(0.0f, 0.0f, 10.0f, 10.0f);
    hikari::BoundingBox<float> bbox1(-5.0f, -5.0f, 10.0f, 10.0f);

    REQUIRE( bbox0.intersects(bbox1) );
}

TEST_CASE( "BoundingBox/methods/intersects (BoundingBox, fully contained)", "BoundingBox::intersects() is true if the rectangle is fully contained" ) {
    hikari::BoundingBox<float> bbox0(0.0f, 0.0f, 10.0f, 10.0f);
    hikari::BoundingBox<float> bbox1(1.0f, 1.0f, 3.0f, 3.0f);

    REQUIRE( bbox0.intersects(bbox1) );
}

TEST_CASE( "BoundingBox/methods/non-intersects (BoundingBox)", "BoundingBox::intersects() is false for rectangles that do not intersect" ) {
    hikari::BoundingBox<float> bbox0(0.0f, 0.0f, 10.0f, 10.0f);
    hikari::BoundingBox<float> bbox1(-5.0f, -5.0f, 1.0f, 1.0f);

    REQUIRE_FALSE( bbox0.intersects(bbox1) );
}

TEST_CASE( "BoundingBox/methods/contains (BoundingBox)", "BoundingBox::contains() is true if the rectangle is fully contained" ) {
    hikari::BoundingBox<float> bbox0(0.0f, 0.0f, 10.0f, 10.0f);
    hikari::BoundingBox<float> bbox1(1.0f, 1.0f, 3.0f, 3.0f);

    REQUIRE( bbox0.contains(bbox1) );
}

TEST_CASE( "BoundingBox/methods/non-contains (BoundingBox, overlap only)", "BoundingBox::contains() is false if the rectangles only overlap" ) {
    hikari::BoundingBox<float> bbox0(0.0f, 0.0f, 10.0f, 10.0f);
    hikari::BoundingBox<float> bbox1(-5.0f, -5.0f, 10.0f, 10.0f);

    REQUIRE_FALSE( bbox0.contains(bbox1) );
}

TEST_CASE( "BoundingBox/methods/non-contains (BoundingBox, no overlap)", "BoundingBox::contains() is false if the rectangle is not contained" ) {
    hikari::BoundingBox<float> bbox0(0.0f, 0.0f, 10.0f, 10.0f);
    hikari::BoundingBox<float> bbox1(-5.0f, -5.0f, 1.0f, 1.0f);

    REQUIRE_FALSE( bbox0.contains(bbox1) );
}

TEST_CASE( "BoundingBox/methods/contains (x, y)", "BoundingBox::contains() is true if the point at (x, y) is contained in the rectangle" ) {
    hikari::BoundingBox<float> bbox0(0.0f, 0.0f, 10.0f, 10.0f);

    REQUIRE( bbox0.contains(2.0f, 2.0f) );
}

TEST_CASE( "BoundingBox/methods/non-contains (x, y)", "BoundingBox::contains() is false if the point at (x, y) is not contained in the rectangle" ) {
    hikari::BoundingBox<float> bbox0(0.0f, 0.0f, 10.0f, 10.0f);

    REQUIRE_FALSE( bbox0.contains(-2.0f, -2.0f) );
}

TEST_CASE( "BoundingBox/methods/isLeftOf (true case)", "BoundingBox::isLeftOf() is true if the other BoundingBox is to the right" ) {
    hikari::BoundingBox<float> bbox0(0.0f, 0.0f, 10.0f, 10.0f);
    hikari::BoundingBox<float> bbox1(20.0f, -5.0f, 1.0f, 1.0f);

    REQUIRE( bbox0.isLeftOf(bbox1) );
}

TEST_CASE( "BoundingBox/methods/isLeftOf (false case)", "BoundingBox::isLeftOf() is false if the other BoundingBox is to the left" ) {
    hikari::BoundingBox<float> bbox0(20.0f, -5.0f, 1.0f, 1.0f);
    hikari::BoundingBox<float> bbox1(0.0f, 0.0f, 10.0f, 10.0f);

    REQUIRE_FALSE( bbox0.isLeftOf(bbox1) );
}

TEST_CASE( "BoundingBox/methods/isLeftOf (false case, overlapping)", "BoundingBox::isLeftOf() is false if the other BoundingBox overlaps horizontally" ) {
    hikari::BoundingBox<float> bbox0(-5.0f, 0.0f, 10.0f, 10.0f);
    hikari::BoundingBox<float> bbox1(0.0f, 0.0f, 10.0f, 10.0f);

    REQUIRE_FALSE( bbox0.isLeftOf(bbox1) );
}

TEST_CASE( "BoundingBox/methods/isRightOf (true case)", "BoundingBox::isRightOf() is true if the other BoundingBox is to the left" ) {
    hikari::BoundingBox<float> bbox0(0.0f, 0.0f, 10.0f, 10.0f);
    hikari::BoundingBox<float> bbox1(-20.0f, -5.0f, 1.0f, 1.0f);

    REQUIRE( bbox0.isRightOf(bbox1) );
}

TEST_CASE( "BoundingBox/methods/isRightOf (false case)", "BoundingBox::isRightOf() is false if the other BoundingBox is to the right" ) {
    hikari::BoundingBox<float> bbox0(-20.0f, -5.0f, 1.0f, 1.0f);
    hikari::BoundingBox<float> bbox1(0.0f, 0.0f, 10.0f, 10.0f);

    REQUIRE_FALSE( bbox0.isRightOf(bbox1) );
}

TEST_CASE( "BoundingBox/methods/isRightOf (false case, overlapping)", "BoundingBox::isRightOf() is false if the other BoundingBox overlaps horizontally" ) {
    hikari::BoundingBox<float> bbox0(5.0f, 0.0f, 10.0f, 10.0f);
    hikari::BoundingBox<float> bbox1(0.0f, 0.0f, 10.0f, 10.0f);

    REQUIRE_FALSE( bbox0.isRightOf(bbox1) );
}

TEST_CASE( "BoundingBox/methods/isAbove (true case)", "BoundingBox::isAbove() is true if the other BoundingBox is below" ) {
    hikari::BoundingBox<float> bbox0(0.0f, 0.0f, 10.0f, 10.0f);
    hikari::BoundingBox<float> bbox1(20.0f, 15.0f, 1.0f, 10.0f);

    REQUIRE( bbox0.isAbove(bbox1) );
}

TEST_CASE( "BoundingBox/methods/isAbove (false case)", "BoundingBox::isAbove() is false if the other BoundingBox is above" ) {
    hikari::BoundingBox<float> bbox0(20.0f, 15.0f, 1.0f, 10.0f);
    hikari::BoundingBox<float> bbox1(0.0f, 0.0f, 10.0f, 10.0f);

    REQUIRE_FALSE( bbox0.isAbove(bbox1) );
}

TEST_CASE( "BoundingBox/methods/isAbove (false case, overlapping)", "BoundingBox::isAbove() is false if the other BoundingBox overlaps vertically" ) {
    hikari::BoundingBox<float> bbox0(0.0f, 0.0f, 10.0f, 10.0f);
    hikari::BoundingBox<float> bbox1(0.0f, 5.0f, 10.0f, 10.0f);

    REQUIRE_FALSE( bbox0.isAbove(bbox1) );
}

TEST_CASE( "BoundingBox/methods/isBelow (true case)", "BoundingBox::isBelow() is true if the other BoundingBox is above" ) {
    hikari::BoundingBox<float> bbox0(20.0f, 15.0f, 1.0f, 10.0f);
    hikari::BoundingBox<float> bbox1(0.0f, 0.0f, 10.0f, 10.0f);

    REQUIRE( bbox0.isBelow(bbox1) );
}

TEST_CASE( "BoundingBox/methods/isBelow (false case)", "BoundingBox::isBelow() is false if the other BoundingBox is below" ) {
    hikari::BoundingBox<float> bbox0(0.0f, 0.0f, 10.0f, 10.0f);
    hikari::BoundingBox<float> bbox1(20.0f, 15.0f, 1.0f, 10.0f);

    REQUIRE_FALSE( bbox0.isBelow(bbox1) );
}

TEST_CASE( "BoundingBox/methods/isBelow (false case, overlapping)", "BoundingBox::isBelow() is false if the other BoundingBox overlaps vertically" ) {
    hikari::BoundingBox<float> bbox0(0.0f, 5.0f, 10.0f, 10.0f);
    hikari::BoundingBox<float> bbox1(0.0f, 0.0f, 10.0f, 10.0f);

    REQUIRE_FALSE( bbox0.isBelow(bbox1) );
}
