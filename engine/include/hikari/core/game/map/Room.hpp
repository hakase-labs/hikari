#ifndef HIKARI_CORE_GAME_MAP_ROOM
#define HIKARI_CORE_GAME_MAP_ROOM

#include "hikari/core/Platform.hpp"
#include "hikari/core/game/map/RoomTransition.hpp"
#include "hikari/core/geom/Point2D.hpp"
#include "hikari/core/geom/Rectangle2D.hpp"
#include <memory>
#include <vector>

#if (_WIN32 && _MSC_VER)
    #pragma warning(push)
    #pragma warning(disable:4251)
#endif

namespace hikari {

    class CollectableItem;
    class Doodad;
    class Enemy;
    class Spawner;

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
        std::vector<std::shared_ptr<CollectableItem>> collectableItems;
        std::vector<std::shared_ptr<Doodad>> doodads;
        std::vector<std::shared_ptr<Enemy>> enemies;
        std::vector<std::shared_ptr<Spawner>> spawners;

        const inline bool isInBounds(const int &x, const int &y) const;

    public:
        const static int NO_TILE = -1;

        Room(const int &id, const int &x, const int &y, const int &width, const int &height, const int &gridSize, const Point2D<int> &heroSpawnPosition, const Rectangle2D<int> &cameraBounds,
            const std::vector<int> &tile, const std::vector<int> &attr, const std::vector<RoomTransition> &transitions, const std::vector<std::shared_ptr<Spawner>> &spawners);

        const int getId() const;
        const int getX() const;
        const int getY() const;
        const int getWidth() const;
        const int getHeight() const;
        const int getGridSize() const;
        const Rectangle2D<int>& getBounds() const;
        const Rectangle2D<int>& getCameraBounds() const;
        const Point2D<int>& getHeroSpawnPosition() const;

        /**
            Gets the tile located at the position (tileX, tileY). If the
            position is outside of the bounds of the map, -1 is returned.

            All positions are relative to the top left corner of the map data,
            where the top left corner is defined as (0, 0).

            @param tileX the x-coordinate of the tile to get
            @param tileY the y-coordinate of the tile to get
        */
        const int getTileAt(int tileX, int tileY) const;

        /**
            Gets the attribute located at the position (attrX, attrY). If the
            position is outside of the bounds of the map, -1 is returned.

            All positions are relative to the top left corner of the map data,
            where the top left corner is defined as (0, 0).

            @param attrX the x-coordinate of the attribute to get
            @param attrY the y-coordinate of the attribute to get
        */
        const int getAttributeAt(int tileX, int tileY) const;

        /**
            Gets the list of the room's transitions.
        */
        const std::vector<RoomTransition>& getTransitions() const;

        const std::vector<std::shared_ptr<CollectableItem>>& getCollectableItems() const;
        const std::vector<std::shared_ptr<Doodad>>& getDoodads() const;
        const std::vector<std::shared_ptr<Enemy>>& getEnemies() const;
        const std::vector<std::shared_ptr<Spawner>>& getSpawners() const;
    };

} // hikari

#if (_WIN32 && _MSC_VER)
    #pragma warning(pop)
#endif

#endif // HIKARI_CORE_GAME_MAP_ROOM
