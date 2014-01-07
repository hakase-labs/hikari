#include "hikari/core/game/map/Map.hpp"
#include "hikari/core/game/map/Room.hpp"
#include "hikari/core/game/map/Tileset.hpp"

#include <algorithm>
#include <string>

namespace hikari {

    const Rectangle2D<int> Map::INVALID_ROOM_RECT = Rectangle2D<int>();

    Map::Map(const TileDataPtr &tileset, const int &gridSize, const std::string & musicName, const std::vector<RoomPtr> &rooms,
            unsigned int startingRoomIndex, unsigned int midpointRoomIndex, unsigned int bossCorridorRoomIndex, unsigned int bossChamberRoomIndex)
        : gridSize(gridSize)
        , musicName(musicName)
        , startingRoomIndex(startingRoomIndex)
        , midpointRoomIndex(midpointRoomIndex)
        , bossCorridorRoomIndex(bossCorridorRoomIndex)
        , bossChamberRoomIndex(bossChamberRoomIndex)
        , tileset(tileset)
        , rooms(rooms) 
    {
        constructRoomRects();
    }

    TileDataPtr Map::getTileset() const {
        return tileset;
    }

    unsigned int Map::getGridSize() const {
        return gridSize;
    }

    const std::string & Map::getMusicName() const {
        return musicName;
    }

    RoomPtr Map::getStartingRoom() const {
        return getRoom(startingRoomIndex);
    }

    RoomPtr Map::getMidpointRoom() const {
        return getRoom(midpointRoomIndex);
    }

    RoomPtr Map::getBossCorridorRoom() const {
        return getRoom(bossCorridorRoomIndex);
    }

    RoomPtr Map::getBossChamberRoom() const {
        return getRoom(bossChamberRoomIndex);
    }

    unsigned int Map::getRoomCount() const {
        return rooms.size();
    }

    const RoomPtr Map::getRoom(unsigned int index) const {
        if(index < getRoomCount()) {
            return rooms.at(index);
        }
        return RoomPtr();
    }

    const Rectangle2D<int>& Map::getRoomRect(unsigned int index) const {
        if(index < getRoomCount()) {
            return roomRectangles.at(index);
        }
        return INVALID_ROOM_RECT;
    }

    unsigned int Map::findRoomIndexByLocation(int x, int y) const {
        int index = 0;
        
        for(std::vector< Rectangle2D<int> >::const_iterator
            it = roomRectangles.begin(),
            end = roomRectangles.end();
            it < end;
            ++it, ++index)
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
            ++it)
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