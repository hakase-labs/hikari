#ifndef HIKARI_CLIENT_GAME_OBJECTS_ITEMSPAWNER
#define HIKARI_CLIENT_GAME_OBJECTS_ITEMSPAWNER

#include "hikari/client/game/objects/Spawner.hpp"

#include <memory>
#include <string>

namespace hikari {

    class CollectableItem;
    
    class ItemSpawner : public Spawner {
    private:
        int spawnedItemId;
        std::string itemName;

        void handleEntityDeathEvent(EventDataPtr event);

    public:
        ItemSpawner(const std::string & itemNam);
        virtual ~ItemSpawner();

        virtual void performAction(GameWorld & world);
        virtual void attachEventListeners(EventManager & eventManager);
        virtual void detachEventListeners(EventManager & eventManager);

        //
        // GameObject overrides
        //
        virtual void onActivated();
        virtual void onDeactivated();
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_ITEMSPAWNER