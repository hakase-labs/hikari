#include <hikari/core/game/map/Room.hpp>

namespace hikari {

    Room::Room(const int &id, const int &x, const int &y, 
        const int &width, const int &height, const int &gridSize, const Rectangle2D<int> &cameraBounds,
        const std::vector<int> &tile, const std::vector<int> &attr, const std::vector<RoomTransition> &transitions, const std::vector<std::shared_ptr<Spawner>> &spawners)
        : id(id)
        , x(x)
        , y(y)
        , width(width)
        , height(height)
        , gridSize(gridSize)
        , bounds(x * gridSize, y * gridSize, width * gridSize, height * gridSize)
        , cameraBounds(cameraBounds)
        , tile(tile)
        , attr(attr)
        , transitions(transitions)
        , collectableItems()
        , doodads()
        , enemies()
        , spawners(spawners) {

    }
    
    const int Room::getId() const {
        return id;
    }

    const int Room::getX() const {
        return x;
    }

    const int Room::getY() const {
        return y;
    }
    
    const int Room::getWidth() const {
        return width;
    }

    const int Room::getHeight() const {
        return height;
    }

    const int Room::getGridSize() const {
        return gridSize;
    }

    const Rectangle2D<int>& Room::getBounds() const {
        return bounds;
    }

    const Rectangle2D<int>& Room::getCameraBounds() const {
        return cameraBounds;
    }

    const int Room::getTileAt(int tileX, int tileY) const {
        // Make coordinates relative
        tileX = tileX - getX();
        tileY = tileY - getY();

        if(isInBounds(tileX, tileY)) {
            return tile[tileX + (tileY * getWidth())]; 
        }

        return NO_TILE;
    }

    const int Room::getAttributeAt(int tileX, int tileY) const {
        // Make coordinates relative
        tileX = tileX - getX();
        tileY = tileY - getY();

        if(isInBounds(tileX, tileY)) {
            return attr[tileX + (tileY * getWidth())]; 
        }

        return NO_TILE;
    }

    const inline bool Room::isInBounds(const int &x, const int &y) const {
        return ((x >= 0 && x < getWidth()) && (y >= 0 && y < getHeight()));
    }
    
    const std::vector<RoomTransition>& Room::getTransitions() const {
        return transitions;
    }

    const std::vector<std::shared_ptr<CollectableItem>>& Room::getCollectableItems() const {
        return collectableItems;
    }

    const std::vector<std::shared_ptr<Doodad>>& Room::getDoodads() const {
        return doodads;
    }

    const std::vector<std::shared_ptr<Enemy>>& Room::getEnemies() const {
        return enemies;
    }

    const std::vector<std::shared_ptr<Spawner>>& Room::getSpawners() const {
        return spawners;
    }

} // hikari