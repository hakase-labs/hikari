#include "hikari/client/game/objects/BlockSequenceDescriptor.hpp"

namespace hikari {
    BlockSequenceDescriptor::BlockSequenceDescriptor(
        const Rectangle2D<int> & bounds,
        const std::vector<Point2D<int>> & blockPositions,
        const std::vector<BlockTiming> & timing,
        const std::string & entityName,
        const std::string & soundName
    )
        : bounds(bounds)
        , blockPositions(blockPositions)
        , timing(timing)
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

    const std::string & BlockSequenceDescriptor::getEntityName() const {
        return entityName;
    }

    const std::string & BlockSequenceDescriptor::getSoundName() const {
        return soundName;
    }
} // hikari
