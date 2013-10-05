#ifndef HIKARI_CORE_GAME_MAP_ROOM
#define HIKARI_CORE_GAME_MAP_ROOM

#include "hikari/core/Platform.hpp"
#include "hikari/core/game/map/RoomTransition.hpp"
#include "hikari/core/geom/BoundingBox.hpp"
#include "hikari/core/geom/Point2D.hpp"
#include "hikari/core/geom/Rectangle2D.hpp"
#include <list>
#include <memory>
#include <vector>

#if (_WIN32 && _MSC_VER)
    #pragma warning(push)
    #pragma warning(disable:4251)
#endif

namespace hikari {

    class Spawner;
    class Door;

    class HIKARI_API Room {
    private:
        int id;
        int x;
        int y;
        int width;
        int height;
        int gridSize;
        Point2D<int> heroSpawnPosition;
        Rectangle2D<int> bounds;
        Rectangle2D<int> cameraBounds;
        std::vector<int> tile;
        std::vector<int> attr;
        std::vector<RoomTransition> transitions;
        std::vector<std::shared_ptr<Spawner>> spawners;
        std::list<BoundingBox<float>> ladders;
        std::shared_ptr<Door> entranceDoor;
        std::shared_ptr<Door> exitDoor;

        const inline bool isInBounds(const int &x, const int &y) const;

        /**
         * Iterates through the attribute data searching for ladders and traces
         * their region into BoundingBox instances.
         */
        void traceLadders();

    public:
        const static int NO_TILE = -1;

        Room(int id, int x, int y, int width, int height, int gridSize, const Point2D<int> &heroSpawnPosition, const Rectangle2D<int> &cameraBounds,
            const std::vector<int> &tile, const std::vector<int> &attr, const std::vector<RoomTransition> &transitions, const std::vector<std::shared_ptr<Spawner>> &spawners,
            const std::shared_ptr<Door> & entranceDoor, const std::shared_ptr<Door> & exitDoor);

        ~Room();

        const int getId() const;
        const int getX() const;
        const int getY() const;
        const int getWidth() const;
        const int getHeight() const;
        const int getGridSize() const;
        const Rectangle2D<int>& getBounds() const;
        const Rectangle2D<int>& getCameraBounds() const;

        /**
         * Gets a reference to the point within the room where the hero should
         * be spawned. 
         * 
         * This is typically used if the stage round starts in this room and the
         * player needs to be spawned somewhere. This poisition is where Rock 
         * will teleport down to when a round starts.
         *
         * @return point where the hero should spawn in this room
         */
        const Point2D<int>& getHeroSpawnPosition() const;

        /**
         * Gets the tile located at the position (tileX, tileY). If the
         * position is outside of the bounds of the map, -1 is returned.
         *
         * All positions are relative to the top left corner of the map data,
         * where the top left corner is defined as (0, 0).
         * 
         * @param  tileX the x-coordinate of the tile to get
         * @param  tileY tileY the y-coordinate of the tile to get
         * @return       tile found at (tileX, tileY)
         */
        const int getTileAt(int tileX, int tileY) const;

        /**
         * Gets the attribute located at the position (attrX, attrY). If the
         * position is outside of the bounds of the map, -1 is returned.
         * 
         * All positions are relative to the top left corner of the map data,
         * where the top left corner is defined as (0, 0).
         * 
         * @param attrX the x-coordinate of the attribute to get
         * @param attrY the y-coordinate of the attribute to get
         * @return      attribute flags found at (tileX, tileY)
        */
        const int getAttributeAt(int tileX, int tileY) const;

        /**
         * Gets a reference to the list of the room's transitions.
         *
         * @see hikari::RoomTransition
         * @return reference to list of RoomTransitions
         */
        const std::vector<RoomTransition>& getTransitions() const;

        /**
         * Gets a reference to the list of shared pointers to the Spawners in 
         * this room.
         *
         * @see hikari::Spawner
         * @return vector of smart pointers to all Spawners
         */
        const std::vector<std::shared_ptr<Spawner>>& getSpawners() const;

        /**
         * Gets a reference to the list of rectangles representing the ladders
         * in the room.
         *
         * @return list of ladder rectangles
         */
        const std::list<BoundingBox<float>> & getLadders() const;

        /**
         * Gets a list of weak pointers to the Spawner objects in this Room.
         *
         * @see hikari::Spawner
         * @return list of Spawners
         */
        std::vector<std::weak_ptr<Spawner>> getSpawnerList() const;

        /**
         * Gets the room's entrance door, if it has one. If the room has no
         * entrance door then the returned pointer will be a nullptr.
         *
         * @see getExit
         * @return pointer to entrance Door or nullptr
         */
        const std::shared_ptr<Door> & getEntranceDoor();

        /**
         * Gets the room's exit door, if it has one. If the room has no exit 
         * door then the returned pointer will be a nullptr.
         *
         * @see getEntrance
         * @return pointer to exit Door or nullptr
         */
        const std::shared_ptr<Door> & getExitDoor();
    };

} // hikari

#if (_WIN32 && _MSC_VER)
    #pragma warning(pop)
#endif

#endif // HIKARI_CORE_GAME_MAP_ROOM
