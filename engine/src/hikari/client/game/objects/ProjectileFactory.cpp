#include "hikari/client/game/objects/ProjectileFactory.hpp"
#include "hikari/client/game/objects/GameObject.hpp"
#include "hikari/client/game/objects/Projectile.hpp"

#include "hikari/client/scripting/SquirrelService.hpp"

#include "hikari/core/game/AnimationLoader.hpp"
#include "hikari/core/util/AnimationSetCache.hpp"
#include "hikari/core/util/ImageCache.hpp"

namespace hikari {

    ProjectileFactory::ProjectileFactory(const std::weak_ptr<AnimationSetCache>& animationSetCache, const std::weak_ptr<ImageCache>& imageCache, const std::weak_ptr<SquirrelService>& squirrel)
        : Service()
        , animationSetCache(animationSetCache)
        , imageCache(imageCache)
        , squirrel(squirrel)
        , prototypeRegistry()
    {
         
    }
    
    ProjectileFactory::~ProjectileFactory() {

    }

    std::unique_ptr<Projectile> ProjectileFactory::create(const std::string& ProjectileType) {
        auto prototype = prototypeRegistry.find(ProjectileType);

        if(prototype != std::end(prototypeRegistry)) {
            return (*prototype).second->clone();
        } else {
            // TODO: Return a "default" item so no nullptrs will be made?
            return std::unique_ptr<Projectile>(nullptr);
        }
    }

    void ProjectileFactory::registerPrototype(const std::string & prototypeName, const std::shared_ptr<Projectile> & instance) {
        if(prototypeRegistry.find(prototypeName) == std::end(prototypeRegistry)) {
            prototypeRegistry.insert(std::make_pair(prototypeName, instance));
        } else {
            // Already registered; exception?
        }
    }
} // hikari