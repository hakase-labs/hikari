#include "hikari/core/game/TileMapCollisionResolver.hpp"
#include "hikari/core/game/CollisionInfo.hpp"
#include "hikari/core/game/Direction.hpp"
#include "hikari/core/game/map/Room.hpp"
#include "hikari/core/game/map/Tileset.hpp"

namespace hikari {

    TileMapCollisionResolver::TileMapCollisionResolver()
        : map()
        , tileBounds(0, 0, 0, 0)
    {
    }

    TileMapCollisionResolver::~TileMapCollisionResolver() {

    }

    void TileMapCollisionResolver::setRoom(std::weak_ptr<Room> newRoom) {
        map = newRoom;

        if(!map.expired()) {
            auto mapPtr = map.lock();
            if(mapPtr) {
                const int tileSize = mapPtr->getGridSize();
            
                tileBounds
                    .setWidth(tileSize)
                    .setHeight(tileSize);
            }
        }
    }

    std::weak_ptr<Room> TileMapCollisionResolver::getRoom() const {
        return map;
    }

    void TileMapCollisionResolver::checkHorizontalEdge(const int& x, const int& yMin, const int& yMax, const Direction& directionX, CollisionInfo& collisionInfo) {
        sweepHorizontalEdge(x, yMin, yMax, directionX, collisionInfo);
    }

    void TileMapCollisionResolver::checkVerticalEdge(const int& y, const int& xMin, const int& xMax, const Direction& directionY, CollisionInfo& collisionInfo) {
        sweepVerticalEdge(y, xMin, xMax, directionY, collisionInfo);
    }

    void TileMapCollisionResolver::sweepHorizontalEdge(const int& x, const int& yMin, const int& yMax, const Direction& directionX, CollisionInfo& collisionInfo) {
        if(!map.expired()) {
            auto mapPtr = map.lock();
            if(mapPtr) {
                const int tileX = x / mapPtr->getGridSize();
                const int tileYMin = yMin / mapPtr->getGridSize();
                const int tileYMax = yMax / mapPtr->getGridSize();

                collisionInfo.isCollisionX = false;

                for(int tileY = tileYMin; tileY <= tileYMax; tileY++) {
                    const int currentTile = mapPtr->getAttributeAt(tileX, tileY);

                    if(tileIsSolid(currentTile)) {
                        collisionInfo.isCollisionX = true;
                        collisionInfo.tileX = tileX;
                        collisionInfo.tileY = tileY;
                        collisionInfo.tileType = currentTile;
                        collisionInfo.directionX = directionX;
                
                        determineCorrection(directionX, collisionInfo);

                        return;
                    }
                }
            }
        }
        return;
    }

    void TileMapCollisionResolver::sweepVerticalEdge(const int& y, const int& xMin, const int& xMax, const Direction& directionY, CollisionInfo& collisionInfo) {
        if(!map.expired()) {
            auto mapPtr = map.lock();
            if(mapPtr) {
                const int tileY = y / mapPtr->getGridSize();
                const int tileXMin = xMin / mapPtr->getGridSize();
                const int tileXMax = xMax / mapPtr->getGridSize();

                collisionInfo.isCollisionY = false;

                for(int tileX = tileXMin; tileX <= tileXMax; tileX++) {
                    const int currentTile = mapPtr->getAttributeAt(tileX, tileY);

                    // Here we check for ladder tops as well as solid ground.
                    // Ladder tops are only "solid" if you're falling "down" on them.
                    if(tileIsSolid(currentTile) || (collisionInfo.treatLadderTopAsGround && tileIsLadderTop(currentTile) && directionY == Directions::Down)) {
                        collisionInfo.isCollisionY = true;
                        collisionInfo.tileX = tileX;
                        collisionInfo.tileY = tileY;
                        collisionInfo.tileType = currentTile;
                        collisionInfo.directionY = directionY;

                        determineCorrection(directionY, collisionInfo);

                        return;
                    }
                }
            }
        }
        return;
    }

    void TileMapCollisionResolver::determineCorrection(const Direction& direction, CollisionInfo& collisionInfo) {
         if(!map.expired()) {
            auto mapPtr = map.lock();
            if(mapPtr) {
                const int tileSize = mapPtr->getGridSize();

                tileBounds
                    .setPosition(collisionInfo.tileX * tileSize, collisionInfo.tileY * tileSize);

                if(direction == Directions::Left) {
                    // Collision happened on the left edge, so return the X for the RIGHT side of the tile.
                    collisionInfo.correctedX = tileBounds.getRight();
                } else if(direction == Directions::Right) {
                    // Collision happened on the right edge, so just set the X position.
                    collisionInfo.correctedX = tileBounds.getLeft();
                } else if(direction == Directions::Up) {
                    // Collision happened on the bottom edge, to just set the Y position?
                    collisionInfo.correctedY = tileBounds.getBottom();
                } else if(direction == Directions::Down) {
                    // Collision happened on the top edge, so take height into consideration?
                    collisionInfo.correctedY = tileBounds.getTop();
                }
            }
         }
    }

    bool TileMapCollisionResolver::tileIsSolid(const int& tileAttribute) const {
        return (tileAttribute != Room::NO_TILE) && TileAttribute::hasAttribute(tileAttribute, TileAttribute::SOLID);
    }

    bool TileMapCollisionResolver::tileIsLadderTop(const int& tileAttribute) const {
        return (tileAttribute != Room::NO_TILE) && TileAttribute::hasAttribute(tileAttribute, TileAttribute::LADDER_TOP);
    }

} // hikari