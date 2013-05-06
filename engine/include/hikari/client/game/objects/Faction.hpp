#ifndef HIKARI_CLIENT_GAME_OBJECTS_FACTION
#define HIKARI_CLIENT_GAME_OBJECTS_FACTION

namespace hikari {
namespace Faction {

    /**
     * An enumeration of differen factions to be used by game objects. Factions 
     * allow objects to be grouped together in such a way that it makes it 
     * easier to determine what happens when the objects interact with each other.
     */
    enum Type {
        World = 0,
        Hero = 1,
        Enemy = 2
    };

} // hikari::Faction
} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_FACTION
