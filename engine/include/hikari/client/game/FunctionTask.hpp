#ifndef HIKARI_CLIENT_GAME_FUNCTIONTASK
#define HIKARI_CLIENT_GAME_FUNCTIONTASK

#include "hikari/client/game/BaseTask.hpp"
#include <functional>

namespace hikari {

    class FunctionTask : public BaseTask {
    public:
        typedef std::function<bool (float dt)> TaskFunction;

    private:
        TaskFunction func;

    public:
        FunctionTask(int priority, TaskFunction taskFunction);
        FunctionTask(int priority, SequenceType type, TaskFunction taskFunction);
        virtual ~FunctionTask();

        virtual void update(float dt);
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_FUNCTIONTASK