#include "hikari/client/game/objects/HeroTeleportingMobilityState.hpp"
#include "hikari/client/game/objects/HeroIdleMobilityState.hpp"

namespace hikari {

    Hero::TeleportingMobilityState::TeleportingMobilityState(Hero & hero)
        : MobilityState(hero)
        , morphingLimit(0.2167f) // ~13 frames
        , morphingCounter(0.0f)
    {

    }

    Hero::TeleportingMobilityState::~TeleportingMobilityState() {

    }

    void Hero::TeleportingMobilityState::enter() {
        hero.isTeleporting = true;
        hero.isMorphing = false;
        hero.chooseAnimation();
        hero.setVelocityX(0.0f);
        hero.setVelocityY(0.0f);
    }

    void Hero::TeleportingMobilityState::exit() {
        hero.isTeleporting = false;
        hero.isMorphing = false;
    }

    Hero::MobilityState::StateChangeAction Hero::TeleportingMobilityState::update(const float & dt) {
        if(hero.isMorphing) {
            if(morphingCounter == 0.0f) {
                hero.chooseAnimation();
            }

            morphingCounter += dt;

            if(morphingCounter >= morphingLimit) {
                // Time to change!
                hero.requestMobilityStateChange(std::unique_ptr<MobilityState>(new IdleMobilityState(hero)));
                // Emit event or play a sound
                return MobilityState::NEXT;
            }
        }

        return MobilityState::CONTINUE;
    }

} // hikari