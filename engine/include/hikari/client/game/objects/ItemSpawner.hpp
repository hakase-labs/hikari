#ifndef HIKARI_CLIENT_GAME_OBJECTS_ITEMSPAWNER
#define HIKARI_CLIENT_GAME_OBJECTS_ITEMSPAWNER

#include "hikari/client/game/objects/Spawner.hpp"

#include <memory>
#include <string>

namespace hikari {

    class CollectableItem;
    
    class ItemSpawner : public Spawner {
    private:
        std::string itemName;

    public:
        ItemSpawner(const std::string & itemNam);
        virtual ~ItemSpawner();

        virtual void performAction(GameWorld & world);

        //
        // GameObject overrides
        //
        virtual void onActivated();
        virtual void onDeactivated();
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_ITEMSPAWNER