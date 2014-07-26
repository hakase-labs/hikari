#include "hikari/client/game/objects/HeroWalkingMobilityState.hpp"
#include "hikari/client/game/objects/HeroAirbornMobilityState.hpp"
#include "hikari/client/game/objects/HeroIdleMobilityState.hpp"
#include "hikari/client/game/objects/HeroSlidingMobilityState.hpp"
#include "hikari/client/game/objects/PalettedAnimatedSprite.hpp"
#include "hikari/core/game/SpriteAnimator.hpp"
#include "hikari/core/util/Log.hpp"

namespace hikari {

    Hero::WalkingMobilityState::WalkingMobilityState(Hero & hero)
        : MobilityState(hero)
        , accelerationDelay(0)
        , accelerationDelayThreshold(6) // stall for 6 frames
        , isDecelerating(false)
        , lastDirection(Directions::None)
    {
        // HIKARI_LOG(debug4) << "WalkingMobilityState()";
    }

    Hero::WalkingMobilityState::~WalkingMobilityState() {
        // HIKARI_LOG(debug4) << "~WalkingMobilityState()";
    }

    void Hero::WalkingMobilityState::enter() {
        hero.isWalking = true;
        hero.isAirborn = false;
        hero.isStanding = false;
        hero.isDecelerating = false;
        isDecelerating = false;
    }

    void Hero::WalkingMobilityState::exit() {
        hero.isWalking = false;
    }

    Hero::MobilityState::StateChangeAction Hero::WalkingMobilityState::update(const float & dt) {
        if(hero.actionController) {
            auto const * controller = hero.actionController.get();

            lastDirection = hero.getDirection();

            if(controller->shouldMoveLeft() && !controller->shouldMoveRight()) {
                hero.setDirection(Directions::Left);
                hero.setVelocityX(-(hero.walkVelocity.getX()));
            }

            if(controller->shouldMoveRight() && !controller->shouldMoveLeft()) {
                hero.setDirection(Directions::Right);
                hero.setVelocityX(hero.walkVelocity.getX());
            }

            // Handle direction switching (reset acceleration)
            if(hero.getDirection() != lastDirection) {
                hero.isFullyAccelerated = false;
                hero.getAnimatedSprite()->rewind();
                accelerationDelay = 0;
            }

            // Handle acceleration
            if(!hero.isFullyAccelerated) {
                hero.body.setApplyHorizontalVelocity(accelerationDelay == 0);

                hero.isFullyAccelerated = !(accelerationDelay < accelerationDelayThreshold);
                accelerationDelay += 1; // dt;

                HIKARI_LOG(debug4) << "Accellerating...";

                hero.chooseAnimation();
            } else {
                hero.body.setApplyHorizontalVelocity(true);
                hero.chooseAnimation();
            }

            //
            // Other state conditions
            //
            bool movingInBothDirections = controller->shouldMoveLeft() && controller->shouldMoveRight();
            bool movingInNoDirections = !controller->shouldMoveLeft() && !controller->shouldMoveRight();

            // Moving in both directions at the same time causes Rock to idle.
            // Or not moving in either direction, so go back to standing.
            if(movingInBothDirections || movingInNoDirections) {
                if(!isDecelerating) {
                    isDecelerating = true;
                } else {
                    hero.isDecelerating = true;
                    hero.requestMobilityStateChange(std::unique_ptr<MobilityState>(new IdleMobilityState(hero)));
                    return MobilityState::NEXT;
                }
            }

            if(controller->shouldSlide() && hero.canSlide()) {
                hero.requestMobilityStateChange(std::unique_ptr<MobilityState>(new SlidingMobilityState(hero)));
                return MobilityState::NEXT;
            }

            // Trying to jump so go ahead and jump.
            if(hero.canJump() && controller->shouldJump() && !controller->shouldSlide()) {
                hero.requestMobilityStateChange(std::unique_ptr<MobilityState>(new AirbornMobilityState(hero)));
                return MobilityState::NEXT;
            }

            if(!hero.body.isOnGround()) {
                hero.requestMobilityStateChange(std::unique_ptr<MobilityState>(new AirbornMobilityState(hero)));
                return MobilityState::NEXT;
            }
        }

        return MobilityState::CONTINUE;
    }

} // hikari
