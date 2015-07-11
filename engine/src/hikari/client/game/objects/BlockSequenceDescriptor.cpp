#include "hikari/client/game/objects/BlockSequenceDescriptor.hpp"

namespace hikari {
    const float BlockSequenceDescriptor::DEFAULT_SPAWN_INTERVAL = 1.0f;
    const float BlockSequenceDescriptor::DEFAULT_MAXIMUM_BLOCK_AGE = 106.0f / 60.0f;
    const char * BlockSequenceDescriptor::DEFAULT_BLOCK_ENTITY = "Appearing Block (Red)";
    const char * BlockSequenceDescriptor::DEFAULT_SPAWN_SOUND = "Disappearing Block";

    BlockSequenceDescriptor::BlockSequenceDescriptor(
        const Rectangle2D<int> & bounds,
        const std::vector<Point2D<int>> & blockPositions,
        const std::vector<BlockTiming> & timing,
        float spawnInterval,
        float maximumBlockAge,
        const std::string & entityName,
        const std::string & soundName
    )
        : bounds(bounds)
        , blockPositions(blockPositions)
        , timing(timing)
        , spawnInterval(spawnInterval)
        , maximumBlockAge(maximumBlockAge)
        , entityName(entityName)
        , soundName(soundName)
    {

    }

    const Rectangle2D<int> & BlockSequenceDescriptor::getBounds() const {
        return bounds;
    }

    const std::vector<Point2D<int>> & BlockSequenceDescriptor::getBlockPositions() const {
        return blockPositions;
    }

    const std::vector<BlockTiming> & BlockSequenceDescriptor::getTiming() const {
        return timing;
    }

    float BlockSequenceDescriptor::getSpawnInterval() const {
        return spawnInterval;
    }

    float BlockSequenceDescriptor::getMaximumBlockAge() const {
        return maximumBlockAge;
    }

    const std::string & BlockSequenceDescriptor::getEntityName() const {
        return entityName;
    }

    const std::string & BlockSequenceDescriptor::getSoundName() const {
        return soundName;
    }
} // hikari
