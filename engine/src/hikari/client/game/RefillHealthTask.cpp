#include "hikari/client/game/RefillHealthTask.hpp"
#include "hikari/client/game/GameProgress.hpp"
#include "hikari/client/audio/AudioService.hpp"
#include "hikari/core/util/Log.hpp"

namespace hikari {
    const float RefillHealthTask::DELAY_PER_HEALTH_TICK = (1.0f / 60.0f) * 4.0f; // 4-frames

    RefillHealthTask::RefillHealthTask(int refillAmount,
            const std::weak_ptr<AudioService> & audioService,
            const std::weak_ptr<GameProgress> & gameProgress)
        : BaseTask(0, Task::TYPE_BLOCKING)
        , refillAmount(refillAmount)
        , refillCounter(refillAmount)
        , delayTimer(delayTimer)
        , audioService(audioService)
        , gameProgress(gameProgress)
    {
        delayTimer = DELAY_PER_HEALTH_TICK;

        if(const auto & progress = gameProgress.lock()) {
            int energy = progress->getPlayerEnergy();
            int diff = progress->getPlayerMaxEnergy() - energy;

            refillCounter = std::min(refillAmount, diff);
        }
    }

    RefillHealthTask::~RefillHealthTask() {

    }

    void RefillHealthTask::update(float dt) {
        if(refillCounter > 0) {
            if(delayTimer > 0.0f) {
            } else {
                HIKARI_LOG(debug4) << "RefillHealthTask " << refillCounter;
                delayTimer = DELAY_PER_HEALTH_TICK;
                refillCounter -= 1;

                if(const auto & audio = audioService.lock()) {
                    audio->playSample(28); // SAMPLE_REFILL
                }

                if(const auto & progress = gameProgress.lock()) {
                    progress->setPlayerEnergy(progress->getPlayerEnergy() + 1);
                }
            }

            delayTimer -= dt;
        } else {
            markAsCompleted();
        }
    }

} // hikari
