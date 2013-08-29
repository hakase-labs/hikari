#ifndef HIKARI_CLIENT_GAME_OBJECTS_SPAWNER
#define HIKARI_CLIENT_GAME_OBJECTS_SPAWNER

#include "hikari/client/game/objects/GameObject.hpp"
#include "hikari/client/game/GameWorld.hpp"
#include "hikari/core/game/Direction.hpp"
#include "hikari/core/math/Vector2.hpp"
#include "hikari/client/game/events/EventData.hpp"
#include "hikari/client/game/events/EventListenerDelegate.hpp"

#include <memory>

namespace hikari {

    class EventBus;
    
    /**
     * A Spawner is responsible for spawning one or more objects in the game
     * world. It keeps track of whether its spawn should be alive or not.
    */
    class Spawner : public GameObject {
    private:
        Direction direction;
        Vector2<float> position;
        bool awake;

    protected:
        std::vector<std::pair<EventListenerDelegate, EventType>> eventHandlerDelegates;

    public:
        Spawner();

        virtual ~Spawner() {
            // no-op
        }

        /**
         * Gets the direction in which the spawned object should face when
         * spawned.
         */
        const Direction& getDirection() const;

        /**
         * Sets the direction in which the spawned object should face when
         * spawned.
         * 
         * @param direction Direction to face
         */
        void setDirection(const Direction& direction);

        /**
         * Gets the position where this Spawner's offspring will spawn.
         *
         * @return offspring's spawn position in world coordinates.
         */
        const Vector2<float>& getPosition() const;

        /**
         * Sets the position where this Spawner's offspring will spawn.
         *
         * @param newPosition the offspring's spawn position in world 
         * coordinates.
         */
        void setPosition(const Vector2<float>& newPosition);

        /**
         * Performs an implementation-specific "spawning" action.
         * 
         * @param world the World to use for spawning
         */
        virtual void performAction(GameWorld & world);

        /**
         * Attaches event listeners for the spawner to use.
         * 
         * @param eventBus the EventBus to attach listeners to.
         */
        virtual void attachEventListeners(EventBus & eventBus);

        /**
         * Removes any previously-attached event listeners from an EventBus.
         *
         * @param eventBus the EventBus to remove from.
         */
        virtual void detachEventListeners(EventBus & eventBus);

        /**
         * Determines if the Spawner is awake or not. Being awake indicates
         * that it is in a position to spawn an object (like being on screen,
         * for example).
         *
         * @return if the spawner is awake or not
         * @see Spawner::setAwake
         */
        bool isAwake() const;

        /**
         * Sets whether the Spawner is awake or not.
         * 
         * @param awake the value to set Spawner's awakeness to
         * @see Spawner::isAwake
         */
        void setAwake(bool awake);

        /**
         * Determines whether the Spawner is able to spawn an object at this
         * time. By default a Spawner is able to spawn if it is not awake. This
         * logic is implementation-specific, so subclasses may change how this
         * is determined.
         *
         * @return true if able to spawn an object, false otherwise
         */
        virtual bool canSpawn() const;

        /**
         * Called when a Spawner is awoken from sleep.
         */
        virtual void onWake();

        /**
         * Called when a Spawner is put to bed.
         */
        virtual void onSleep();

        //
        // GameObject overrides
        //
        virtual void onActivated();
        virtual void onDeactivated();
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_SPAWNER