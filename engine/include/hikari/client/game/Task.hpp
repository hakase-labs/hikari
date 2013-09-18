#ifndef HIKARI_CLIENT_GAME_TASK
#define HIKARI_CLIENT_GAME_TASK

#include "hikari/core/game/Updatable.hpp"

namespace hikari {

    /**
     * A task is a class that encapsulates some kind of job that is completed
     * on a schedule. Some tasks may require multiple updates before they are
     * completed. Some tasks can block, which prevents other tasks from 
     * executing, while others run in parallel.
     */
    class Task : public Updatable {
    public:
        enum SequenceType {
            TYPE_PARALLEL = 0,
            TYPE_BLOCKING
        };

        //
        // Note to self: consider adding a TaskStatus enum that has values
        // like NOT_STARTED, IN_PROGRESS, COMPLETED so you can do more
        // interesting this with task states.
        //

    protected:
        /**
         * Marks the Task as completed. Completed Tasks are removed from the
         * task queue.
         *
         * @see Task::isCompleted
         */
        virtual void markAsCompleted() = 0;

    public:
        virtual ~Task();
        virtual bool isComplete() const = 0;
        virtual int getPriority() const = 0;
        virtual SequenceType getSequenceType() const = 0;

        virtual void update(float dt) = 0;
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_TASK