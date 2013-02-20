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
        if("eTank" == itemType) {
            return createETank();
        } else if("extraLife" == itemType) {
            return createExtraLife();
        } else if("weaponEnergyLarge" == itemType) {
            return createHealthEnergyLarge();
        } else if("weaponEnergySmall" == itemType) {
            return createWeaponEnergySmall();
        } else if("healthEnergyLarge" == itemType) {
            return createHealthEnergyLarge();
        } else if("healthEnergySmall" == itemType) {
            return createHealthEnergySmall();
        } else {
            auto prototype = prototypeRegistry.find(itemType);

            if(prototype != std::end(prototypeRegistry)) {
                return (*prototype).second->clone();
            }
        }

        return std::shared_ptr<CollectableItem>(nullptr);
    }

    std::shared_ptr<CollectableItem> ItemFactory::createETank() {
        auto effect = std::make_shared<ScriptedEffect>(*squirrel, "AddETankEffect");
        auto item = std::make_shared<CollectableItem>(GameObject::generateObjectId(), nullptr, effect);

        item->setAnimationSet(animationSetCache->get("assets/animations/items.json"));
        item->setSpriteTexture(imageCache->get("assets/images/sp-collectables.png"));
        item->changeAnimation("e-tank");
        item->setAgeless(true);

        return item;
    }

    std::shared_ptr<CollectableItem> ItemFactory::createExtraLife() {
        auto effect = std::make_shared<ScriptedEffect>(*squirrel, "ExtraLifeEffect");
        auto item = std::make_shared<CollectableItem>(GameObject::generateObjectId(), nullptr, effect);

        item->setAnimationSet(animationSetCache->get("assets/animations/items.json"));
        item->setSpriteTexture(imageCache->get("assets/images/sp-collectables.png"));
        item->changeAnimation("extra-life-rockman");
        item->setAgeless(true);

        return item;
    }
    
    std::shared_ptr<CollectableItem> ItemFactory::createWeaponEnergyLarge() {
        auto item = std::make_shared<CollectableItem>(GameObject::generateObjectId(), nullptr, std::make_shared<NothingEffect>());

        return item;
    }
    
    std::shared_ptr<CollectableItem> ItemFactory::createWeaponEnergySmall() {
        auto item = std::make_shared<CollectableItem>(GameObject::generateObjectId(), nullptr, std::make_shared<NothingEffect>());

        return item;
    }
    
    std::shared_ptr<CollectableItem> ItemFactory::createHealthEnergyLarge() {
        auto item = std::make_shared<CollectableItem>(GameObject::generateObjectId(), nullptr, std::make_shared<NothingEffect>());

        return item;
    }
    
    std::shared_ptr<CollectableItem> ItemFactory::createHealthEnergySmall() {
        auto item = std::make_shared<CollectableItem>(GameObject::generateObjectId(), nullptr, std::make_shared<NothingEffect>());

        return item;
    }
    
    void ItemFactory::registerPrototype(const std::string & prototypeName, const std::shared_ptr<CollectableItem> & instance) {
        if(prototypeRegistry.find(prototypeName) == std::end(prototypeRegistry)) {
            prototypeRegistry.insert(std::make_pair(prototypeName, instance));
        } else {
            // Already registered; exception?
        }
    }
} // hikari