#include "hikari/client/game/objects/HeroSlidingMobilityState.hpp"
#include "hikari/client/game/objects/HeroIdleMobilityState.hpp"
#include "hikari/client/game/objects/HeroWalkingMobilityState.hpp"
#include "hikari/client/game/objects/HeroAirbornMobilityState.hpp"

namespace hikari {

    Hero::SlidingMobilityState::SlidingMobilityState(Hero & hero)
        : MobilityState(hero)
        , oldBoundingBox(hero.getBoundingBox())
        , slideDuration(0.0f)
        , slideDurationThreshold(1.0f / 60.0f * 19.0f) // 19 frames, 0.3166 seconds
    {

    }

    Hero::SlidingMobilityState::~SlidingMobilityState() {

    }

    void Hero::SlidingMobilityState::enter() {
        slideDuration = 0.0f;

        hero.isWalking = false;
        hero.isStanding = false;
        hero.isAirborn = false;
        hero.isFullyAccelerated = true;
        hero.isSliding = true;
        hero.body.setApplyHorizontalVelocity(true);
        hero.body.setApplyVerticalVelocity(true);

        oldBoundingBox = hero.getBoundingBox();

        BoundingBoxF newBoundingBox(oldBoundingBox);
        newBoundingBox.setHeight(16.0f);
        newBoundingBox.setWidth(16.0f);
        newBoundingBox.setOrigin(8.0f, 14.0f);
        newBoundingBox.setBottom(oldBoundingBox.getBottom());

        hero.setBoundingBox(newBoundingBox);

        hero.chooseAnimation();
    }

    void Hero::SlidingMobilityState::exit() {
        const auto currentBoundingBox = hero.getBoundingBox();

        BoundingBoxF restoredBoundingBox(oldBoundingBox);
        restoredBoundingBox.setBottom(currentBoundingBox.getBottom());
        restoredBoundingBox.setPosition(currentBoundingBox.getPosition());

        hero.setBoundingBox(restoredBoundingBox);

        hero.isSliding = false;
        hero.isInTunnel = false;
    }

    Hero::MobilityState::StateChangeAction Hero::SlidingMobilityState::update(const float & dt) {
        slideDuration += dt;

        if(auto const * controller = hero.actionController.get()) {
            if(hero.isInTunnel || (slideDuration < slideDurationThreshold && (controller->shouldSlide() || !controller->shouldStopSliding()))) {
                if(controller->shouldMoveLeft()) {
                    hero.setDirection(Directions::Left);
                }

                if(controller->shouldMoveRight()) {
                    hero.setDirection(Directions::Right);
                }

                auto const direction = hero.getDirection();

                if(direction == Directions::Left) {
                    hero.setVelocityX(-(hero.slideVelocity.getX()));
                } else {
                    hero.setVelocityX(hero.slideVelocity.getX());
                }
            } else {
                if(!controller->shouldMoveLeft() && !controller->shouldMoveRight()){
                    hero.isDecelerating = true;
                    hero.requestMobilityStateChange(std::unique_ptr<MobilityState>(new IdleMobilityState(hero)));
                    return MobilityState::NEXT;
                }

                if((controller->shouldMoveLeft() && !controller->shouldMoveRight())
                    || (controller->shouldMoveRight() && !controller->shouldMoveLeft())) {
                        hero.isFullyAccelerated = true;
                        hero.requestMobilityStateChange(std::unique_ptr<MobilityState>(new WalkingMobilityState(hero)));
                        return MobilityState::NEXT;
                }

                if(hero.canJump() && controller->shouldJump()) {
                    hero.requestMobilityStateChange(std::unique_ptr<MobilityState>(new AirbornMobilityState(hero)));
                    return MobilityState::NEXT;
                }

                if(!hero.body.isOnGround()) {
                    hero.requestMobilityStateChange(std::unique_ptr<MobilityState>(new AirbornMobilityState(hero)));
                    return MobilityState::NEXT;
                }

                hero.isDecelerating = true;
                hero.requestMobilityStateChange(std::unique_ptr<MobilityState>(new IdleMobilityState(hero)));
                return MobilityState::NEXT;
            }
        }

        return MobilityState::CONTINUE;
    }

} // hikari