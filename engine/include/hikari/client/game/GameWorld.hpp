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

namespace sf {
    class RenderTarget;
}

namespace hikari {

    class GameObject;
    class Hero;
    class CollectableItem;
    class Doodad;
    class Enemy;
    class Particle;
    class Projectile;

    class GameWorld : public Updatable {
    private:
        std::shared_ptr<Hero> player;
        std::queue<std::shared_ptr<GameObject>> queuedAdditions;
        std::queue<std::shared_ptr<GameObject>> queuedRemovals;
        std::vector<std::shared_ptr<GameObject>> activeObjects;
        std::unordered_map<int, std::shared_ptr<GameObject>> objectRegistry;
        bool gravityEnabled;

        void processAdditions();
        void processRemovals();

    public:
        GameWorld();
        virtual ~GameWorld();

        void queueObjectAddition(const std::shared_ptr<GameObject> &obj);
        void queueObjectRemoval(const std::shared_ptr<GameObject> &obj);

        std::shared_ptr<CollectableItem> spawnCollectableItem(const std::string & name /* CollectableItemInstanceConfig instanceConfig */) const;
        std::shared_ptr<Doodad> spawnDoodad(const std::string & name /* DoodadInstanceConfig instanceConfig */) const;
        std::shared_ptr<Enemy> spawnEnemy(const std::string & name /* EnemyInstanceConfig instanceConfig */) const;
        std::shared_ptr<Particle> spawnParticle(const std::string & name /* ParticleInstanceConfig instanceConfig */) const;
        std::shared_ptr<Projectile> spawnProjectile(const std::string & name /* ProjectileInstanceConfig instanceConfig */) const;

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
        virtual void update(const float &dt);
    };

}

#endif // HIKARI_CLIENT_GAME_GAMEWORLD
