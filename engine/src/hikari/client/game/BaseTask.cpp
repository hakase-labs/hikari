#include "hikari/client/game/BaseTask.hpp"

namespace hikari {
    BaseTask::BaseTask(int priority, SequenceType type)
        : Task()
        , sequenceType(type)
        , priority(priority)
        , completedFlag(false)
    {

    }

    BaseTask::~BaseTask() {

    }

    void BaseTask::markAsCompleted() {
        completedFlag = true;
    }

    bool BaseTask::isComplete() const {
        return completedFlag;
    }

    int BaseTask::getPriority() const {
        return priority;
    }

    Task::SequenceType BaseTask::getSequenceType() const {
        return sequenceType;
    }

    void BaseTask::update(float dt) {

    }

} // hikari
