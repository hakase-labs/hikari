#include "catch.hpp"

#include <hikari/core/geom/GeometryUtils.hpp>
#include <hikari/core/geom/BoundingBox.hpp>
#include <hikari/core/geom/Rectangle2D.hpp>
#include <hikari/core/math/Vector2.hpp>

//
// Tests for functions in the hikari::geom namespace
//

//
// Containment
//
TEST_CASE( "hikari::geom::contains/BoundingBox and Rectangle2D", "Returns true if the Rectangle2D is fully contained in the BoundingBox" ) {
    const hikari::Rectangle2D<float> rectangle0(1.0f, 1.0f, 8.0f, 8.0f);
    const hikari::BoundingBox<float> bbox0(0.0f, 0.0f, 16.0f, 16.0f);

    REQUIRE( hikari::geom::contains(bbox0, rectangle0) );
}

TEST_CASE( "hikari::geom::contains/BoundingBox and Rectangle2D/intersecting", "Returns false if the Rectangle2D only intersects the BoundingBox" ) {
    const hikari::Rectangle2D<float> rectangle0(1.0f, 1.0f, 16.0f, 16.0f);
    const hikari::BoundingBox<float> bbox0(0.0f, 0.0f, 16.0f, 16.0f);

    REQUIRE_FALSE( hikari::geom::contains(bbox0, rectangle0) );
}

TEST_CASE( "hikari::geom::contains/Rectangle2D and BoundingBox", "Returns true if the BoundingBox is fully contained in the Rectangle2D" ) {
    const hikari::BoundingBox<float> bbox0(1.0f, 1.0f, 8.0f, 8.0f);
    const hikari::Rectangle2D<float> rectangle0(0.0f, 0.0f, 16.0f, 16.0f);

    REQUIRE( hikari::geom::contains(rectangle0, bbox0) );
}

TEST_CASE( "hikari::geom::contains/Rectangle2D and BoundingBox/intersecting", "Returns false if the BoundingBox only intersects the Rectangle2D" ) {
    const hikari::BoundingBox<float> bbox0(1.0f, 1.0f, 16.0f, 16.0f);
    const hikari::Rectangle2D<float> rectangle0(0.0f, 0.0f, 16.0f, 16.0f);

    REQUIRE_FALSE( hikari::geom::contains(rectangle0, bbox0) );
}

//
// Intersection
//
TEST_CASE( "hikari::geom::intersects/Rectangle2D and BoundingBox", "Returns true if the BoundingBox intersects the Rectangle2D" ) {
    const hikari::Rectangle2D<float> rectangle0(1.0f, 1.0f, 16.0f, 16.0f);
    const hikari::BoundingBox<float> bbox0(0.0f, 0.0f, 16.0f, 16.0f);

    REQUIRE( hikari::geom::intersects(rectangle0, bbox0) );
}

TEST_CASE( "hikari::geom::intersects/Rectangle2D and BoundingBox/non intersection", "Returns false if the BoundingBox does not intersect the Rectangle2D" ) {
    const hikari::Rectangle2D<float> rectangle0(20.0f, 20.0f, 16.0f, 16.0f);
    const hikari::BoundingBox<float> bbox0(0.0f, 0.0f, 16.0f, 16.0f);

    REQUIRE_FALSE( hikari::geom::intersects(rectangle0, bbox0) );
}

TEST_CASE( "hikari::geom::intersects/BoundingBox and Rectangle2D", "Returns true if the Rectangle2D intersects the BoundingBox" ) {
    const hikari::Rectangle2D<float> rectangle0(0.0f, 0.0f, 16.0f, 16.0f);
    const hikari::BoundingBox<float> bbox0(1.0f, 1.0f, 16.0f, 16.0f);

    REQUIRE( hikari::geom::intersects(bbox0, rectangle0) );
}

TEST_CASE( "hikari::geom::intersects/BoundingBox and Rectangle2D/non intersection", "Returns false if the Rectangle2D does not intersect the BoundingBox" ) {
    const hikari::Rectangle2D<float> rectangle0(0.0f, 0.0f, 16.0f, 16.0f);
    const hikari::BoundingBox<float> bbox0(20.0f, 20.0f, 16.0f, 16.0f);

    REQUIRE_FALSE( hikari::geom::intersects(bbox0, rectangle0) );
}

TEST_CASE( "hikari::geom::intersection", "Returns an intersection of two BoundingBoxes" ) {
    const hikari::BoundingBox<float> bbox0(0.0f, 0.0f, 16.0f, 16.0f);
    const hikari::BoundingBox<float> bbox1(4.0f, 4.0f, 16.0f, 16.0f);
    const hikari::BoundingBox<float> result = hikari::geom::intersection(bbox0, bbox1);

    REQUIRE( result.getWidth() == 12.0f );
    REQUIRE( result.getHeight() == 12.0f );
    REQUIRE( result.getPosition() == hikari::Vector2<float>(4.0f, 4.0f) );
}
