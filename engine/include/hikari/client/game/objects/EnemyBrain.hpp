#ifndef HIKARI_CLIENT_GAME_OBJECTS_ENEMYBRAIN
#define HIKARI_CLIENT_GAME_OBJECTS_ENEMYBRAIN

#include "hikari/client/game/objects/Enemy.hpp"
#include "hikari/core/util/Cloneable.hpp"

namespace hikari {

    class Movable;
    class CollisionInfo;

    /**
     * EnemyBrain is the base class for all Enemy behaviors. It is where the
     * logic that makes each enemy unique is implemented.
     *
     * @see ScriptedEnemyBrain
     */
    class EnemyBrain : public Cloneable<EnemyBrain> {
    private:
        /** A pointer back to the host Enemy object */
        Enemy* host;

    protected:
        /**
         * Gets the pointer to the host Enemy object.
         *
         * @see EnemyBrain::attach
         */
        Enemy* const getHost();

    public:
        /**
         * Default constructor.
         */
        EnemyBrain();

        /**
         * Copy constructor.
         */
        EnemyBrain(const EnemyBrain & proto);

        /**
         * Destructor.
         */
        virtual ~EnemyBrain();

        /**
         * Creates a new instance of the brain by copy-constructing it.
         *
         * @return a clone of the instance
         */
        virtual std::unique_ptr<EnemyBrain> clone() const;

        /**
         * Attaches the EnemyBrain to an instance of Enemy.
         * 
         * @param host a pointer to an instance of Enemy to attach to
         * @see EnemyBrain::detach
         * @see EnemyBrain::getHost
         */
        virtual void attach(Enemy* host);

        /**
         * Detaches the brain from its host.
         *
         * @see EnemyBrain::attach
         * @see EnemyBrain::getHost
         */
        virtual void detach();

        /**
         * Called when collisions take place with tiles in the world. Allows for
         * handling of collision response from within a brain.
         * 
         * @param body the host's body
         * @param info the collision information
         */
        virtual void handleCollision(Movable& body, CollisionInfo& info);

        /**
         * Called when collisions take place with other objects. Useful for
         * reacting to when the enemy touches the player or some other object.
         *
         * @param otherId [description]
         */
        virtual void handleObjectTouch(int otherId);

        /**
         * Updates the brain's logic.
         * 
         * @param dt time, in seconds, to update for
         */
        virtual void update(float dt);
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_ENEMYBRAIN
