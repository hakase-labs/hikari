#ifndef HIKARI_CLIENT_GAME_OBJECTS_ENEMYSPAWNER
#define HIKARI_CLIENT_GAME_OBJECTS_ENEMYSPAWNER

#include "hikari/client/game/objects/Spawner.hpp"

#include <memory>
#include <string>

namespace hikari {
    
    /**
     * A class responsible for spawning instances of an enemy.
     */
    class EnemySpawner : public Spawner {
    private:
        static const int DEFAULT_SPAWN_LIMIT;
        static const float DEFAULT_SPAWN_RATE;

    private:
        std::vector<int> spawnedEnemyIds;
        std::string enemyType;
        bool hasLivingSpawn; // Indicates whether ths spawner has living "children"
        int spawnLimit;      // How many enemies this spawner will spawn at once time
        float spawnRate;     // How long to wait before spawning another enemy
        float spawnRateAccumulator;

        void handleObjectRemovedEvent(EventDataPtr event);

    public:
        EnemySpawner(const std::string & enemyType, 
            int spawnLimit = DEFAULT_SPAWN_LIMIT, float spawnRate = DEFAULT_SPAWN_RATE);

        virtual ~EnemySpawner();

        virtual void performAction(GameWorld & world);
        virtual void attachEventListeners(EventBus & EventBus);
        virtual void detachEventListeners(EventBus & EventBus);

        void setSpawnLimit(int limit);
        void setSpawnRate(float rate);

        bool canSpawn() const;

        //
        // GameObject overrides
        //
        virtual void onActivated();
        virtual void onDeactivated();
        virtual void update(float dt);
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_ENEMYSPAWNER