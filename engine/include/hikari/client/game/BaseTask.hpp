#ifndef HIKARI_CLIENT_GAME_BASETASK
#define HIKARI_CLIENT_GAME_BASETASK

#include "hikari/client/game/Task.hpp"

namespace hikari {

    /**
     * A task is a class that encapsulates some kind of job that is completed
     * on a schedule. Some tasks may require multiple updates before they are
     * completed. Some tasks can block, which prevents other tasks from 
     * executing, while others run in parallel.
     */
    class BaseTask : public Task {
    private:
        SequenceType sequenceType;
        int priority;
        bool completedFlag;

    protected:
        virtual void markAsCompleted();

    public:
        explicit BaseTask(int priority, SequenceType type = TYPE_PARALLEL);
        virtual ~BaseTask();

        virtual bool isComplete() const;
        virtual int getPriority() const;
        virtual Task::SequenceType getSequenceType() const;

        virtual void update(float dt);
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_BASETASK