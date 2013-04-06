#include "hikari/client/game/objects/Hero.hpp"

namespace hikari {

    class Hero::ClimbingMobilityState : public Hero::MobilityState {
    public:
        ClimbingMobilityState(Hero & hero);
        virtual ~ClimbingMobilityState();
        virtual void enter();
        virtual void exit();
        virtual StateChangeAction update(const float & dt);
    };

} // hikari