#ifndef HIKARI_CLIENT_GAME_OBJECTS_BLOCKTIMING
#define HIKARI_CLIENT_GAME_OBJECTS_BLOCKTIMING

#include <vector>

namespace hikari {
    class BlockTiming {
    private:
        std::vector<int> blockIndicies;

    public:
        explicit BlockTiming(const std::vector<int> & blockIndicies);
        const std::vector<int> & getBlockIndicies() const;
    };
} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_BLOCKTIMING
