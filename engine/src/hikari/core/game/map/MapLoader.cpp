#include "hikari/core/game/map/MapLoader.hpp"
#include "hikari/core/game/map/Map.hpp"
#include "hikari/core/game/map/Room.hpp"
#include "hikari/core/game/map/RoomTransition.hpp"
#include "hikari/core/game/map/Tileset.hpp"
#include "hikari/client/game/objects/Spawner.hpp"
#include "hikari/core/util/StringUtils.hpp"
#include "hikari/core/util/TilesetCache.hpp"
#include "hikari/core/util/PhysFS.hpp"
#include "hikari/core/util/Log.hpp"
#include <json/reader.h>
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <string>
#include <vector>

namespace hikari {

    const char* MapLoader::PROPERTY_NAME_TILESET = "tileset";
    const char* MapLoader::PROPERTY_NAME_GRIDSIZE = "gridsize";
    const char* MapLoader::PROPERTY_NAME_ROOMS = "rooms";
    const char* MapLoader::PROPERTY_NAME_ROOM_ID = "id";
    const char* MapLoader::PROPERTY_NAME_ROOM_X = "x";
    const char* MapLoader::PROPERTY_NAME_ROOM_Y = "y";
    const char* MapLoader::PROPERTY_NAME_ROOM_WIDTH = "width";
    const char* MapLoader::PROPERTY_NAME_ROOM_HEIGHT = "height";
    const char* MapLoader::PROPERTY_NAME_ROOM_CAMERABOUNDS = "cameraBounds";
    const char* MapLoader::PROPERTY_NAME_ROOM_CAMERABOUNDS_X = "x";
    const char* MapLoader::PROPERTY_NAME_ROOM_CAMERABOUNDS_Y = "y";
    const char* MapLoader::PROPERTY_NAME_ROOM_CAMERABOUNDS_WIDTH = "width";
    const char* MapLoader::PROPERTY_NAME_ROOM_CAMERABOUNDS_HEIGHT = "height";
    const char* MapLoader::PROPERTY_NAME_ROOM_TILE = "tile";
    const char* MapLoader::PROPERTY_NAME_ROOM_TILEATTRIBUTES = "attr";
    const char* MapLoader::PROPERTY_NAME_ROOM_ENEMIES = "enemies";
    const char* MapLoader::PROPERTY_NAME_ROOM_ENEMIES_TYPE = "type";
    const char* MapLoader::PROPERTY_NAME_ROOM_ENEMIES_POSITION = "position";
    const char* MapLoader::PROPERTY_NAME_ROOM_ENEMIES_POSITION_X = "x";
    const char* MapLoader::PROPERTY_NAME_ROOM_ENEMIES_POSITION_Y = "y";
    const char* MapLoader::PROPERTY_NAME_ROOM_ENEMIES_DIRECTION = "direction";
    const char* MapLoader::PROPERTY_NAME_ROOM_ITEMS = "items";
    const char* MapLoader::PROPERTY_NAME_ROOM_ITEMS_TYPE = "type";
    const char* MapLoader::PROPERTY_NAME_ROOM_ITEMS_X = "x";
    const char* MapLoader::PROPERTY_NAME_ROOM_ITEMS_Y = "y";
    const char* MapLoader::PROPERTY_NAME_ROOM_TRANSITIONS = "transitions";

    MapLoader::MapLoader(const std::shared_ptr<TilesetCache> &tilesetCache)
        : tilesetCache(tilesetCache) {

    }

    MapLoader::~MapLoader() { }

    MapPtr MapLoader::loadFromJson(const Json::Value &json) const {
        validateMapStructure(json);
        return constructMap(json);
    }

    MapPtr MapLoader::constructMap(const Json::Value &json) const {
        // Parse map attributes
        std::string tilesetName = json[PROPERTY_NAME_TILESET].asString();
        int gridSize = json[PROPERTY_NAME_GRIDSIZE].asInt();
        int roomCount = json[PROPERTY_NAME_ROOMS].size();

        TileDataPtr tileset = nullptr;

        try {
            tileset = tilesetCache->get(tilesetName);
        } catch(std::runtime_error err) {
            HIKARI_LOG(fatal) << "Couldn't load a tileset while constructing a map.";
        }

        // Parse rooms
        std::vector<RoomPtr> rooms;
        for(int i = 0; i < roomCount; ++i) {
            rooms.push_back(constructRoom(json[PROPERTY_NAME_ROOMS][i], gridSize));
        }

        return MapPtr(new Map(tileset, gridSize, rooms));
    }

    RoomPtr MapLoader::constructRoom(const Json::Value &json, const int &gridSize) const {
        int id              = json[PROPERTY_NAME_ROOM_ID].asInt();
        int x               = json[PROPERTY_NAME_ROOM_X].asInt();
        int y               = json[PROPERTY_NAME_ROOM_Y].asInt();
        int width           = json[PROPERTY_NAME_ROOM_WIDTH].asInt();
        int height          = json[PROPERTY_NAME_ROOM_HEIGHT].asInt();
        int transitionCount = json[PROPERTY_NAME_ROOM_TRANSITIONS].size();
        int enemyCount      = json[PROPERTY_NAME_ROOM_ENEMIES].size();
        int itemCount       = json[PROPERTY_NAME_ROOM_ITEMS].size();
        
        Rectangle2D<int> cameraBounds = constructCameraBounds(json[PROPERTY_NAME_ROOM_CAMERABOUNDS], x, y, gridSize);

        //
        // Store tiles and attributes
        //
        std::vector<int> tile(width * height, 0);
        std::vector<int> attr(width * height, 0);

        auto tileArray = json[PROPERTY_NAME_ROOM_TILE];
        auto attrArray = json[PROPERTY_NAME_ROOM_TILEATTRIBUTES];

        for(int ty = 0; ty < height; ++ty) {
            for(int tx = 0; tx < width; ++tx) {
                int offset = tx + (ty * width);
                tile[offset] = tileArray[offset].asInt();
                attr[offset] = attrArray[offset].asInt();
            }
        }

        //
        // Construct spawners
        //
        std::vector<std::shared_ptr<Spawner>> spawners;

        auto spawnerArray = json[PROPERTY_NAME_ROOM_ENEMIES];

        if(enemyCount > 0) {
            HIKARI_LOG(debug) << "Found " << enemyCount << " enemy declarations.";
            for(int enemyIndex = 0; enemyIndex < enemyCount; ++enemyIndex) {
                spawners.emplace_back(constructSpawner(spawnerArray[enemyIndex]));
            }
        }

        spawnerArray = json[PROPERTY_NAME_ROOM_ITEMS];
        
        if(itemCount > 0) {
            HIKARI_LOG(debug) << "Found " << itemCount << " item declarations.";
            for(int itemIndex = 0; itemIndex < itemCount; ++itemIndex) {
                spawners.emplace_back(constructSpawner(spawnerArray[itemIndex]));
            }
        }

        // Construct transitions
        std::vector<RoomTransition> transitions;
        for(int i = 0; i < transitionCount; ++i) {
            transitions.push_back(constructTransition(json[PROPERTY_NAME_ROOM_TRANSITIONS][i]));
        }

        RoomPtr result(new Room(id, x, y, width, height, gridSize, cameraBounds, tile, attr, transitions, spawners));

        return result;
    }

    const SpawnerPtr MapLoader::constructSpawner(const Json::Value &json) const {
        auto type         = json[PROPERTY_NAME_ROOM_ENEMIES_TYPE].asString();
        auto x            = json[PROPERTY_NAME_ROOM_ENEMIES_POSITION_X].asInt();
        auto y            = json[PROPERTY_NAME_ROOM_ENEMIES_POSITION_Y].asInt();
        auto dirString    = json.get(PROPERTY_NAME_ROOM_ENEMIES_DIRECTION, "None").asString();
        auto spawner      = std::make_shared<Spawner>();
        auto direction    = (dirString == "Up" ? Directions::Up : 
                                (dirString == "Right" ? Directions::Right : 
                                    (dirString == "Down" ? Directions::Down : 
                                        (dirString == "Left" ? Directions::Left : 
                                            Directions::None)
                                        )
                                    )
                                );

        spawner->setPosition(Vector2D(static_cast<float>(x), static_cast<float>(y)));
        spawner->setDirection(direction);

        return spawner;
    }

    const RoomTransition MapLoader::constructTransition(const Json::Value &json) const {
        bool isBossEntrance = json.get("boss", false).asBool();
        
        // TODO: Do we need this?
        int from = -1;

        int to = json["to"].asInt();
        int width = json["width"].asInt();
        int height = json["height"].asInt();
        int x = json["x"].asInt();
        int y = json["y"].asInt();
        RoomTransition::Direction dir;

        std::string dirString = json["direction"].asString();

        if(dirString == "forward") {
            dir = RoomTransition::DirectionForward;
        } else if(dirString == "backward") {
            dir = RoomTransition::DirectionBackward;
        } else if(dirString == "up") {
            dir = RoomTransition::DirectionUp;
        } else if(dirString == "down") {
            dir = RoomTransition::DirectionDown;
        } else {
            // All other values are "transport" equivalents.
            dir = RoomTransition::DirectionTeleport;
        }

        return RoomTransition(from, to, width, height, x, y, dir, isBossEntrance);
    }

    const Rectangle2D<int> MapLoader::constructCameraBounds(const Json::Value &json, 
            const int &roomX, const int &roomY, const int &gridSize) const {
        int x = (roomX + json[PROPERTY_NAME_ROOM_CAMERABOUNDS_X].asInt()) * gridSize;
        int y = (roomY + json[PROPERTY_NAME_ROOM_CAMERABOUNDS_Y].asInt()) * gridSize;
        int width = json[PROPERTY_NAME_ROOM_CAMERABOUNDS_WIDTH].asInt() * gridSize;
        int height = json[PROPERTY_NAME_ROOM_CAMERABOUNDS_HEIGHT].asInt() * gridSize;

        return Rectangle2D<int>(x, y, width, height);
    }

    bool MapLoader::validateMapStructure(const Json::Value &json) const {
        bool isValid = true;

        // Root must be an object
        if(!json.isObject()) {
            isValid = false;
        }

        // Root must contain a string property called "tileset"
        if(!json[PROPERTY_NAME_TILESET].isString()) {
            isValid = false;
        }

        if(!json[PROPERTY_NAME_GRIDSIZE].isNumeric()) {
            isValid = false;
        }

        // Root must contain an array property called "rooms"
        if(!json[PROPERTY_NAME_ROOMS].isArray()) {
            isValid = false;
        }

        // Validate rooms here

        if(!isValid) {
            throw std::runtime_error("JSON structure is not a valid Map.");
        }

        return isValid;
    }

    bool MapLoader::validateRoomStructure(const Json::Value &json) const {
        bool isValid = true;

        // Root must be an object
        if(!json.isObject()) {
            isValid = false;
        }

        return isValid;
    }

} // hikari
