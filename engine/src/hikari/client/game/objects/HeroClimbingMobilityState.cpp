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
        // hero.setPosition(static_cast<float>(hero.ladderPositionX), hero.getPosition().getY());
        hero.body.setGravitated(false);
        hero.body.setTreatLadderTopAsGround(false);

        hero.body.setPosition(climbableRegion.getLeft() + (climbableRegion.getWidth() / 2), hero.getPosition().getY());

        HIKARI_LOG(debug4) << "Hero::ClimbingMobilityState::enter()";

        // Determine the top-most pixel position of the ladder
    }

    void Hero::ClimbingMobilityState::exit() {
        hero.isClimbing = false;
        hero.isOnLadder = false;
        hero.body.setGravitated(true);
        hero.body.setTreatLadderTopAsGround(true);
        hero.getAnimatedSprite()->unpause();
        HIKARI_LOG(debug4) << "Hero::ClimbingMobilityState::exit()";
    }

    Hero::MobilityState::StateChangeAction Hero::ClimbingMobilityState::update(const float & dt) {
        hero.body.setGravitated(false);
        hero.body.setTreatLadderTopAsGround(false);

        if(hero.actionController) {
            auto const * controller = hero.actionController.get();

            hero.setVelocityY(0);

            if(!hero.isShooting) {
                hero.getAnimatedSprite()->pause();

                if(hero.isTouchingLadderTop) {
                    hero.changeAnimation("climbing-top");
                } else {
                    hero.changeAnimation("climbing");
                }

                if(controller->shouldMoveUp()) {
                    // Check if moving would eject the hero
                    float projectedY = hero.getPosition().getY() + (hero.getVelocityY() - hero.climbVelocity.getY());
                    bool willBeEjected = projectedY < climbableRegion.getTop();
                    bool willUnmount = !hero.getBoundingBox().intersects(climbableRegion);

                    if(willBeEjected || willUnmount) {
                        // Special case: climbed over the top, just set to the
                        // top of the ladder
                        HIKARI_LOG(debug4) << "Climbed up the top! eject";
                        hero.setVelocityY(0.0f);
                        hero.body.setBottom(climbableRegion.getTop());
                        hero.body.setOnGround(true);

                        // if(hero.body.isOnGround()) {
                        hero.requestMobilityStateChange(std::unique_ptr<MobilityState>(new IdleMobilityState(hero)));
                        return MobilityState::NEXT;
                        // }
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
                        float heroFeetY = hero.body.getBoundingBox().getBottom();
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

        // if(hero.actionController) {
        //     auto const * controller = hero.actionController.get();

        //     hero.setVelocityY(0);

        //     if(!hero.isShooting) {
        //         hero.getAnimatedSprite()->pause();
        //     } else {
        //         if(controller->shouldMoveLeft()) {
        //             hero.setDirection(Directions::Left);
        //         } else if(controller->shouldMoveRight()) {
        //             hero.setDirection(Directions::Right);
        //         }
        //         hero.chooseAnimation();
        //     }

        //     if(!hero.isShooting) {
        //         if(controller->shouldMoveUp()) {
        //             if(hero.isTouchingLadderTop) {
        //                 hero.changeAnimation("climbing-top");
        //             } else {
        //                 hero.changeAnimation("climbing");
        //             }
        //             hero.setVelocityY(-hero.climbVelocity.getY());
        //             hero.getAnimatedSprite()->unpause();

        //             // If we climb to the top of the ladder, we need to be put in the ground instead of move up too high.
        //             // TODO: clean this code up... it's messy!
        //             if(!hero.isTouchingLadder) {
        //                 hero.setVelocityY(0.0f);

        //                 auto positionPixels = hero.body.getPosition();
        //                 auto offsetPixels = hero.body.getBoundingBox().getOrigin();
        //                 auto gridSize = hero.getRoom()->getGridSize();
        //                 auto bottom = hero.body.getBoundingBox().getBottom();
        //                 int newBottom = (static_cast<int>(std::floor(bottom) / static_cast<float>(gridSize)) + 1) * gridSize; // Quantize the bottom pixels
        //                 int newY = newBottom;// + offsetPixels.getY();
        //                 //hero.body.setPosition(positionPixels.getX(), static_cast<float>(newY));
        //                 hero.body.setBottom(static_cast<float>(newBottom));
        //                 hero.body.setOnGround(true);

        //                 // HIKARI_LOG(hikari::LogLevel::debug) << "SHOULD BE POSITIONING";

        //                 // HIKARI_LOG(debug4) << "new y = " << newY;
        //             }
        //         } else if(controller->shouldMoveDown()) {
        //             if(hero.isTouchingLadderTop) {
        //                 hero.changeAnimation("climbing-top");
        //             } else {
        //                 hero.changeAnimation("climbing");
        //             }
        //             hero.setVelocityY(hero.climbVelocity.getY());
        //             hero.getAnimatedSprite()->unpause();
        //         } else if(controller->shouldJump()) {
        //             // If you're holding up or down the jump button is ignored
        //             // So that's why it's at the end of this if/else branch
        //             hero.isFalling = true;
        //             hero.requestMobilityStateChange(std::unique_ptr<MobilityState>(new AirbornMobilityState(hero)));
        //             return MobilityState::NEXT;
        //         }
        //     }

        //     // Only change directions if you're shooting
        //     // If you're not shooting then you can't change directions left/right

        //     // Can jump (amd fall down) from ladders


        //     if(hero.body.isOnGround() && !hero.isTouchingLadderWithFeet) {
        //         // HIKARI_LOG(hikari::LogLevel::debug) << "GOING IDLE";

        //         hero.requestMobilityStateChange(std::unique_ptr<MobilityState>(new IdleMobilityState(hero)));
        //         return MobilityState::NEXT;
        //     }

        //     // Can reach the top of a ladder and not be climbing anymore
        //     if(!hero.isTouchingLadder && !hero.isTouchingLadderWithFeet) {
        //         if(controller->shouldMoveDown()) {
        //             hero.isFalling = true;
        //             hero.requestMobilityStateChange(std::unique_ptr<MobilityState>(new AirbornMobilityState(hero)));
        //             return MobilityState::NEXT;
        //         }

        //         // HIKARI_LOG(hikari::LogLevel::debug) << "POSSIBLY FLYING OFF LADDER";

        //         hero.requestMobilityStateChange(std::unique_ptr<MobilityState>(new IdleMobilityState(hero)));
        //         return MobilityState::NEXT;
        //     }
        // }

        return MobilityState::CONTINUE;
    }

} // hikari