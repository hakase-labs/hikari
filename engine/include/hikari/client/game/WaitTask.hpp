#ifndef HIKARI_CLIENT_GAME_WAITTASK
#define HIKARI_CLIENT_GAME_WAITTASK

#include "hikari/client/game/BaseTask.hpp"
#include <functional>

namespace hikari {

    class WaitTask : public BaseTask {
    private:
        float threshold;

    public:
        WaitTask(float threshold);
        virtual ~WaitTask();

        virtual void update(float dt);
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_WAITTASK