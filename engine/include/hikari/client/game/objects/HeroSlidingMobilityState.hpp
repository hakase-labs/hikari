#include "hikari/client/game/objects/Hero.hpp"

namespace hikari {

    class Hero::SlidingMobilityState : public Hero::MobilityState {
    private:
        float slideDuration;
        float slideDurationThreshold;
        BoundingBoxF oldBoundingBox;
    public:
        SlidingMobilityState(Hero & hero);
        virtual ~SlidingMobilityState();
        virtual void enter();
        virtual void exit();
        virtual StateChangeAction update(const float & dt);
    };

} // hikari