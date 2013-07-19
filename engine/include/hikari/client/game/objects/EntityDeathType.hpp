#ifndef HIKARI_CLIENT_GAME_OBJECTS_ENTITYDEATHTYPE
#define HIKARI_CLIENT_GAME_OBJECTS_ENTITYDEATHTYPE

namespace hikari {

    /**
     * EntityDeathType is an enum of values describing the "type" of death an
     * Entity should experience. Basically, the kind of explosion that should
     * appear at death.
     */
    namespace EntityDeathType {
        enum Type {
            Nothing,    /// Nothing should appear
            Small,      /// A small radial explosion should appear
            Hero        /// 16 small radial explosions (Rock or Robot Master)
        };
    }

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_ENTITYDEATHTYPE