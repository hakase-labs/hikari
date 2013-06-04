#ifndef HIKARI_CLIENT_GAME_OBJECT_ENEMYFACTORY
#define HIKARI_CLIENT_GAME_OBJECT_ENEMYFACTORY

#include <memory>
#include <string>
#include <unordered_map>

#include "hikari/core/util/Service.hpp"

namespace hikari {

    class AnimationSetCache;
    class Enemy;
    class ImageCache;
    class SquirrelService;

    class EnemyFactory : public Service {
    private:
        //
        // Fields
        //
        std::weak_ptr<AnimationSetCache> animationSetCache;
        std::weak_ptr<ImageCache> imageCache;
        std::weak_ptr<SquirrelService> squirrel;
        std::unordered_map<std::string, std::shared_ptr<Enemy>> prototypeRegistry;

    public:
        //
        // Constructor
        //
        EnemyFactory(const std::weak_ptr<AnimationSetCache>& animationSetCache, const std::weak_ptr<ImageCache>& imageCache, const std::weak_ptr<SquirrelService>& squirrel);
        virtual ~EnemyFactory();

        //
        // Methods
        //
        std::unique_ptr<Enemy> create(const std::string& enemyType);

        void registerPrototype(const std::string & prototypeName, const std::shared_ptr<Enemy> & instancee);
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECT_ENEMYFACTORY