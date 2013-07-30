#include "hikari/client/game/objects/Hero.hpp"
#include "hikari/core/geom/BoundingBox.hpp"

namespace hikari {

    class Hero::ClimbingMobilityState : public Hero::MobilityState {
    private:
        const BoundingBox<float> climbableRegion;
        
    public:
        ClimbingMobilityState(Hero & hero, const BoundingBox<float> & climbableRegion);
        virtual ~ClimbingMobilityState();
        virtual void enter();
        virtual void exit();
        virtual StateChangeAction update(const float & dt);
    };

} // hikari