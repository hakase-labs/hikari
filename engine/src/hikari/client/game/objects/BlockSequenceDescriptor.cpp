#include "hikari/client/game/objects/BlockSequenceDescriptor.hpp"

namespace hikari {
    BlockSequenceDescriptor::BlockSequenceDescriptor(const Rectangle2D<int> & bounds, const std::vector<Point2D<int>> & blockPositions, const std::vector<BlockTiming> & timing)
        : bounds(bounds)
        , blockPositions(blockPositions)
        , timing(timing)
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
} // hikari
