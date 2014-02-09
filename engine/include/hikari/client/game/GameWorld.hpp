#ifndef HIKARI_CLIENT_GAME_GAMEWORLD
#define HIKARI_CLIENT_GAME_GAMEWORLD

#include "hikari/core/game/Updatable.hpp"
#include "hikari/core/math/Vector2.hpp"
#include "hikari/core/game/Direction.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>
#include <list>

namespace sf {
    class RenderTarget;
}

namespace hikari {
    
    class Room;
    class GameObject;
    class Hero;
    class CollectableItem;
    class ItemFactory;
    class Doodad;
    class Entity;
    class Enemy;
    class EnemyFactory;
    class Particle;
    class ParticleFactory;
    class Projectile;
    class ProjectileFactory;
    class EventBus;

    class GameWorld : public Updatable {
    private:
        std::weak_ptr<EventBus> eventBus;
        std::shared_ptr<Hero> player;
        std::shared_ptr<Room> currentRoom;
        std::weak_ptr<ItemFactory> itemFactory;
        std::weak_ptr<EnemyFactory> enemyFactory;
        std::weak_ptr<ParticleFactory> particleFactory;
        std::weak_ptr<ProjectileFactory> projectileFactory;
        std::list<std::shared_ptr<GameObject>> queuedAdditions;
        std::list<std::shared_ptr<GameObject>> queuedRemovals;
        std::vector<std::shared_ptr<GameObject>> activeObjects;

        std::list<std::shared_ptr<CollectableItem>> queuedItemAdditions;
        std::list<std::shared_ptr<CollectableItem>> queuedItemRemovals;
        std::vector<std::shared_ptr<CollectableItem>> activeItems;

        std::list<std::shared_ptr<Enemy>> queuedEnemyAdditions;
        std::list<std::shared_ptr<Enemy>> queuedEnemyRemovals;
        std::vector<std::shared_ptr<Enemy>> activeEnemies;

        std::list<std::shared_ptr<Particle>> queuedParticleAdditions;
        std::list<std::shared_ptr<Particle>> queuedParticleRemovals;
        std::vector<std::shared_ptr<Particle>> activeParticles;

        std::list<std::shared_ptr<Projectile>> queuedProjectileAdditions;
        std::list<std::shared_ptr<Projectile>> queuedProjectileRemovals;
        std::vector<std::shared_ptr<Projectile>> activeProjectiles;

        std::unordered_map<int, std::shared_ptr<GameObject>> objectRegistry;
        bool gravityEnabled;

        void processAdditions();
        void processRemovals();

    public:
        GameWorld();
        virtual ~GameWorld();

        void setEventBus(const std::weak_ptr<EventBus> & eventBus);
        const std::weak_ptr<EventBus> & getEventBus() const;

        void setCurrentRoom(const std::shared_ptr<Room> & room);
        const std::shared_ptr<Room> & getCurrentRoom() const;

        void setItemFactory(const std::weak_ptr<ItemFactory> & itemFactory);
        void setEnemyFactory(const std::weak_ptr<EnemyFactory> & enemyFactory);
        void setParticleFactory(const std::weak_ptr<ParticleFactory> & particleFactory);
        void setProjectileFactory(const std::weak_ptr<ProjectileFactory> & projectileFactory);

        void queueObjectAddition(const std::shared_ptr<GameObject> &obj);
        void queueObjectAddition(const std::shared_ptr<CollectableItem> &obj);
        void queueObjectAddition(const std::shared_ptr<Enemy> &obj);
        void queueObjectAddition(const std::shared_ptr<Particle> &obj);
        void queueObjectAddition(const std::shared_ptr<Projectile> &obj);

        void queueObjectRemoval(const std::shared_ptr<GameObject> &obj);
        void queueObjectRemoval(const std::shared_ptr<CollectableItem> &obj);
        void queueObjectRemoval(const std::shared_ptr<Enemy> &obj);
        void queueObjectRemoval(const std::shared_ptr<Particle> &obj);
        void queueObjectRemoval(const std::shared_ptr<Projectile> &obj);

        void removeAllObjects();

        std::shared_ptr<CollectableItem> spawnCollectableItem(const std::string & name /* CollectableItemInstanceConfig instanceConfig */) const;
        std::unique_ptr<Doodad> spawnDoodad(const std::string & name /* DoodadInstanceConfig instanceConfig */) const;
        std::unique_ptr<Enemy> spawnEnemy(const std::string & name /* EnemyInstanceConfig instanceConfig */) const;
        std::unique_ptr<Particle> spawnParticle(const std::string & name /* ParticleInstanceConfig instanceConfig */) const;
        std::unique_ptr<Projectile> spawnProjectile(const std::string & name /* ProjectileInstanceConfig instanceConfig */) const;

        const std::weak_ptr<GameObject> getObjectById(int id) const;
        const std::vector<std::shared_ptr<CollectableItem>> & getActiveItems() const;
        const std::vector<std::shared_ptr<Enemy>> & getActiveEnemies() const;
        const std::vector<std::shared_ptr<Particle>> & getActiveParticles() const;
        const std::vector<std::shared_ptr<Projectile>> & getActiveProjectiles() const;

        std::vector<std::shared_ptr<Entity>> getObstacles() const;

        void setPlayer(const std::shared_ptr<Hero>& player);

        /**
            Gets the player's current position in absolute coordinates.

            @return player's current position
        */
        const Vector2<float> getPlayerPosition() const;

        /**
            Gets the direction that the player is currently facing.

            @return player's currently faced direction
        */
        const Direction getPlayerFacingDirection() const;

        void render(sf::RenderTarget &target);
        virtual void update(float dt);
    };

}

#endif // HIKARI_CLIENT_GAME_GAMEWORLD
