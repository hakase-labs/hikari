#include "hikari/client/game/objects/BlockTiming.hpp"

namespace hikari {
    BlockTiming::BlockTiming(float at, const std::vector<int> & on, const std::vector<int> & off)
        : at(at)
        , activations(on)
        , deactivations(off)
    {

    }

    float BlockTiming::getAtTime() const {
        return at;
    }

    const std::vector<int> & BlockTiming::getActivations() const {
        return activations;
    }

    const std::vector<int> & BlockTiming::getDeactivations() const {
        return deactivations;
    }
} // hikari
