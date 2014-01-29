#include "hikari/core/game/WorldCollisionResolver.hpp"
#include "hikari/core/game/CollisionInfo.hpp"
#include "hikari/core/game/Direction.hpp"
#include "hikari/core/game/map/Room.hpp"
#include "hikari/core/game/map/Tileset.hpp"
#include "hikari/client/game/GameWorld.hpp"
#include "hikari/client/game/objects/Enemy.hpp"
#include "hikari/core/util/Log.hpp"

namespace hikari {

    WorldCollisionResolver::WorldCollisionResolver()
        : world(nullptr)
        , tileBounds(0, 0, 0, 0)
    {
    }

    WorldCollisionResolver::~WorldCollisionResolver() {

    }

    void WorldCollisionResolver::setWorld(GameWorld * newWorld) {
        world = newWorld;

        if(world) {
            const auto currentRoom = world->getCurrentRoom();

            if(currentRoom) {
                const int tileSize = currentRoom->getGridSize();
        
                tileBounds
                    .setWidth(tileSize)
                    .setHeight(tileSize);
            } else {
                HIKARI_LOG(debug4) << "WorldCollisionResolver::setWorld set world but there is no current room.";
            }
        }
    }

    GameWorld * WorldCollisionResolver::getWorld() const {
        return world;
    }

    void WorldCollisionResolver::checkHorizontalEdge(const int& x, const int& yMin, const int& yMax, const Direction& directionX, CollisionInfo& collisionInfo) {
        sweepHorizontalEdge(x, yMin, yMax, directionX, collisionInfo);
    }

    void WorldCollisionResolver::checkVerticalEdge(const int& y, const int& xMin, const int& xMax, const Direction& directionY, CollisionInfo& collisionInfo) {
        sweepVerticalEdge(y, xMin, xMax, directionY, collisionInfo);
    }

    void WorldCollisionResolver::sweepHorizontalEdge(const int& x, const int& yMin, const int& yMax, const Direction& directionX, CollisionInfo& collisionInfo) {
        if(world) {
            const auto currentRoom = world->getCurrentRoom();
            if(currentRoom) {
                const int tileSize = currentRoom->getGridSize();
                const int tileX = x / tileSize;
                const int tileYMin = yMin / tileSize;
                const int tileYMax = yMax / tileSize;

                collisionInfo.isCollisionX = false;

                const auto obstacles = world->getObstacles();
                const std::size_t obstacleCount = obstacles.size();
                const BoundingBoxF sweepBox(x, yMin, 1.0f, yMax - yMin);

                for(std::size_t i = 0; i < obstacleCount; ++i) {
                    const auto & obstacleBounds = obstacles[i]->getBoundingBox();

                    if(sweepBox.intersects(obstacleBounds)) {
                        collisionInfo.isCollisionX = true;
                        collisionInfo.tileX = -1;
                        collisionInfo.tileY = -1;
                        collisionInfo.tileType = 0;
                        collisionInfo.directionX = directionX;

                        if(directionX == Directions::Left) {
                            collisionInfo.correctedX = obstacleBounds.getRight() + 1;
                        } else if(directionX == Directions::Right) {
                            collisionInfo.correctedX = obstacleBounds.getLeft();
                        }
                    }
                }

                for(int tileY = tileYMin; tileY <= tileYMax; tileY++) {
                    const int currentTile = currentRoom->getAttributeAt(tileX, tileY);

                    if(tileIsSolid(currentTile)) {
                        collisionInfo.isCollisionX = true;
                        collisionInfo.tileX = tileX;
                        collisionInfo.tileY = tileY;
                        collisionInfo.tileType = currentTile;
                        collisionInfo.directionX = directionX;
                
                        determineTileCorrection(directionX, collisionInfo);

                        return;
                    }
                }
            }
        }
        return;
    }

    void WorldCollisionResolver::sweepVerticalEdge(const int& y, const int& xMin, const int& xMax, const Direction& directionY, CollisionInfo& collisionInfo) {
        if(world) {
            const auto currentRoom = world->getCurrentRoom();
            if(currentRoom) {
                const int tileSize = currentRoom->getGridSize();
                const int tileY = y / tileSize;
                const int tileXMin = xMin / tileSize;
                const int tileXMax = xMax / tileSize;

                collisionInfo.isCollisionY = false;

                const auto obstacles = world->getObstacles();
                const std::size_t obstacleCount = obstacles.size();
                const BoundingBoxF sweepBox(xMin, y, xMax - xMin, 1.0f);

                for(std::size_t i = 0; i < obstacleCount; ++i) {
                    const auto & obstacleBounds = obstacles[i]->getBoundingBox();

                    if(sweepBox.intersects(obstacleBounds)) {
                        collisionInfo.isCollisionY = true;
                        collisionInfo.tileX = -1;
                        collisionInfo.tileY = -1;
                        collisionInfo.tileType = 0;
                        collisionInfo.directionY = directionY;

                        if(directionY == Directions::Up) {
                            collisionInfo.correctedY = obstacleBounds.getBottom();
                        } else if(directionY == Directions::Down) {
                            collisionInfo.correctedY = obstacleBounds.getTop();
                        }
                    }
                }

                for(int tileX = tileXMin; tileX <= tileXMax; tileX++) {
                    const int currentTile = currentRoom->getAttributeAt(tileX, tileY);

                    // Here we check for ladder tops as well as solid ground.
                    // Ladder tops are only "solid" if you're falling "down" on them.
                    if(tileIsSolid(currentTile) || (collisionInfo.treatPlatformAsGround && tileIsPlatform(currentTile) && directionY == Directions::Down)) {
                        collisionInfo.isCollisionY = true;
                        collisionInfo.tileX = tileX;
                        collisionInfo.tileY = tileY;
                        collisionInfo.tileType = currentTile;
                        collisionInfo.directionY = directionY;

                        determineTileCorrection(directionY, collisionInfo);

                        return;
                    }
                }
            }
        }
        return;
    }

    void WorldCollisionResolver::determineTileCorrection(const Direction& direction, CollisionInfo& collisionInfo) {
        if(world) {
            const auto currentRoom = world->getCurrentRoom();
            if(currentRoom) {
                const int tileSize = currentRoom->getGridSize();

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

    bool WorldCollisionResolver::tileIsSolid(const int& tileAttribute) const {
        return (tileAttribute != Room::NO_TILE) && TileAttribute::hasAttribute(tileAttribute, TileAttribute::SOLID);
    }

    bool WorldCollisionResolver::tileIsPlatform(const int& tileAttribute) const {
        return (tileAttribute != Room::NO_TILE) && TileAttribute::hasAttribute(tileAttribute, TileAttribute::PLATFORM);
    }

} // hikari