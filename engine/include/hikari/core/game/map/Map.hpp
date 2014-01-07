#ifndef HIKARI_CORE_GAME_MAP_MAP
#define HIKARI_CORE_GAME_MAP_MAP

#include "hikari/core/Platform.hpp"
#include "hikari/core/geom/Rectangle2D.hpp"

#include <memory>

#include <vector>

#if (_WIN32 && _MSC_VER)
    #pragma warning(push)
    #pragma warning(disable:4251)
#endif

namespace hikari {

    class Room;
    class Tileset;

    typedef std::shared_ptr<Room> RoomPtr;
    typedef std::shared_ptr<Tileset> TileDataPtr;

    /**
     * 
     */
    class HIKARI_API Map {
    private:
        int gridSize;
        std::string musicName;
        unsigned int startingRoomIndex;     // Index of the room in the current map where the hero "starts" from
        unsigned int midpointRoomIndex;     // Index of the room in the current map where the hero spawns from if reached
        unsigned int bossCorridorRoomIndex; // Index of the room where the hero spawns from if he reached the boss chamber
        unsigned int bossChamberRoomIndex;  // Index of the room where the hero fights the boss
        TileDataPtr tileset;
        std::vector<RoomPtr> rooms;
        std::vector< Rectangle2D<int> > roomRectangles; // stores room locations and dimensions in pixels

        /**
         * Constructs rectangles that represent the size and location of each
         * room in pixels. These rectangles are used to determine which room
         * a particular point lies in.
         */
        void constructRoomRects();

    public:
        static const Rectangle2D<int> INVALID_ROOM_RECT;

        Map(const TileDataPtr &tileset, const int &gridSize, const std::string & musicName, const std::vector<RoomPtr> &rooms, 
            unsigned int startRoomIndex = 0, unsigned int midpointRoomIndex = 0, unsigned int bossCorridorRoomIndex = 0, unsigned int bossChamberRoomIndex = 0);

        /**
         * Gets a pointer to the Tileset used by this Map.
         *
         * @return Tileset pointer
         */
        TileDataPtr getTileset() const;

        /**
         * Gets the size, in pixels, of the grid in this stage. The grid is 
         * always square.
         *
         * @return size of the grid
         */
        unsigned int getGridSize() const;

        /**
         * Gets the musicn ame of this stage. This is the music that is supposed
         * to be used when this stage is being played.
         *
         * @return a music name
         */
        const std::string & getMusicName() const;

        /**
         * Gets a pointer to the Room specified as the "starting" room.
         */
        RoomPtr getStartingRoom() const;

        /**
         * Gets a pointer to the Room specified as the "midpoint" room.
         */
        RoomPtr getMidpointRoom() const;

        /**
         * Gets a pointer to the Room specified as the "boss corridor" room,
         * which is the room before the boss chamber.
         */
        RoomPtr getBossCorridorRoom() const;

        /**
         * Gets a pointer to the Room specified as the "boss chamber" room,
         * which is the room where the hero fights the boss.
         */
        RoomPtr getBossChamberRoom() const;

        /**
         * Gets the number of rooms that exist in the map.
         */
        unsigned int getRoomCount() const;

       /**
        * Gets a pointer to the map data of the room with a specific index.
        * If no room can be found and the specified index, an empty pointer
        * is returned.
        * 
        * @param  index index of the room to get
        * @return       pointer to room at (index) or nullptr if not found
        */
        const RoomPtr getRoom(unsigned int index) const;

        /**
            Gets the rectangle which represents a given room in pixels in world
            space.

            @param index the index of the room
        */
        const Rectangle2D<int>& getRoomRect(unsigned int index) const;

        /**
            Finds the room which contains the location (x, y) and returns its
            id. If the location (x, y) is not within any of the rooms, -1 is
            returned.
        */
        unsigned int findRoomIndexByLocation(int x, int y) const;
    };

} // hikari

#if (_WIN32 && _MSC_VER)
    #pragma warning(pop)
#endif

#endif // HIKARI_CORE_GAME_MAP_MAP
