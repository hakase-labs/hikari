#include "hikari/client/game/objects/ItemFactory.hpp"
#include "hikari/client/game/objects/GameObject.hpp"
#include "hikari/client/game/objects/CollectableItem.hpp"
#include "hikari/client/game/objects/effects/NothingEffect.hpp"
#include "hikari/client/game/objects/effects/ScriptedEffect.hpp"
#include "hikari/client/scripting/SquirrelService.hpp"

#include "hikari/core/game/AnimationLoader.hpp"
#include "hikari/core/game/map/Room.hpp"
#include "hikari/core/util/AnimationSetCache.hpp"
#include "hikari/core/util/ImageCache.hpp"

namespace hikari {

    ItemFactory::ItemFactory(const std::shared_ptr<AnimationSetCache>& animationSetCache, const std::shared_ptr<ImageCache>& imageCache, const std::shared_ptr<SquirrelService>& squirrel)
        : Service()
        , animationSetCache(animationSetCache)
        , imageCache(imageCache)
        , squirrel(squirrel)
        , prototypeRegistry()
    {
         
    }
    
    ItemFactory::~ItemFactory() {

    }

    std::shared_ptr<CollectableItem> ItemFactory::createItem(const std::string& itemType) {
        auto prototype = prototypeRegistry.find(itemType);

        if(prototype != std::end(prototypeRegistry)) {
            return (*prototype).second->clone();
        } else {
            // TODO: Return a "default" item so no nullptrs will be made?
            return std::shared_ptr<CollectableItem>(nullptr);
        }
    }

    void ItemFactory::registerPrototype(const std::string & prototypeName, const std::shared_ptr<CollectableItem> & instance) {
        if(prototypeRegistry.find(prototypeName) == std::end(prototypeRegistry)) {
            prototypeRegistry.insert(std::make_pair(prototypeName, instance));
        } else {
            // Already registered; exception?
        }
    }
} // hikari