#ifndef HIKARI_CLIENT_GAME_OBJECTS_SPAWNER
#define HIKARI_CLIENT_GAME_OBJECTS_SPAWNER

#include "hikari/client/game/objects/GameObject.hpp"
#include "hikari/core/game/Direction.hpp"
#include "hikari/core/geom/Vector2D.hpp"

namespace hikari {
    
    /**
        A Spawner is responsible for spawning one or more objects in the game
        world. It keeps track of whether its spawn should be alive or not.
    */
    class Spawner : public GameObject {
    private:
        Direction direction;
        Vector2D position;

    public:
        Spawner();

        virtual ~Spawner() {
            // no-op
        }

        const Direction& getDirection() const;

        void setDirection(const Direction& direction);

        /**
            Gets the position where this Spawner's offspring will spawn.

            @return offspring's spawn position in world coordinates.
        */
        const Vector2D& getPosition() const;

        /**
            Sets the position where this Spawner's offspring will spawn.

            @param newPosition the offspring's spawn position in world 
            coordinates.
        */
        void setPosition(const Vector2D& newPosition);

        //
        // GameObject overrides
        //
        virtual void onActivated();
        virtual void onDeactivated();
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_SPAWNER