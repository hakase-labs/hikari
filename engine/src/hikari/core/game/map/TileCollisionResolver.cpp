#include <hikari/core/game/map/TileCollisionResolver.hpp>
#include <hikari/core/game/map/Room.hpp>
#include <hikari/core/geom/AABB.hpp>
#include <hikari/core/geom/RectangleAABB.hpp>
#include <hikari/core/geom/Vector2D.hpp>

namespace hikari {

    void TileCollisionResolver::checkTilesWithinAabb(const Room& room, const Vector2D& min, const Vector2D& max, TileCollisionHandler handler, const float& dt) {
        // Round down
        int minX = static_cast<int>(min.getX()) / room.getGridSize();
        int minY = static_cast<int>(min.getY()) / room.getGridSize();

        // Round up
        int maxX = static_cast<int>(max.getX() + 0.5f) / room.getGridSize();
        int maxY = static_cast<int>(max.getY() + 0.5f) / room.getGridSize();

        // Since tiles are quare no need to do x/y separately
        float tileCenterOffset = static_cast<float>(room.getGridSize() / 2);
        float tileHalfExtents = tileCenterOffset;

        for(int x = minX; x <= maxX; x++) {
            for(int y = minY; y <= maxY; y++) {
                // Generate AABB for this tile
                RectangleAABB tileAabb(
                    Vector2D(
                        static_cast<float>(x * room.getGridSize()) + tileCenterOffset,
                        static_cast<float>(y * room.getGridSize()) + tileCenterOffset
                    ), 
                    Vector2D(tileHalfExtents, tileHalfExtents)
                );

                handler(tileAabb, room.getAttributeAt(x, y), dt, x, y);
            }
        }
    }

}