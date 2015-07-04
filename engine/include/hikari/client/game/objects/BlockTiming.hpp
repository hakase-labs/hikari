#ifndef HIKARI_CLIENT_GAME_OBJECTS_BLOCKTIMING
#define HIKARI_CLIENT_GAME_OBJECTS_BLOCKTIMING

#include <vector>

namespace hikari {
    class BlockTiming {
    private:
        float at;
        std::vector<int> activations;
        std::vector<int> deactivations;

    public:
        BlockTiming(float at, const std::vector<int> & on, const std::vector<int> & off);

        float getAtTime() const;
        const std::vector<int> & getActivations() const;
        const std::vector<int> & getDeactivations() const;
    };
} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_BLOCKTIMING
