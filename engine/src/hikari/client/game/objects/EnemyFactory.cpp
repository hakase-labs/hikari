#include "hikari/client/game/objects/EnemyFactory.hpp"
#include "hikari/client/game/objects/GameObject.hpp"
#include "hikari/client/game/objects/Enemy.hpp"

#include "hikari/client/scripting/SquirrelService.hpp"

#include "hikari/core/game/AnimationLoader.hpp"
#include "hikari/core/game/map/Room.hpp"
#include "hikari/core/util/AnimationSetCache.hpp"
#include "hikari/core/util/ImageCache.hpp"

namespace hikari {

    EnemyFactory::EnemyFactory(const std::weak_ptr<AnimationSetCache>& animationSetCache, const std::weak_ptr<ImageCache>& imageCache, const std::weak_ptr<SquirrelService>& squirrel)
        : Service()
        , animationSetCache(animationSetCache)
        , imageCache(imageCache)
        , squirrel(squirrel)
        , prototypeRegistry()
    {
         
    }
    
    EnemyFactory::~EnemyFactory() {

    }

    std::unique_ptr<Enemy> EnemyFactory::create(const std::string& enemyType) {
        auto prototype = prototypeRegistry.find(enemyType);

        if(prototype != std::end(prototypeRegistry)) {
            return (*prototype).second->clone();
        } else {
            // TODO: Return a "default" item so no nullptrs will be made?
            return std::unique_ptr<Enemy>(nullptr);
        }
    }

    void EnemyFactory::registerPrototype(const std::string & prototypeName, const std::shared_ptr<Enemy> & instance) {
        if(prototypeRegistry.find(prototypeName) == std::end(prototypeRegistry)) {
            prototypeRegistry.insert(std::make_pair(prototypeName, instance));
        } else {
            // Already registered; exception?
        }
    }
} // hikari