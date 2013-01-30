#ifndef HIKARI_CORE_GAME_MAP_MAPRENDERER
#define HIKARI_CORE_GAME_MAP_MAPRENDERER

#include "hikari/core/Platform.hpp"
#include "hikari/core/geom/Rectangle2D.hpp"
#include <memory>
#include <SFML/Graphics/Sprite.hpp>

#pragma warning(push)
#pragma warning(disable:4251)

namespace sf {
    class RenderTarget;
    class View;
}

namespace hikari {

    class Room;
    class Tileset;

    typedef std::shared_ptr<Room> RoomPtr;
    typedef std::shared_ptr<Tileset> TileDataPtr;

    class HIKARI_API MapRenderer {
    private:
        static const int TILE_OVERDRAW;
        RoomPtr room;
        TileDataPtr tileData;
        sf::Sprite tileSprite;
        Rectangle2D<int> visibleScreenArea;
        Rectangle2D<int> visibleTileArea;

        inline void cullTiles();
        inline void applyTileToSprite(const int &tileIndex, const int &x, const int &y);
        inline void applyTileAttributes(const int &attributes);

    public:
        MapRenderer(const RoomPtr &room, const TileDataPtr &tileData);
        
        void setRoom(const RoomPtr &room);
        RoomPtr getRoom() const;

        void setTileData(const TileDataPtr &tileData);
        TileDataPtr getTileData() const;

        void setCullRegion(const Rectangle2D<int> &cullRegion);

        void render(sf::RenderTarget &target);
    };

} // hikari

#pragma warning(pop)

#endif // HIKARI_CORE_GAME_MAP_MAPRENDERER