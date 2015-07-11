#include "hikari/client/game/objects/BlockTiming.hpp"

namespace hikari {
    BlockTiming::BlockTiming(const std::vector<int> & blockIndicies)
        : blockIndicies(blockIndicies)
    {

    }

    const std::vector<int> & BlockTiming::getBlockIndicies() const {
        return blockIndicies;
    }
} // hikari
