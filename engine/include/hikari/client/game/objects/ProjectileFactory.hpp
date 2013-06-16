#ifndef HIKARI_CLIENT_GAME_OBJECT_PROJECTILEFACTORY
#define HIKARI_CLIENT_GAME_OBJECT_PROJECTILEFACTORY

#include <memory>
#include <string>
#include <unordered_map>

#include "hikari/core/util/Service.hpp"

namespace hikari {

    class AnimationSetCache;
    class Projectile;
    class ImageCache;
    class SquirrelService;

    class ProjectileFactory : public Service {
    private:
        //
        // Fields
        //
        std::weak_ptr<AnimationSetCache> animationSetCache;
        std::weak_ptr<ImageCache> imageCache;
        std::weak_ptr<SquirrelService> squirrel;
        std::unordered_map<std::string, std::shared_ptr<Projectile>> prototypeRegistry;

    public:
        //
        // Constructor
        //
        ProjectileFactory(const std::weak_ptr<AnimationSetCache>& animationSetCache,
            const std::weak_ptr<ImageCache>& imageCache, const std::weak_ptr<SquirrelService>& squirrel);
        virtual ~ProjectileFactory();

        //
        // Methods
        //
        std::unique_ptr<Projectile> create(const std::string& enemyType);

        void registerPrototype(const std::string & prototypeName, const std::shared_ptr<Projectile> & instancee);
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECT_PROJECTILEFACTORY