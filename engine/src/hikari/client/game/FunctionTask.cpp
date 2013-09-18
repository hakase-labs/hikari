#include "hikari/client/game/FunctionTask.hpp"

namespace hikari {
    FunctionTask::FunctionTask(int priority, TaskFunction func)
        : BaseTask(priority, Task::TYPE_PARALLEL)
        , func(func)
    {

    }

    FunctionTask::FunctionTask(int priority, SequenceType type, TaskFunction func)
        : BaseTask(priority, type)
        , func(func)
    {

    }

    FunctionTask::~FunctionTask() {

    }

    void FunctionTask::update(float dt) {
        if(func) {
            bool done = func(dt);

            if(done) {
                markAsCompleted();
            }
        }
    }

} // hikari
