#include "hikari/client/game/objects/Hero.hpp"
#include "hikari/core/math/Vector2.hpp"

namespace hikari {

    class Hero::DamagedMobilityState : public Hero::MobilityState {
    private:
        float stunnedTimer;
        Vector2<float> horizontalVelocity;

    public:
        DamagedMobilityState(Hero & hero);
        virtual ~DamagedMobilityState();
        virtual void enter();
        virtual void exit();
        virtual StateChangeAction update(const float & dt);
    };

} // hikari