#include "hikari/core/game/map/Room.hpp"
#include "hikari/core/game/map/Tileset.hpp"
#include "hikari/core/util/Log.hpp"
#include <memory>
#include <algorithm>
#include <list>

namespace hikari {

    Room::Room(const int &id, const int &x, const int &y, 
        const int &width, const int &height, const int &gridSize, const Point2D<int> &heroSpawnPosition, const Rectangle2D<int> &cameraBounds,
        const std::vector<int> &tile, const std::vector<int> &attr, const std::vector<RoomTransition> &transitions, const std::vector<std::shared_ptr<Spawner>> &spawners)
        : id(id)
        , x(x)
        , y(y)
        , width(width)
        , height(height)
        , gridSize(gridSize)
        , heroSpawnPosition(heroSpawnPosition)
        , bounds(x * gridSize, y * gridSize, width * gridSize, height * gridSize)
        , cameraBounds(cameraBounds)
        , tile(tile)
        , attr(attr)
        , transitions(transitions)
        , spawners(spawners)
    {
        traceLadders();
    }

    void Room::traceLadders() {
        std::list<std::pair<int, int>> ladderTops;

        for(int y = 0; y < getHeight(); ++y) {
            for(int x = 0; x < getWidth(); ++x) {
                int attribute = attr[x + (y * getWidth())];

                if(TileAttribute::hasAttribute(attribute, TileAttribute::LADDER_TOP)) {
                    ladderTops.emplace_back(std::pair<int, int>(x, y));
                }
            }
        }

        std::for_each(std::begin(ladderTops), std::end(ladderTops), [this](const std::pair<int, int> & top) {
            int bottom = top.second;

            // Trace the ladder down vertically until you find a tile that is 
            // not a ladder.
            while(bottom < getHeight()) {
                int attribute = attr[top.first + (bottom * getWidth())];

                if(!TileAttribute::hasAttribute(attribute, TileAttribute::LADDER)) {
                    break;
                }

                bottom++;
            }

            ladders.emplace_back(
                BoundingBox<int>(
                    (getX() + top.first)  * getGridSize(), // ladder top left X (in pixels)
                    (getY() + top.second) * getGridSize(), // ladder top left Y (in pixels)
                    getGridSize(),                         // ladder width      (in pixels)
                    (bottom - top.second) * getGridSize()  // ladder height     (in pixels)
                )
            );

            HIKARI_LOG(debug4) << "Found bottom top at " << ladders.back();
        });
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

    const Point2D<int>& Room::getHeroSpawnPosition() const {
        return heroSpawnPosition;
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

    const std::vector<std::shared_ptr<Spawner>>& Room::getSpawners() const {
        return spawners;
    }

    const std::list<BoundingBox<int>> & Room::getLadders() const {
        return ladders;
    }

    std::vector<std::weak_ptr<Spawner>> Room::getSpawnerList() const {
        std::vector<std::weak_ptr<Spawner>> output;
        output.resize(spawners.size());

        std::transform(std::begin(spawners), std::end(spawners), std::begin(output), [&](std::shared_ptr<Spawner> spawner) -> std::weak_ptr<Spawner> {
            return std::weak_ptr<Spawner>(spawner);
        });

        return output;
    }

} // hikari