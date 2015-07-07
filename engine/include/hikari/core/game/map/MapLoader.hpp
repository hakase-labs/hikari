#ifndef HIKARI_CORE_GAME_MAP_MAPLOADER
#define HIKARI_CORE_GAME_MAP_MAPLOADER

#include "hikari/core/Platform.hpp"
#include "hikari/core/geom/Rectangle2D.hpp"
#include "hikari/core/util/Service.hpp"
#include "hikari/core/util/TilesetCache.hpp"

#include <memory>
#include <json/value.h>
#include <string>

namespace hikari {

    class AnimationSetCache;
    class ImageCache;
    class TilesetCache;
    class Map;
    class Door;
    class Room;
    class RoomTransition;
    class BlockSequenceDescriptor;
    class Spawner;
    class Force;

    typedef std::shared_ptr<Map> MapPtr;
    typedef std::shared_ptr<Room> RoomPtr;
    typedef std::shared_ptr<Spawner> SpawnerPtr;
    typedef std::shared_ptr<Force> ForcePtr;

    class HIKARI_API MapLoader : public Service {
    private:
        static const char* PROP_TILESET;
        static const char* PROP_ROOMS;
        static const char* PROP_GRIDSIZE;
        static const char* PROP_MUSICID;
        static const char* PROP_MUSICNAME;
        static const char* PROP_SPECIAL_ROOMS;
        static const char* PROP_SPECIAL_ROOM_STARTING;
        static const char* PROP_SPECIAL_ROOM_MIDPOINT;
        static const char* PROP_SPECIAL_ROOM_BOSS_CORRIDOR;
        static const char* PROP_SPECIAL_ROOM_BOSS_CHAMBER;
        static const char* PROP_BOSS_ENTITY;
        static const char* PROP_ROOM_ID;
        static const char* PROP_ROOM_X;
        static const char* PROP_ROOM_Y;
        static const char* PROP_ROOM_HERO_SPAWN_X;
        static const char* PROP_ROOM_HERO_SPAWN_Y;
        static const char* PROP_ROOM_WIDTH;
        static const char* PROP_ROOM_HEIGHT;
        static const char* PROP_ROOM_BG_COLOR;
        static const char* PROP_ROOM_CAMERABOUNDS;
        static const char* PROP_ROOM_CAMERABOUNDS_X;
        static const char* PROP_ROOM_CAMERABOUNDS_Y;
        static const char* PROP_ROOM_CAMERABOUNDS_WIDTH;
        static const char* PROP_ROOM_CAMERABOUNDS_HEIGHT;
        static const char* PROP_ROOM_TILE;
        static const char* PROP_ROOM_TILEATTRIBUTES;
        static const char* PROP_ROOM_ENEMIES;
        static const char* PROP_ROOM_ENEMIES_TYPE;
        static const char* PROP_ROOM_ENEMIES_POSITION;
        static const char* PROP_ROOM_ENEMIES_POSITION_X;
        static const char* PROP_ROOM_ENEMIES_POSITION_Y;
        static const char* PROP_ROOM_ENEMIES_DIRECTION;
        static const char* PROP_ROOM_ITEMS;
        static const char* PROP_ROOM_ITEMS_TYPE;
        static const char* PROP_ROOM_ITEMS_X;
        static const char* PROP_ROOM_ITEMS_Y;
        static const char* PROP_ROOM_TRANSITIONS;
        static const char* PROP_ROOM_DOORS;
        static const char* PROP_ROOM_DOORS_X;
        static const char* PROP_ROOM_DOORS_Y;
        static const char* PROP_ROOM_DOORS_WIDTH;
        static const char* PROP_ROOM_DOORS_HEIGHT;
        static const char* PROP_ROOM_DOORS_ENTRANCE;
        static const char* PROP_ROOM_DOORS_EXIT;
        static const char* PROP_ROOM_BLOCKSEQUENCES;
        static const char* PROP_ROOM_BLOCKSEQUENCES_X;
        static const char* PROP_ROOM_BLOCKSEQUENCES_Y;
        static const char* PROP_ROOM_BLOCKSEQUENCES_WIDTH;
        static const char* PROP_ROOM_BLOCKSEQUENCES_HEIGHT;
        static const char* PROP_ROOM_BLOCKSEQUENCES_BLOCKS;
        static const char* PROP_ROOM_BLOCKSEQUENCES_TIMING;
        static const char* PROP_ROOM_BLOCKSEQUENCES_AT;
        static const char* PROP_ROOM_BLOCKSEQUENCES_ON;
        static const char* PROP_ROOM_BLOCKSEQUENCES_OFF;

        static const int DEFAULT_HERO_SPAWN_X;
        static const int DEFAULT_HERO_SPAWN_Y;

        enum SpawnType {
            SPAWN_ENEMY = 1,
            SPAWN_ITEM  = 2
        };

        std::shared_ptr<AnimationSetCache> animationSetCache;
        std::shared_ptr<ImageCache> imageCache;
        std::shared_ptr<TilesetCache> tilesetCache;

        MapPtr constructMap(const Json::Value &json) const;
        RoomPtr constructRoom(const Json::Value &json, int gridSize) const;
        SpawnerPtr constructSpawner(const Json::Value &json, SpawnType type, int offsetX = 0, int offsetY = 0) const;
        ForcePtr constructForce(const Json::Value &json, int offsetX = 0, int offsetY = 0) const;
        std::unique_ptr<Door> constructDoor(const Json::Value & json, int offsetX = 0, int offsetY = 0) const;
        RoomTransition constructTransition(const Json::Value &json) const;
        BlockSequenceDescriptor constructBlockSequence(const Json::Value &json,
            int roomX, int roomY, int gridSize) const;
        Rectangle2D<int> constructCameraBounds(const Json::Value &json,
            int roomX, int roomY, int gridSize) const;
        bool validateMapStructure(const Json::Value &json) const;
        bool validateRoomStructure(const Json::Value &json) const;

    public:
        MapLoader(const std::shared_ptr<AnimationSetCache> & animationSetCache,
            const std::shared_ptr<ImageCache> & imageCache,
            const std::shared_ptr<TilesetCache> &tilesetCache
        );
        virtual ~MapLoader();
        MapPtr loadFromJson(const Json::Value &json) const;
    };

} // hikari

#endif // HIKARI_CORE_GAME_MAP_MAPLOADER
