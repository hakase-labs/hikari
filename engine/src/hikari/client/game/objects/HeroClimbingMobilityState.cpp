#include "hikari/client/game/objects/HeroClimbingMobilityState.hpp"
#include "hikari/client/game/objects/HeroAirbornMobilityState.hpp"
#include "hikari/client/game/objects/HeroIdleMobilityState.hpp"
#include "hikari/client/game/objects/AnimatedSprite.hpp"
#include "hikari/core/game/SpriteAnimator.hpp"
#include "hikari/core/game/map/Room.hpp"
#include "hikari/core/util/Log.hpp"

namespace hikari {

    Hero::ClimbingMobilityState::ClimbingMobilityState(Hero & hero, const BoundingBox<float> & climbableRegion)
        : MobilityState(hero)
        , climbableRegion(climbableRegion)
    {

    }

    Hero::ClimbingMobilityState::~ClimbingMobilityState() {

    }

    void Hero::ClimbingMobilityState::enter() {
        hero.isClimbing = true;
        hero.isOnLadder = true;
        hero.isFalling = false;
        hero.isAirborn = false;
        hero.isFullyAccelerated = false;
        hero.isJumping = false;
        hero.isStanding = false;
        hero.isDecelerating = false;
        hero.isWalking = false;

        hero.setVelocityX(0.0f);
        hero.setVelocityY(0.0f);
        hero.body.setGravitated(false);
        hero.body.setTreatLadderTopAsGround(false);

        hero.body.setPosition(climbableRegion.getLeft() + (climbableRegion.getWidth() / 2), hero.getPosition().getY());
    }

    void Hero::ClimbingMobilityState::exit() {
        hero.isClimbing = false;
        hero.isOnLadder = false;
        hero.body.setGravitated(true);
        hero.body.setTreatLadderTopAsGround(true);
        hero.getAnimatedSprite()->unpause();
    }

    Hero::MobilityState::StateChangeAction Hero::ClimbingMobilityState::update(const float & dt) {
        hero.body.setGravitated(false);
        hero.body.setTreatLadderTopAsGround(false);

        if(hero.actionController) {
            auto const * controller = hero.actionController.get();

            hero.setVelocityY(0);

            if(!hero.isShooting) {
                hero.getAnimatedSprite()->pause();

                float heroFeetY = hero.getPosition().getY();

                // Check if we're overlapping enough to bend over
                if((heroFeetY - climbableRegion.getTop()) <= 8) {
                    hero.changeAnimation("climbing-top");
                } else {
                    hero.changeAnimation("climbing");
                }

                if(controller->shouldMoveUp()) {
                    // Check if moving would eject the hero
                    float projectedY = heroFeetY + (hero.getVelocityY() - hero.climbVelocity.getY());
                    bool willBeEjected = projectedY < climbableRegion.getTop();
                    bool willUnmount = !hero.getBoundingBox().intersects(climbableRegion);

                    if(willBeEjected || willUnmount) {
                        // Special case: climbed over the top, just set to the
                        // top of the ladder
                        hero.setVelocityY(0.0f);
                        hero.body.setBottom(climbableRegion.getTop());
                        hero.body.setOnGround(true);
                        hero.requestMobilityStateChange(std::unique_ptr<MobilityState>(new IdleMobilityState(hero)));
                        return MobilityState::NEXT;
                    } else {
                        hero.setVelocityY(-hero.climbVelocity.getY());
                        hero.getAnimatedSprite()->unpause();
                    }
                } else if(controller->shouldMoveDown()) {
                    hero.setVelocityY(hero.climbVelocity.getY());
                    hero.getAnimatedSprite()->unpause();

                    bool willUnmount = !hero.getBoundingBox().intersects(climbableRegion);

                    if(willUnmount) {
                        hero.isFalling = true;
                        hero.requestMobilityStateChange(std::unique_ptr<MobilityState>(new AirbornMobilityState(hero)));
                        return MobilityState::NEXT;
                    }

                    if(hero.body.isOnGround()) {
                        bool touchingTopOfLadder = heroFeetY <= climbableRegion.getTop() + 1;

                        if(!touchingTopOfLadder) {
                            hero.requestMobilityStateChange(std::unique_ptr<MobilityState>(new IdleMobilityState(hero)));
                            return MobilityState::NEXT;
                        }
                    }
                } else if(controller->shouldJump()) {
                    // If you're holding up or down the jump button is ignored
                    // So that's why it's at the end of this if/else branch
                    hero.isFalling = true;
                    hero.requestMobilityStateChange(std::unique_ptr<MobilityState>(new AirbornMobilityState(hero)));
                    return MobilityState::NEXT;
                }
            } else {
                if(controller->shouldMoveLeft()) {
                    hero.setDirection(Directions::Left);
                } else if(controller->shouldMoveRight()) {
                    hero.setDirection(Directions::Right);
                }
                hero.chooseAnimation();
            }   
        }

        return MobilityState::CONTINUE;
    }

} // hikari