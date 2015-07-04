#ifndef HIKARI_CLIENT_GAME_OBJECTS_BLOCKSEQUENCEDESCRIPTOR
#define HIKARI_CLIENT_GAME_OBJECTS_BLOCKSEQUENCEDESCRIPTOR

#include "hikari/core/geom/Point2D.hpp"
#include "hikari/core/geom/Rectangle2D.hpp"
#include "hikari/client/game/objects/BlockTiming.hpp"
#include <vector>

namespace hikari {
    class BlockSequenceDescriptor {
    private:
        Rectangle2D<int> bounds;
        std::vector<Point2D<int>> blockPositions;
        std::vector<BlockTiming> timing;

    public:
        BlockSequenceDescriptor(const Rectangle2D<int> & bounds, const std::vector<Point2D<int>> & blockPositions, const std::vector<BlockTiming> & timing);

        const Rectangle2D<int> & getBounds() const;
        const std::vector<Point2D<int>> & getBlockPositions() const;
        const std::vector<BlockTiming> & getTiming() const;
    };
} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_BLOCKSEQUENCEDESCRIPTOR
