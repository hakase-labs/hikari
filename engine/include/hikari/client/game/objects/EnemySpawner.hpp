#ifndef HIKARI_CLIENT_GAME_OBJECTS_ENEMYSPAWNER
#define HIKARI_CLIENT_GAME_OBJECTS_ENEMYSPAWNER

#include "hikari/client/game/objects/Spawner.hpp"

#include <memory>
#include <string>

namespace hikari {
    
    /**
     * A class responsible for spawning instances of Enemy. EnemySpawners can
     * spawn more than once Enemy instance at a specified rate.
     */
    class EnemySpawner : public Spawner {
    private:
        static const unsigned int DEFAULT_SPAWN_LIMIT;
        static const float DEFAULT_SPAWN_RATE;

    private:
        std::vector<int> spawnedEnemyIds;
        std::string enemyType;
        unsigned int spawnLimit;      // How many enemies this spawner will spawn at one time
        float spawnRate;              // How long to wait before spawning another enemy
        float spawnRateAccumulator;   // Counter between spawns

        void handleObjectRemovedEvent(EventDataPtr event);

    public:
        EnemySpawner(const std::string & enemyType, 
            unsigned int spawnLimit = DEFAULT_SPAWN_LIMIT, float spawnRate = DEFAULT_SPAWN_RATE);

        virtual ~EnemySpawner();

        virtual void performAction(GameWorld & world);
        virtual void attachEventListeners(EventBus & EventBus);
        virtual void detachEventListeners(EventBus & EventBus);

        /**
         * Sets the limit on the number of Enemies the EnemySpawner can produce.
         * This value must always be greater than 0.
         *
         * @param limit number of things to spawn (greater than 0)
         */
        void setSpawnLimit(unsigned int limit);

        /**
         * Sets the rate, in seconds, that Enemies can be spawned. This is the
         * amount of time between spawns. For example, if the spawn rate is 1.0
         * then the spawner will wait 1 second before trying to spawn another
         * Enemy.
         *
         * The spawn rate cannot be less than 0 and therefore must be either 0
         * or a positive value. Negative values will be treated as 0.
         *
         * @param rate the rate (in seconds) to spawn Enemies at
         */
        void setSpawnRate(float rate);

        /**
         * Determines if an Enemy can be spawned. This takes into account the
         * spawn limit and rate for the EnemySpawner.
         *
         * @return true if an Enemy can be spawned by this EnemySpawner, false
         *         otherwise
         */
        virtual bool canSpawn() const;

        //
        // GameObject overrides
        //
        virtual void onActivated();
        virtual void onDeactivated();
        virtual void update(float dt);
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_ENEMYSPAWNER