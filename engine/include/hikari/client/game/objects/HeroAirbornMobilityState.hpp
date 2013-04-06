#include "hikari/client/game/objects/Hero.hpp"

namespace hikari {

    class Hero::AirbornMobilityState : public Hero::MobilityState {
    public:
        AirbornMobilityState(Hero & hero);
        virtual ~AirbornMobilityState();
        virtual void enter();
        virtual void exit();
        virtual StateChangeAction update(const float & dt);
    };

} // hikari