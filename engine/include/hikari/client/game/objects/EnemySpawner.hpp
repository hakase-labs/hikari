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
        std::vector<int> spawnedEnemyIds;
        std::string enemyType;
        bool hasLivingSpawn; // Indicates whether ths spawner has living "children"

        void handleObjectRemovedEvent(EventDataPtr event);

    public:
        EnemySpawner(const std::string & enemyType);
        virtual ~EnemySpawner();

        virtual void performAction(GameWorld & world);
        virtual void attachEventListeners(EventManager & eventManager);
        virtual void detachEventListeners(EventManager & eventManager);

        //
        // GameObject overrides
        //
        virtual void onActivated();
        virtual void onDeactivated();
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_ENEMYSPAWNER