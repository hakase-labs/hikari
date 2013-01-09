#ifndef HIKARI_CLIENT_GAME_OBJECT_ITEMFACTORY
#define HIKARI_CLIENT_GAME_OBJECT_ITEMFACTORY

#include <memory>
#include <string>

namespace hikari {

    class AnimationSetCache;
    class CollectableItem;
    class ImageCache;
    class SquirrelService;

    class ItemFactory {
    private:
        //
        // Fields
        //
        std::shared_ptr<AnimationSetCache> animationSetCache;
        std::shared_ptr<ImageCache> imageCache;
        std::shared_ptr<SquirrelService> squirrel;

        //
        // Methods
        //
        std::shared_ptr<CollectableItem> createETank();
        std::shared_ptr<CollectableItem> createExtraLife();
        std::shared_ptr<CollectableItem> createWeaponEnergyLarge();
        std::shared_ptr<CollectableItem> createWeaponEnergySmall();
        std::shared_ptr<CollectableItem> createHealthEnergyLarge();
        std::shared_ptr<CollectableItem> createHealthEnergySmall();

    public:
        //
        // Constructor
        //
        ItemFactory(const std::shared_ptr<AnimationSetCache>& animationSetCache, const std::shared_ptr<ImageCache>& imageCache, const std::shared_ptr<SquirrelService>& squirrel);

        //
        // Methods
        //
        std::shared_ptr<CollectableItem> createItem(const std::string& itemType);
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECT_ITEMFACTORY