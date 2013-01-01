#include <hikari/core/game/map/Map.hpp>
#include <hikari/core/game/map/Room.hpp>
#include <hikari/core/game/map/Tileset.hpp>

namespace hikari {

    const Rectangle2D<int> Map::INVALID_ROOM_RECT = Rectangle2D<int>();

    Map::Map(const TileDataPtr &tileset, const int &gridSize, const std::vector<RoomPtr> &rooms)
        : gridSize(gridSize)
        , tileset(tileset)
        , rooms(rooms) 
    {
        constructRoomRects();
    }

    const TileDataPtr& Map::getTileset() const {
        return tileset;
    }

    const int Map::getGridSize() const {
        return gridSize;
    }

    const int Map::getRoomCount() const {
        return rooms.size();
    }

    const RoomPtr Map::getRoom(int index) const {
        if(index >= 0 && index < getRoomCount()) {
            return rooms.at(index);
        }
        return RoomPtr();
    }

    const Rectangle2D<int>& Map::getRoomRect(int index) const {
        if(index >= 0 && index < getRoomCount()) {
            return roomRectangles.at(index);
        }
        return INVALID_ROOM_RECT;
    }

    const int Map::getRoomIndexByLocation(int x, int y) const {
        int index = 0;
        for(std::vector< Rectangle2D<int> >::const_iterator
            it = roomRectangles.begin(),
            end = roomRectangles.end();
            it < end;
            it++, index++)
        {
            if((*it).contains(x, y)) {
                return index;
            }
        }
        return -1;
    }

    void Map::constructRoomRects() {
        roomRectangles.clear();

        for(std::vector<RoomPtr>::const_iterator 
            it = rooms.begin(),
            end = rooms.end();
            it < end;
            it++)
        {
            const RoomPtr& thisRoom = (*it);

            Rectangle2D<int> thisRoomRectangle(
                thisRoom->getX() * gridSize,
                thisRoom->getY() * gridSize,
                thisRoom->getWidth() * gridSize,
                thisRoom->getHeight() * gridSize
            );

            roomRectangles.push_back(thisRoomRectangle);
        }
    }

} // hikari