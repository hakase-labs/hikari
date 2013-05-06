#include "hikari/client/game/objects/Hero.hpp"

namespace hikari {    

    class Hero::WalkingMobilityState : public Hero::MobilityState {
    private:
        int accelerationDelay;
        int accelerationDelayThreshold;
        bool isDecelerating;
        Direction lastDirection;
    public:
        WalkingMobilityState(Hero & hero);
        virtual ~WalkingMobilityState();
        virtual void enter();
        virtual void exit();
        virtual StateChangeAction update(const float & dt);
    };

} // hikari