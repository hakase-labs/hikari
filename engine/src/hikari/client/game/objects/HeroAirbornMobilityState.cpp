#include "hikari/client/game/objects/HeroAirbornMobilityState.hpp"
#include "hikari/client/game/objects/HeroWalkingMobilityState.hpp"
#include "hikari/client/game/objects/HeroIdleMobilityState.hpp"

namespace hikari {

    Hero::AirbornMobilityState::AirbornMobilityState(Hero & hero)
        : MobilityState(hero)
    {

    }

    Hero::AirbornMobilityState::~AirbornMobilityState() {

    }

    void Hero::AirbornMobilityState::enter() {
        hero.isAirborn = true;
        hero.body.setApplyHorizontalVelocity(true);

        if(hero.actionController) {
            auto const * controller = hero.actionController.get();
            if(controller->shouldJump() && !hero.isFalling) {
                hero.setVelocityY(hero.jumpVelocity.getY());
                hero.isJumping = true;
            } else if(!hero.body.isOnGround()) {
                hero.isFalling = true;
            }
        }

        hero.chooseAnimation();
    }

    void Hero::AirbornMobilityState::exit() {
        hero.isJumping = false;
    }

    Hero::MobilityState::StateChangeAction Hero::AirbornMobilityState::update(const float & dt) {
        if(hero.actionController) {
            auto const * controller = hero.actionController.get();

            // Handle movement in the air
            if(controller->shouldMoveLeft()) {
                hero.setDirection(Directions::Left);
                hero.setVelocityX(-(hero.walkVelocity.getX()));
            } else if(controller->shouldMoveRight()) {
                hero.setDirection(Directions::Right);
                hero.setVelocityX(hero.walkVelocity.getX());
            } else {
                hero.setVelocityX(0.0f);
            }

            if(hero.getVelocityY() > 0) {
                hero.countDecendingFrames++;
            } else if(hero.getVelocityY() < 0) {
                hero.countAscendingFrames++;
            }

            //
            // Other state conditions
            //
            if(hero.isJumping && controller->shouldStopJumping() && hero.getVelocityY() < 0.0f) {
                hero.isJumping = false;
                hero.isFalling = true;
                hero.setVelocityY(hero.suddenFallVelocity.getY());
            }

            if(hero.body.isOnGround()) {
                if(controller->shouldMoveLeft() || controller->shouldMoveRight()) {
                    hero.isFullyAccelerated = true;
                    hero.requestMobilityStateChange(std::unique_ptr<MobilityState>(new WalkingMobilityState(hero)));
                    return MobilityState::NEXT;
                } else {
                    hero.requestMobilityStateChange(std::unique_ptr<MobilityState>(new IdleMobilityState(hero)));
                    return MobilityState::NEXT;
                }
            }
        }

        return MobilityState::CONTINUE;
    }

} // hikari