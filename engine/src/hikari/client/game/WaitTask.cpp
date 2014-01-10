#include "hikari/client/game/WaitTask.hpp"

namespace hikari {
    WaitTask::WaitTask(float threshold)
        : BaseTask(0, Task::TYPE_BLOCKING)
        , threshold(threshold)
    {

    }

    WaitTask::~WaitTask() {

    }

    void WaitTask::update(float dt) {
        threshold -= dt;

        if(threshold <= 0) {
          markAsCompleted();
        }
    }

} // hikari
