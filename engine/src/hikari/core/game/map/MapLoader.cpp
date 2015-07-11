#include "hikari/core/game/map/MapLoader.hpp"
#include "hikari/core/game/map/Map.hpp"
#include "hikari/core/game/map/Door.hpp"
#include "hikari/core/game/map/Force.hpp"
#include "hikari/core/game/map/Room.hpp"
#include "hikari/core/game/map/RoomTransition.hpp"
#include "hikari/core/game/map/Tileset.hpp"
#include "hikari/client/game/objects/Spawner.hpp"
#include "hikari/client/game/objects/ItemSpawner.hpp"
#include "hikari/client/game/objects/EnemySpawner.hpp"
#include "hikari/client/game/objects/BlockSequenceDescriptor.hpp"
#include "hikari/client/game/objects/BlockTiming.hpp"
#include "hikari/client/scripting/SquirrelUtils.hpp"
#include "hikari/core/geom/BoundingBox.hpp"
#include "hikari/core/util/AnimationSetCache.hpp"
#include "hikari/core/util/ImageCache.hpp"
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

    const char* MapLoader::PROP_TILESET = "tileset";
    const char* MapLoader::PROP_GRIDSIZE = "gridsize";
    const char* MapLoader::PROP_MUSICNAME = "musicName";
    const char* MapLoader::PROP_ROOMS = "rooms";
    const char* MapLoader::PROP_SPECIAL_ROOMS = "specialRooms";
    const char* MapLoader::PROP_SPECIAL_ROOM_STARTING = "starting";
    const char* MapLoader::PROP_SPECIAL_ROOM_MIDPOINT = "midpoint";
    const char* MapLoader::PROP_SPECIAL_ROOM_BOSS_CORRIDOR = "bossCorridor";
    const char* MapLoader::PROP_SPECIAL_ROOM_BOSS_CHAMBER = "bossChamber";
    const char* MapLoader::PROP_BOSS_ENTITY = "bossEntity";
    const char* MapLoader::PROP_ROOM_ID = "id";
    const char* MapLoader::PROP_ROOM_X = "x";
    const char* MapLoader::PROP_ROOM_Y = "y";
    const char* MapLoader::PROP_ROOM_HERO_SPAWN_X = "heroSpawnX";
    const char* MapLoader::PROP_ROOM_HERO_SPAWN_Y = "heroSpawnY";
    const char* MapLoader::PROP_ROOM_WIDTH = "width";
    const char* MapLoader::PROP_ROOM_HEIGHT = "height";
    const char* MapLoader::PROP_ROOM_BG_COLOR = "backgroundColor";
    const char* MapLoader::PROP_ROOM_CAMERABOUNDS = "cameraBounds";
    const char* MapLoader::PROP_ROOM_CAMERABOUNDS_X = "x";
    const char* MapLoader::PROP_ROOM_CAMERABOUNDS_Y = "y";
    const char* MapLoader::PROP_ROOM_CAMERABOUNDS_WIDTH = "width";
    const char* MapLoader::PROP_ROOM_CAMERABOUNDS_HEIGHT = "height";
    const char* MapLoader::PROP_ROOM_TILE = "tile";
    const char* MapLoader::PROP_ROOM_TILEATTRIBUTES = "attr";
    const char* MapLoader::PROP_ROOM_ENEMIES = "enemies";
    const char* MapLoader::PROP_ROOM_ENEMIES_TYPE = "type";
    const char* MapLoader::PROP_ROOM_ENEMIES_POSITION = "position";
    const char* MapLoader::PROP_ROOM_ENEMIES_POSITION_X = "x";
    const char* MapLoader::PROP_ROOM_ENEMIES_POSITION_Y = "y";
    const char* MapLoader::PROP_ROOM_ENEMIES_DIRECTION = "direction";
    const char* MapLoader::PROP_ROOM_ITEMS = "items";
    const char* MapLoader::PROP_ROOM_ITEMS_TYPE = "type";
    const char* MapLoader::PROP_ROOM_ITEMS_X = "x";
    const char* MapLoader::PROP_ROOM_ITEMS_Y = "y";
    const char* MapLoader::PROP_ROOM_TRANSITIONS = "transitions";
    const char* MapLoader::PROP_ROOM_DOORS = "doors";
    const char* MapLoader::PROP_ROOM_DOORS_X = "x";
    const char* MapLoader::PROP_ROOM_DOORS_Y = "y";
    const char* MapLoader::PROP_ROOM_DOORS_WIDTH = "width";
    const char* MapLoader::PROP_ROOM_DOORS_HEIGHT = "height";
    const char* MapLoader::PROP_ROOM_DOORS_ENTRANCE = "entrance";
    const char* MapLoader::PROP_ROOM_DOORS_EXIT = "exit";
    const char* MapLoader::PROP_ROOM_BLOCKSEQUENCES = "blockSequences";
    const char* MapLoader::PROP_ROOM_BLOCKSEQUENCES_X = "x";
    const char* MapLoader::PROP_ROOM_BLOCKSEQUENCES_Y = "y";
    const char* MapLoader::PROP_ROOM_BLOCKSEQUENCES_WIDTH = "width";
    const char* MapLoader::PROP_ROOM_BLOCKSEQUENCES_HEIGHT = "height";
    const char* MapLoader::PROP_ROOM_BLOCKSEQUENCES_BLOCKS = "blocks";
    const char* MapLoader::PROP_ROOM_BLOCKSEQUENCES_SEQUENCE = "sequence";
    const char* MapLoader::PROP_ROOM_BLOCKSEQUENCES_INTERVAL = "interval";
    const char* MapLoader::PROP_ROOM_BLOCKSEQUENCES_MAXIMUM_BLOCK_AGE = "maximumBlockAge";
    const char* MapLoader::PROP_ROOM_BLOCKSEQUENCES_ENTITY = "entity";
    const char* MapLoader::PROP_ROOM_BLOCKSEQUENCES_SPAWN_SOUND = "spawnSound";

    const int MapLoader::DEFAULT_HERO_SPAWN_X = 0;
    const int MapLoader::DEFAULT_HERO_SPAWN_Y = 0;

    MapLoader::MapLoader(const std::shared_ptr<AnimationSetCache> & animationSetCache,
        const std::shared_ptr<ImageCache> & imageCache,
        const std::shared_ptr<TilesetCache> &tilesetCache
    )
        : animationSetCache(animationSetCache)
        , imageCache(imageCache)
        , tilesetCache(tilesetCache) {

    }

    MapLoader::~MapLoader() { }

    MapPtr MapLoader::loadFromJson(const Json::Value &json) const {
        validateMapStructure(json);
        return constructMap(json);
    }

    MapPtr MapLoader::constructMap(const Json::Value &json) const {
        // Parse map attributes
        std::string tilesetName = json[PROP_TILESET].asString();
        int gridSize = json[PROP_GRIDSIZE].asInt();
        std::string musicName = json.get(PROP_MUSICNAME, "None").asString();
        int roomCount = json[PROP_ROOMS].size();

        TileDataPtr tileset = nullptr;

        try {
            tileset = tilesetCache->get(tilesetName);
        } catch(std::runtime_error & err) {
            HIKARI_LOG(fatal) << "Couldn't load a tileset while constructing a map. (" << err.what() << ")";
        }

        // Parse rooms
        std::vector<RoomPtr> rooms;
        for(int i = 0; i < roomCount; ++i) {
            rooms.push_back(constructRoom(json[PROP_ROOMS][i], gridSize));
        }

        //
        // Determine special room indicies
        //
        int startingRoomIndex = 0;
        int midpointRoomIndex = 0;
        int bossCorridorIndex = 0;
        int bossChamberIndex  = 0;

        auto specialRoomIndicies = json[PROP_SPECIAL_ROOMS];

        if(!specialRoomIndicies.isNull()) {
            startingRoomIndex = specialRoomIndicies.get(PROP_SPECIAL_ROOM_STARTING,      0).asInt();
            midpointRoomIndex = specialRoomIndicies.get(PROP_SPECIAL_ROOM_MIDPOINT,      0).asInt();
            bossCorridorIndex = specialRoomIndicies.get(PROP_SPECIAL_ROOM_BOSS_CORRIDOR, 0).asInt();
            bossChamberIndex  = specialRoomIndicies.get(PROP_SPECIAL_ROOM_BOSS_CHAMBER,  0).asInt();
        }

        std::string bossEntity = json.get(PROP_BOSS_ENTITY, "None").asString();

        return MapPtr(
            new Map(
                tileset,
                gridSize,
                musicName,
                bossEntity,
                rooms,
                startingRoomIndex,
                midpointRoomIndex,
                bossCorridorIndex,
                bossChamberIndex
            )
        );
    }

    RoomPtr MapLoader::constructRoom(const Json::Value &json, int gridSize) const {
        int id              = json[PROP_ROOM_ID].asInt();
        int x               = json[PROP_ROOM_X].asInt();
        int y               = json[PROP_ROOM_Y].asInt();
        int width           = json[PROP_ROOM_WIDTH].asInt();
        int height          = json[PROP_ROOM_HEIGHT].asInt();
        int backgroundColor = json.get(PROP_ROOM_BG_COLOR, Room::DEFAULT_BG_COLOR).asInt();
        int heroSpawnX      = json.get(PROP_ROOM_HERO_SPAWN_X, DEFAULT_HERO_SPAWN_X).asInt();
        int heroSpawnY      = json.get(PROP_ROOM_HERO_SPAWN_Y, DEFAULT_HERO_SPAWN_Y).asInt();
        int transitionCount = json[PROP_ROOM_TRANSITIONS].size();
        int enemyCount      = json[PROP_ROOM_ENEMIES].size();
        int itemCount       = json[PROP_ROOM_ITEMS].size();
        int blockSequenceCount = 0;
        bool hasDoors       = json.isMember(PROP_ROOM_DOORS);
        const std::string bossEntity = json.get(PROP_BOSS_ENTITY, Room::DEFAULT_BOSS_ENTITY_NAME).asString();

        Point2D<int> heroSpawnPosition = Point2D<int>(heroSpawnX, heroSpawnY);
        Rectangle2D<int> cameraBounds = constructCameraBounds(json[PROP_ROOM_CAMERABOUNDS], x, y, gridSize);

        //
        // Store tiles and attributes
        //
        std::vector<int> tile(width * height, 0);
        std::vector<int> attr(width * height, 0);

        const auto & tileArray = json[PROP_ROOM_TILE];
        const auto & attrArray = json[PROP_ROOM_TILEATTRIBUTES];

        for(int ty = 0; ty < height; ++ty) {
            for(int tx = 0; tx < width; ++tx) {
                int offset = tx + (ty * width);
                tile[offset] = tileArray[offset].asInt();
                attr[offset] = attrArray[offset].asInt();
            }
        }

        // The origin of the room, in pixels, for relative object offsets.
        int roomOriginX = x * gridSize;
        int roomOriginY = y * gridSize;

        HIKARI_LOG(debug4) << "Room origin: (" << roomOriginX << ", " << roomOriginY << ")";

        //
        // Construct spawners
        //
        std::vector<std::shared_ptr<Spawner>> spawners;

        const auto & enemySpawnerArray = json[PROP_ROOM_ENEMIES];

        if(enemyCount > 0) {
            HIKARI_LOG(debug) << "Found " << enemyCount << " enemy declarations.";
            for(int enemyIndex = 0; enemyIndex < enemyCount; ++enemyIndex) {
                spawners.emplace_back(
                    constructSpawner(
                        enemySpawnerArray[enemyIndex],
                        SPAWN_ENEMY,
                        roomOriginX,
                        roomOriginY
                    )
                );
            }
        }

        const auto & itemSpawnerArray = json[PROP_ROOM_ITEMS];

        if(itemCount > 0) {
            HIKARI_LOG(debug) << "Found " << itemCount << " item declarations.";
            for(int itemIndex = 0; itemIndex < itemCount; ++itemIndex) {
                spawners.emplace_back(
                    constructSpawner(
                        itemSpawnerArray[itemIndex],
                        SPAWN_ITEM,
                        roomOriginX,
                        roomOriginY
                    )
                );
            }
        }

        //
        // Construct forces
        //
        std::vector<std::shared_ptr<Force>> forces;

        const auto & forceArray = json["forces"];

        if(forceArray.size() > 0) {
            HIKARI_LOG(debug) << "Found " << forceArray.size() << " force declarations.";
            for(std::size_t forceIndex = 0; forceIndex < forceArray.size(); ++forceIndex) {
                forces.emplace_back(
                    constructForce(
                        forceArray[forceIndex],
                        roomOriginX,
                        roomOriginY
                    )
                );
            }
        }

        //
        // Construct transitions
        //
        std::vector<RoomTransition> transitions;
        for(int i = 0; i < transitionCount; ++i) {
            transitions.emplace_back(constructTransition(json[PROP_ROOM_TRANSITIONS][i]));
        }

        //
        // Construct block sequences
        //
        std::vector<BlockSequenceDescriptor> blockSequences;

        if(json[PROP_ROOM_BLOCKSEQUENCES].isArray()) {
            blockSequenceCount = json[PROP_ROOM_BLOCKSEQUENCES].size();
        }

        for(int i = 0; i < blockSequenceCount; ++i) {
            blockSequences.emplace_back(
                constructBlockSequence(
                    json[PROP_ROOM_BLOCKSEQUENCES][i],
                    roomOriginX,
                    roomOriginY,
                    gridSize
                )
            );
        }

        //
        // Construct doors
        //
        std::unique_ptr<Door> entranceDoor;
        std::unique_ptr<Door> exitDoor;

        if(hasDoors) {
            HIKARI_LOG(debug4) << "The room has doors array.";
            const auto & doorsJson = json[PROP_ROOM_DOORS];

            if(doorsJson.size() > 0) {
                if(doorsJson.isMember(PROP_ROOM_DOORS_ENTRANCE)) {
                    HIKARI_LOG(debug4) << "The room has an entrance.";
                    entranceDoor = constructDoor(doorsJson[PROP_ROOM_DOORS_ENTRANCE], x, y);
                }

                if(doorsJson.isMember(PROP_ROOM_DOORS_EXIT)) {
                    HIKARI_LOG(debug4) << "The room has an exit.";
                    exitDoor = constructDoor(doorsJson[PROP_ROOM_DOORS_EXIT], x, y);
                }
            } else {
                HIKARI_LOG(debug4) << "The room has no door definitions.";
            }
        }

        RoomPtr result(
            new Room(
                id,
                x,
                y,
                width,
                height,
                gridSize,
                backgroundColor,
                heroSpawnPosition,
                cameraBounds,
                std::move(tile),
                std::move(attr),
                std::move(transitions),
                std::move(spawners),
                std::move(forces),
                std::move(blockSequences),
                std::move(entranceDoor),
                std::move(exitDoor),
                bossEntity
            )
        );

        return result;
    }

    std::unique_ptr<Door> MapLoader::constructDoor(const Json::Value & json, int offsetX, int offsetY) const {
        int x = json.get(PROP_ROOM_DOORS_X, 0).asInt();
        int y = json.get(PROP_ROOM_DOORS_Y, 0).asInt();
        int width = json.get(PROP_ROOM_DOORS_WIDTH, 1).asInt();
        int height = json.get(PROP_ROOM_DOORS_HEIGHT, 3).asInt();

        std::unique_ptr<Door> doorInstance(new Door(x + offsetX, y + offsetY, width, height));

        auto animationSetPtr = animationSetCache->get("assets/animations/door.json");
        // auto spriteTexture = imageCache->get(animationSetPtr->getImageFileName());

        doorInstance->setAnimationSet(animationSetPtr);

        return doorInstance;
    }

    SpawnerPtr MapLoader::constructSpawner(const Json::Value &json, SpawnType type, int offsetX, int offsetY) const {
        auto spawner = std::shared_ptr<Spawner>(nullptr);

        HIKARI_LOG(debug4) << "constructSpawner offset: (" << offsetX << ", " << offsetY << ")";

        switch(type) {
            case SPAWN_ITEM:
            {
                auto type         = json[PROP_ROOM_ENEMIES_TYPE].asString();
                auto x            = json[PROP_ROOM_ENEMIES_POSITION_X].asInt();
                auto y            = json[PROP_ROOM_ENEMIES_POSITION_Y].asInt();
                auto dirString    = json.get(PROP_ROOM_ENEMIES_DIRECTION, "None").asString();
                auto direction    = (dirString == "Up" ? Directions::Up :
                                        (dirString == "Right" ? Directions::Right :
                                            (dirString == "Down" ? Directions::Down :
                                                (dirString == "Left" ? Directions::Left :
                                                    Directions::None)
                                                )
                                            )
                                        );

                x += offsetX;
                y += offsetY;

                spawner.reset(new ItemSpawner(type));
                spawner->setPosition(Vector2<float>(static_cast<float>(x), static_cast<float>(y)));
                spawner->setDirection(direction);

                return spawner;
            }

            case SPAWN_ENEMY:
            {
                auto type         = json[PROP_ROOM_ENEMIES_TYPE].asString();
                auto x            = json[PROP_ROOM_ENEMIES_POSITION_X].asInt();
                auto y            = json[PROP_ROOM_ENEMIES_POSITION_Y].asInt();
                auto dirString    = json.get(PROP_ROOM_ENEMIES_DIRECTION, "None").asString();
                auto direction    = (dirString == "Up" ? Directions::Up :
                                        (dirString == "Right" ? Directions::Right :
                                            (dirString == "Down" ? Directions::Down :
                                                (dirString == "Left" ? Directions::Left :
                                                    Directions::None)
                                                )
                                            )
                                        );
                auto spawnLimit   = json.get("spawnLimit", 1).asUInt();
                auto spawnRate    = static_cast<float>(json.get("spawnRate", 1.0).asDouble());

                x += offsetX;
                y += offsetY;

                HIKARI_LOG(debug4) << "Spawning enemy at (" << x << ", " << y << ")";

                auto enemySpawner = std::make_shared<EnemySpawner>(type, spawnLimit, spawnRate);
                enemySpawner->setPosition(Vector2<float>(static_cast<float>(x), static_cast<float>(y)));
                enemySpawner->setDirection(direction);

                if(json.isMember("continuous")) {
                    bool isContinuous = json["continuous"].asBool();
                    enemySpawner->setContinuous(isContinuous);
                }

                if(json.isMember("config")) {
                    auto configJson = json["config"];

                    Sqrat::Table configTable(Sqrat::DefaultVM::Get());

                    if(!configJson.isNull()) {
                        configTable = SquirrelUtils::jsonToSquirrel(Sqrat::DefaultVM::Get(), configJson);
                    }

                    enemySpawner->setInstanceConfig(configTable);
                }

                spawner = enemySpawner;

                return spawner;
            }
            default:
                break;
        }

        return spawner;
    }

    ForcePtr MapLoader::constructForce(const Json::Value &json, int offsetX, int offsetY) const {
        HIKARI_LOG(debug4) << "constructForce offset: (" << offsetX << ", " << offsetY << ")";

        auto x = json["x"].asInt();
        auto y = json["y"].asInt();
        auto width = json["width"].asInt();
        auto height = json["height"].asInt();
        auto vX = json.get("velocityX", 0.0f).asFloat();
        auto vY = json.get("velocityY", 0.0f).asFloat();

        auto force = std::make_shared<Force>(
            BoundingBox<float>(
                static_cast<float>(x),
                static_cast<float>(y),
                static_cast<float>(width),
                static_cast<float>(height)
            ),
            Vector2<float>(vX, vY)
        );

        return force;
    }

    RoomTransition MapLoader::constructTransition(const Json::Value &json) const {
        bool isDoor = json.get("door", false).asBool();
        bool ladderOnly = json.get("ladderOnly", false).asBool();

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

        return RoomTransition(from, to, width, height, x, y, dir, isDoor, ladderOnly);
    }

    BlockSequenceDescriptor MapLoader::constructBlockSequence(const Json::Value &json,
        int roomX, int roomY, int gridSize) const {
        const int x = roomX + json[PROP_ROOM_BLOCKSEQUENCES_X].asInt() * gridSize;
        const int y = roomY + json[PROP_ROOM_BLOCKSEQUENCES_Y].asInt() * gridSize;
        const int width = json[PROP_ROOM_BLOCKSEQUENCES_WIDTH].asInt() * gridSize;
        const int height = json[PROP_ROOM_BLOCKSEQUENCES_HEIGHT].asInt() * gridSize;

        const float spawnInterval = json.get(
            PROP_ROOM_BLOCKSEQUENCES_INTERVAL,
            BlockSequenceDescriptor::DEFAULT_SPAWN_INTERVAL
        ).asFloat();
        const float maximumBlockAge = json.get(
            PROP_ROOM_BLOCKSEQUENCES_MAXIMUM_BLOCK_AGE,
            BlockSequenceDescriptor::DEFAULT_MAXIMUM_BLOCK_AGE
        ).asFloat();

        const std::string entityName = json.get(
            PROP_ROOM_BLOCKSEQUENCES_ENTITY,
            BlockSequenceDescriptor::DEFAULT_BLOCK_ENTITY
        ).asString();
        const std::string spawnSound = json.get(
            PROP_ROOM_BLOCKSEQUENCES_SPAWN_SOUND,
            BlockSequenceDescriptor::DEFAULT_SPAWN_SOUND
        ).asString();

        std::vector<Point2D<int>> blockPositions;
        std::vector<BlockTiming> timing;

        const auto & blockJson = json[PROP_ROOM_BLOCKSEQUENCES_BLOCKS];

        if(blockJson.isArray()) {
            const std::size_t length = blockJson.size();

            for(std::size_t i = 0; i < length; ++i) {
                const auto & block = blockJson[i];
                int blockX = x + block[PROP_ROOM_BLOCKSEQUENCES_X].asInt() * gridSize;
                int blockY = y + block[PROP_ROOM_BLOCKSEQUENCES_Y].asInt() * gridSize;

                blockPositions.push_back(Point2D<int>(blockX, blockY));
            }
        }

        const auto & sequenceJson = json[PROP_ROOM_BLOCKSEQUENCES_SEQUENCE];

        if(sequenceJson.isArray()) {
            const std::size_t length = sequenceJson.size();

            for(std::size_t i = 0; i < length; ++i) {
                const auto & blockIndiciesJson = sequenceJson[i];

                std::vector<int> indicies;

                for(std::size_t index = 0; index < blockIndiciesJson.size(); ++index) {
                    const auto & id = blockIndiciesJson[index].asInt();
                    indicies.push_back(id);
                }

                timing.push_back(BlockTiming(indicies));
            }
        }

        return BlockSequenceDescriptor(
            Rectangle2D<int>(x, y, width, height),
            blockPositions,
            timing,
            spawnInterval,
            maximumBlockAge,
            entityName,
            spawnSound
        );
    }

    Rectangle2D<int> MapLoader::constructCameraBounds(const Json::Value &json,
            int roomX, int roomY, int gridSize) const {
        int x = (roomX + json[PROP_ROOM_CAMERABOUNDS_X].asInt()) * gridSize;
        int y = (roomY + json[PROP_ROOM_CAMERABOUNDS_Y].asInt()) * gridSize;
        int width = json[PROP_ROOM_CAMERABOUNDS_WIDTH].asInt() * gridSize;
        int height = json[PROP_ROOM_CAMERABOUNDS_HEIGHT].asInt() * gridSize;

        return Rectangle2D<int>(x, y, width, height);
    }

    bool MapLoader::validateMapStructure(const Json::Value &json) const {
        bool isValid = true;

        // Root must be an object
        if(!json.isObject()) {
            isValid = false;
        }

        // Root must contain a string property called "tileset"
        if(!json[PROP_TILESET].isString()) {
            isValid = false;
        }

        if(!json[PROP_GRIDSIZE].isNumeric()) {
            isValid = false;
        }

        // Root must contain an array property called "rooms"
        if(!json[PROP_ROOMS].isArray()) {
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
