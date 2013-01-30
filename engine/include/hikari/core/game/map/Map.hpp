#ifndef HIKARI_CORE_GAME_MAP_MAP
#define HIKARI_CORE_GAME_MAP_MAP

#include "hikari/core/Platform.hpp"
#include "hikari/core/geom/Rectangle2D.hpp"

#include <memory>

#include <vector>

#pragma warning(push)
#pragma warning(disable:4251)

namespace hikari {

    class Room;
    class Tileset;

    typedef std::shared_ptr<Room> RoomPtr;
    typedef std::shared_ptr<Tileset> TileDataPtr;

    class HIKARI_API Map {
    private:
        int gridSize;
        TileDataPtr tileset;
        std::vector<RoomPtr> rooms;
        std::vector< Rectangle2D<int> > roomRectangles; // stores room locations and dimensions in pixels
        
        /**
            Constructs rectangles that represent the size and location of each
            room in pixels. These rectangles are used to determine which room
            a particular point lies in.
        */
        void constructRoomRects();

    public:
        static const Rectangle2D<int> INVALID_ROOM_RECT;

        Map(const TileDataPtr &tileset, const int &gridSize, const std::vector<RoomPtr> &rooms);

        const TileDataPtr& getTileset() const;
        
        /**
            Gets the size of the grid this map uses in pixels.
        */
        const int getGridSize() const;

        /**
            Gets the number of rooms that exist in the map.
        */
        const int getRoomCount() const;
        
        /**
            Gets a pointer to the map data of the room with a specific index. 
            If no room can be found and the specified index, an empty pointer
            is returned.
            
            @param index the index of the room
        */
        const RoomPtr getRoom(int index) const;

        /**
            Gets the rectangle which represents a given room in pixels in world
            space.

            @param index the index of the room
        */
        const Rectangle2D<int>& getRoomRect(int index) const;
        
        /**
            Finds the room which contains the location (x, y) and returns its 
            id. If the location (x, y) is not within any of the rooms, -1 is 
            returned.
        */
        const int getRoomIndexByLocation(int x, int y) const;
    };

} // hikari

#pragma warning(pop)

#endif // HIKARI_CORE_GAME_MAP_MAP