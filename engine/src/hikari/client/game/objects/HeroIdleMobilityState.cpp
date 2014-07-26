#include "hikari/client/game/objects/HeroIdleMobilityState.hpp"
#include "hikari/client/game/objects/HeroAirbornMobilityState.hpp"
#include "hikari/client/game/objects/HeroWalkingMobilityState.hpp"
#include "hikari/client/game/objects/HeroSlidingMobilityState.hpp"

namespace hikari {

    Hero::IdleMobilityState::IdleMobilityState(Hero & hero)
        : MobilityState(hero)
    {

    }

    Hero::IdleMobilityState::~IdleMobilityState() {

    }

    void Hero::IdleMobilityState::enter() {
        hero.isStanding = true;
        hero.isFullyAccelerated = false;
        hero.chooseAnimation();
        hero.setVelocityX(0.0f);
    }

    void Hero::IdleMobilityState::exit() {
        hero.isStanding = false;
    }

    Hero::MobilityState::StateChangeAction Hero::IdleMobilityState::update(const float & dt) {
        if(hero.actionController) {
            auto const * controller = hero.actionController.get();

            if(hero.isDecelerating) {
                hero.isDecelerating = false;
                hero.chooseAnimation();
            }

            // Disappearing blocks, moving platform, who knows...
            if(!hero.body.isOnGround()) {
                hero.requestMobilityStateChange(std::unique_ptr<MobilityState>(new AirbornMobilityState(hero)));
                return MobilityState::NEXT;
            } else {
                if((controller->shouldMoveLeft() && !controller->shouldMoveRight())
                    || (controller->shouldMoveRight() && !controller->shouldMoveLeft())) {
                    hero.requestMobilityStateChange(std::unique_ptr<MobilityState>(new WalkingMobilityState(hero)));
                    return MobilityState::NEXT;
                }

                if(controller->shouldSlide() && hero.canSlide()) {
                    hero.requestMobilityStateChange(std::unique_ptr<MobilityState>(new SlidingMobilityState(hero)));
                    return MobilityState::NEXT;
                }

                if(hero.canJump() && controller->shouldJump() && !controller->shouldSlide()) {
                    hero.requestMobilityStateChange(std::unique_ptr<MobilityState>(new AirbornMobilityState(hero)));
                    return MobilityState::NEXT;
                }
            }
        }

        return MobilityState::CONTINUE;
    }

} // hikari
