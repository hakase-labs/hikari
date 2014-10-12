#include "hikari/core/game/map/Room.hpp"
#include "hikari/core/game/map/Door.hpp"
#include "hikari/core/game/map/Force.hpp"
#include "hikari/core/game/map/Tileset.hpp"
#include "hikari/core/util/Log.hpp"
#include <memory>
#include <algorithm>
#include <list>

namespace hikari {

    const std::string Room::DEFAULT_BOSS_ENTITY_NAME = "None";

    Room::Room(int id, int x, int y, int width, int height, int gridSize, int backgroundColor,
            const Point2D<int> &heroSpawnPosition, const Rectangle2D<int> &cameraBounds,
            const std::vector<int> &tile, const std::vector<int> &attr,
            const std::vector<RoomTransition> &transitions,
            const std::vector<std::shared_ptr<Spawner>> &spawners,
            const std::vector<std::shared_ptr<Force>> &forces,
            const std::shared_ptr<Door> & entranceDoor,
            const std::shared_ptr<Door> & exitDoor,
            const std::string & bossEntityName
    )
        : id(id)
        , x(x)
        , y(y)
        , width(width)
        , height(height)
        , gridSize(gridSize)
        , backgroundColor(backgroundColor)
        , heroSpawnPosition(heroSpawnPosition)
        , bounds(x * gridSize, y * gridSize, width * gridSize, height * gridSize)
        , cameraBounds(cameraBounds)
        , tile(tile)
        , attr(attr)
        , transitions(transitions)
        , spawners(spawners)
        , forces(forces)
        , entranceDoor(entranceDoor)
        , exitDoor(exitDoor)
        , bossEntityName(bossEntityName)
    {
        traceLadders();
    }

    Room::~Room() {

    }

    void Room::traceLadders() {
        for(int x = 0; x < getWidth(); ++x) {
            bool topFound = false;
            bool bottomFound = false;
            int ladderTop = 0;
            int ladderX = 0;
            int ladderBottom = 0;

            for(int y = 0; y < getHeight(); ++y) {
                int attribute = attr[x + (y * getWidth())];

                if(TileAttribute::hasAttribute(attribute, TileAttribute::LADDER)) {
                    if(!topFound) {
                        topFound = true;
                        ladderTop = y;
                        ladderX = x;
                    }

                    // Find ladder at the bottom of the map
                    if(y == getHeight() - 1) {
                        if(topFound) {
                            bottomFound = true;
                            ladderBottom = y + 1;
                        }
                    }
                } else {
                    if(topFound) {
                        if(!bottomFound) {
                            bottomFound = true;
                            ladderBottom = y;
                        }
                    }
                }

                if(topFound && bottomFound) {
                    ladders.emplace_back(
                        BoundingBox<float>(
                            static_cast<float>(((getX() + ladderX) * getGridSize()) + 4  ), // ladder top left X (in pixels)
                            static_cast<float>((getY() + ladderTop) * getGridSize()      ), // ladder top left Y (in pixels)
                            static_cast<float>(getGridSize() - 4 - 4                     ), // ladder width      (in pixels)
                            static_cast<float>((ladderBottom - ladderTop) * getGridSize())  // ladder height     (in pixels)
                        )
                    );

                    HIKARI_LOG(debug4) << "Found ladder at " << ladders.back();

                    topFound = false;
                    bottomFound = false;
                    ladderX = 0;
                    ladderBottom = 0;
                    ladderTop = 0;
                }
            }
        }
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

    const int Room::getBackgroundColor() const {
        return backgroundColor;
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

    const std::string & Room::getBossEntity() const {
        return bossEntityName;
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

    const std::vector<std::shared_ptr<Force>>& Room::getForces() const {
        return forces;
    }

    const std::list<BoundingBox<float>> & Room::getLadders() const {
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

    const std::shared_ptr<Door> & Room::getEntranceDoor() {
        return entranceDoor;
    }

    const std::shared_ptr<Door> & Room::getExitDoor() {
        return exitDoor;
    }

} // hikari
