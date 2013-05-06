#include "hikari/client/game/objects/Hero.hpp"

namespace hikari {

    class Hero::IdleMobilityState : public Hero::MobilityState {
    public:
        IdleMobilityState(Hero & hero);
        virtual ~IdleMobilityState();
        virtual void enter();
        virtual void exit();
        virtual StateChangeAction update(const float & dt);
    };

} // hikari