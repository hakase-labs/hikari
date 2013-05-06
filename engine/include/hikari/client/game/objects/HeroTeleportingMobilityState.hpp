#include "hikari/client/game/objects/Hero.hpp"

namespace hikari {

    /**
     * Encapsulates the "teleporting" state that the Hero can be in. This class is a
     * private inner class of Hero.
     *
     * @see Hero
     * @see Hero::MobilityState
     */
    class Hero::TeleportingMobilityState : public Hero::MobilityState {
    private:
        float morphingLimit;
        float morphingCounter;
    public:
        TeleportingMobilityState(Hero & hero);
        virtual ~TeleportingMobilityState();
        virtual void enter();
        virtual void exit();
        virtual StateChangeAction update(const float & dt);
    };

} // hikari