#include <hikari/client/game/objects/ItemFactory.hpp>
#include <hikari/client/game/objects/GameObject.hpp>
#include <hikari/client/game/objects/CollectableItem.hpp>
#include <hikari/client/game/objects/effects/NothingEffect.hpp>
#include <hikari/client/game/objects/effects/ScriptedEffect.hpp>
#include <hikari/client/scripting/SquirrelService.hpp>

#include <hikari/core/game/AnimationLoader.hpp>
#include <hikari/core/game/map/Room.hpp>
#include <hikari/core/util/ImageCache.hpp>

namespace hikari {

    ItemFactory::ItemFactory(const std::shared_ptr<ImageCache>& imageCache, const std::shared_ptr<SquirrelService>& squirrel)
        : imageCache(imageCache)
        , squirrel(squirrel)
    {
        squirrel->runScriptFile("assets/scripts/EffectBase.nut");
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
        }

        return std::shared_ptr<CollectableItem>(nullptr);
    }

    std::shared_ptr<CollectableItem> ItemFactory::createETank() {
        auto item = std::make_shared<CollectableItem>(GameObject::generateObjectId(), nullptr, std::make_shared<ScriptedEffect>(*squirrel, "EffectBase"));

        // TODO: This is wrong for sure; need an AnimationCache to load these X_X
        item->setAnimationSet(AnimationLoader::loadSet("assets/animations/items.json"));
        item->setSpriteTexture(*imageCache->get("assets/images/sp-collectables.png"));
        item->changeAnimation("e-tank");
        item->setAgeless(true);

        return item;
    }

    std::shared_ptr<CollectableItem> ItemFactory::createExtraLife() {
        auto item = std::make_shared<CollectableItem>(GameObject::generateObjectId(), nullptr, std::make_shared<ScriptedEffect>(*squirrel, "EffectBase"));

        // TODO: This is wrong for sure; need an AnimationCache to load these X_X
        item->setAnimationSet(AnimationLoader::loadSet("assets/animations/items.json"));
        item->setSpriteTexture(*imageCache->get("assets/images/sp-collectables.png"));
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
    

} // hikari