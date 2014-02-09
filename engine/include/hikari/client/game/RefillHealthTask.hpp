#ifndef HIKARI_CLIENT_GAME_REFILL_HEALTH_TASK
#define HIKARI_CLIENT_GAME_REFILL_HEALTH_TASK

#include "hikari/client/game/BaseTask.hpp"
#include <memory>

namespace hikari {

    class AudioService;
    class GameProgress;

    class RefillHealthTask : public BaseTask {
    public:
        enum RefillType {
            PLAYER_ENERGY,
            WEAPON_ENERGY,
            BOSS_ENERGY
        };

    private:
        const static float DELAY_PER_HEALTH_TICK;
        RefillType type;
        int refillCounter;
        float delayTimer;
        std::weak_ptr<AudioService> audioService;
        std::weak_ptr<GameProgress> gameProgress;

    public:
        RefillHealthTask(RefillType type, int refillAmount,
            const std::weak_ptr<AudioService> & audioService,
            const std::weak_ptr<GameProgress> & gameProgress);

        virtual ~RefillHealthTask();

        virtual void update(float dt);
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_REFILL_HEALTH_TASK