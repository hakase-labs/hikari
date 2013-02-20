#ifndef HIKARI_CLIENT_GAME_OBJECTS_ITEMSPAWNER
#define HIKARI_CLIENT_GAME_OBJECTS_ITEMSPAWNER

#include "hikari/client/game/objects/Spawner.hpp"

#include <string>

namespace hikari {
    
    class ItemSpawner : public Spawner {
    private:
        std::string itemName;

    public:
        ItemSpawner(const std::string & itemNam);
        virtual ~ItemSpawner();

        //
        // GameObject overrides
        //
        virtual void onActivated();
        virtual void onDeactivated();
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_ITEMSPAWNER