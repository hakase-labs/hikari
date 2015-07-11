#ifndef HIKARI_CLIENT_GAME_OBJECTS_BLOCKSEQUENCEDESCRIPTOR
#define HIKARI_CLIENT_GAME_OBJECTS_BLOCKSEQUENCEDESCRIPTOR

#include "hikari/core/geom/Point2D.hpp"
#include "hikari/core/geom/Rectangle2D.hpp"
#include "hikari/client/game/objects/BlockTiming.hpp"
#include <string>
#include <vector>

namespace hikari {
    class BlockSequenceDescriptor {
    private:
        Rectangle2D<int> bounds;
        std::vector<Point2D<int>> blockPositions;
        std::vector<BlockTiming> timing;
        float spawnInterval;
        float maximumBlockAge;
        std::string entityName;
        std::string soundName;

    public:
        static const float DEFAULT_SPAWN_INTERVAL;
        static const float DEFAULT_MAXIMUM_BLOCK_AGE;
        static const char * DEFAULT_BLOCK_ENTITY;
        static const char * DEFAULT_SPAWN_SOUND;

        BlockSequenceDescriptor(
            const Rectangle2D<int> & bounds,
            const std::vector<Point2D<int>> & blockPositions,
            const std::vector<BlockTiming> & timing,
            float spawnInterval,
            float maximumBlockAge,
            const std::string & entityName,
            const std::string & soundName
        );

        const Rectangle2D<int> & getBounds() const;
        const std::vector<Point2D<int>> & getBlockPositions() const;
        const std::vector<BlockTiming> & getTiming() const;
        float getSpawnInterval() const;
        float getMaximumBlockAge() const;
        const std::string & getEntityName() const;
        const std::string & getSoundName() const;
    };
} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_BLOCKSEQUENCEDESCRIPTOR
