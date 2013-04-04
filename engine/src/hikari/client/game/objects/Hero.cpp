#include "hikari/client/game/objects/Hero.hpp"
#include "hikari/core/game/Animation.hpp"
#include "hikari/core/game/map/Room.hpp"
#include "hikari/core/math/RetroVector.hpp"
#include "hikari/core/game/SpriteAnimator.hpp"
#include "hikari/core/game/map/Tileset.hpp"
#include "hikari/core/util/Log.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

namespace hikari {

    Hero::Hero(const int &id, std::shared_ptr<Room> room)
        : Entity(id, room)
        , isDecelerating(false)
        , isStanding(false)
        , isWalking(false)
        , isSliding(false)
        , isInTunnel(false)
        , isJumping(false)
        , isFalling(false)
        , isAirborn(false)
        , isOnLadder(false)
        , isTouchingLadder(false)
        , isTouchingLadderWithFeet(false)
        , isFullyAccelerated(false)
        , isShooting(false)
        , isTeleporting(false)
        , ladderPositionX(0)
        , mobilityState(nullptr)
        , shootingState(nullptr)
        , actionController(nullptr)
    {
        body.setGravitated(true);
        body.setHasWorldCollision(true);

        body.setCollisionCallback(std::bind(&Entity::handleCollision, this, std::placeholders::_1, std::placeholders::_2));
        body.setLandingCallback([this](Movable& movable, CollisionInfo& collisionInfo) {
            if(actionController->shouldMoveRight() || actionController->shouldMoveLeft()) {
                this->isFullyAccelerated = true;
            }

            this->isJumping = false;
            this->isFalling = false;
            this->isAirborn = false;

            this->countAscendingFrames = 0;
            this->countDecendingFrames = 0;
        });

        walkVelocity = Vector2<float>(RetroVector(0x01, 0x4C).toFloat(), 0.0f);
        climbVelocity = Vector2<float>(walkVelocity.getY(), walkVelocity.getX()); // Climbs at same speed as he walks
        jumpVelocity = Vector2<float>(0.0f, -(RetroVector(0x04, 0xA5) + RetroVector(0, 0x40) + RetroVector(0, 0x40)).toFloat());
        suddenFallVelocity = Vector2<float>(0.0f, RetroVector(0, 0x80).toFloat());
        slideVelocity = Vector2<float>(RetroVector(0x02, 0x80).toFloat(), 0.0f);
        hurtVelocity = Vector2<float>();

        accelerationDelay = 0;
        accelerationDelayThreshold = 6;

        this->countAscendingFrames = 0;
        this->countDecendingFrames = 0;

        isFullyAccelerated = false;

        changeMobilityState(std::unique_ptr<MobilityState>(new IdleMobilityState(this)));
        changeShootingState(std::unique_ptr<ShootingState>(new NotShootingState(*this)));
    }

    Hero::~Hero() {

    }

    const std::shared_ptr<HeroActionController>& Hero::getActionController() const {
        return actionController;
    }

    void Hero::setActionController(const std::shared_ptr<HeroActionController>& actionController) {
        this->actionController = actionController;
    }

    void Hero::update(const float &dt) {
        if(room) {
            const int gridSize = room->getGridSize();

            // Do global updating...
            ladderPositionX = 0;
            isTouchingLadder = false;
            isTouchingLadderTop = false;
            isTouchingLadderWithFeet = false;

            // Check to see if we're touching a ladder
            {
                auto bbox = this->getBoundingBox();
                auto topRight = bbox.getTopRight();
                auto topLeft = bbox.getTopLeft();
                auto bottomLeft = bbox.getBottomLeft();
                auto bottomRight = bbox.getBottomRight();

                int trAttr        = room->getAttributeAt(static_cast<int>(topRight.getX()    - 1) / gridSize, static_cast<int>(topRight.getY()       ) / gridSize);
                int tlAttr        = room->getAttributeAt(static_cast<int>(topLeft.getX()        ) / gridSize, static_cast<int>(topLeft.getY()        ) / gridSize);
                int blAttr        = room->getAttributeAt(static_cast<int>(bottomLeft.getX()     ) / gridSize, static_cast<int>(bottomLeft.getY()  - 1) / gridSize);
                int brAttr        = room->getAttributeAt(static_cast<int>(bottomRight.getX() - 1) / gridSize, static_cast<int>(bottomRight.getY() - 1) / gridSize);
                int feetLeftAttr  = room->getAttributeAt(static_cast<int>(bottomLeft.getX()     ) / gridSize, static_cast<int>(bottomLeft.getY()     ) / gridSize);
                int feetRightAttr = room->getAttributeAt(static_cast<int>(bottomRight.getX() - 1) / gridSize, static_cast<int>(bottomRight.getY()    ) / gridSize);
                int posAttr       = room->getAttributeAt(static_cast<int>(getPosition().getX()  ) / gridSize, static_cast<int>(getPosition().getY()  ) / gridSize);

                bool touchingTopRight =     ((trAttr != Room::NO_TILE)         && TileAttribute::hasAttribute(trAttr,          TileAttribute::LADDER));
                bool touchingTopLeft =      ((tlAttr != Room::NO_TILE)         && TileAttribute::hasAttribute(tlAttr,          TileAttribute::LADDER));
                bool touchingBottomLeft =   ((blAttr != Room::NO_TILE)         && TileAttribute::hasAttribute(blAttr,          TileAttribute::LADDER));
                bool touchingBottomRight =  ((brAttr != Room::NO_TILE)         && TileAttribute::hasAttribute(brAttr,          TileAttribute::LADDER));
                bool touchingFeetLeft =     ((feetLeftAttr != Room::NO_TILE)   && TileAttribute::hasAttribute(feetLeftAttr,    TileAttribute::LADDER_TOP));
                bool touchingFeetRight =    ((feetRightAttr != Room::NO_TILE)  && TileAttribute::hasAttribute(feetRightAttr,   TileAttribute::LADDER_TOP));
                bool touchingBodyOnLadder = ((posAttr != Room::NO_TILE)        && TileAttribute::hasAttribute(posAttr,         TileAttribute::LADDER));
                bool touchingLadderTop =    ((posAttr != Room::NO_TILE)        && TileAttribute::hasAttribute(posAttr,         TileAttribute::LADDER_TOP));

                isTouchingLadderWithFeet = touchingFeetLeft || touchingFeetRight;

                if(touchingBodyOnLadder|| touchingTopRight || touchingBottomRight || touchingTopLeft || touchingBottomLeft || isTouchingLadderWithFeet) {
                    if(touchingBodyOnLadder) {
                        isTouchingLadder = true;
                    }

                    if(touchingTopRight || touchingBottomRight || touchingFeetRight) {
                        // Determine the overlap amount rightward
                        int realX = static_cast<int>(topRight.getX());
                        int tileX = (static_cast<int>(topRight.getX()) / gridSize) * gridSize;
                        int overlap = std::abs(realX - tileX);

                        if(overlap >= 4) {
                            ladderPositionX = tileX + static_cast<int>(bbox.getWidth() / 2);
                            isTouchingLadder = true;

                            // std::cout << "Overlap Right: " << overlap << std::endl;
                        }
                    }

                    if(touchingTopLeft || touchingBottomLeft || touchingFeetLeft) {
                        // Determine the overlap leftward
                        int realX = static_cast<int>(topLeft.getX());
                        int tileX = (static_cast<int>(topLeft.getX()) / gridSize) * gridSize;
                        int overlap = std::abs(realX - tileX - gridSize);

                        //// ladderPositionX = (((static_cast<int>(topRight.getX()) / gridSize) - 1) * gridSize ) + (bbox.getWidth() / 2);
                        if(overlap >= 4) {
                            ladderPositionX = tileX + static_cast<int>(bbox.getWidth() / 2);
                            isTouchingLadder = true;

                            // std::cout << "Overlap Left: " << overlap << std::endl;
                        }
                    }

                    if(touchingLadderTop) {
                        // Find overlap
                        unsigned char ladderTopOverlap = (static_cast<int>(getPosition().getY()) % gridSize);

                        if(ladderTopOverlap <= 8) {
                            isTouchingLadderTop = true;
                        }

                        // std::cout << "Ladder top overlap: " << static_cast<int>(ladderTopOverlap) << std::endl;
                    }
                } else {
                    isTouchingLadder = false;
                    isOnLadder = false;
                }
            }

            //
            // Check if we're in a tunnel
            //
            if(isSliding) {
                const auto & bbox   = body.getBoundingBox();
                const int startingX = static_cast<int>(bbox.getLeft());
                const int endingX   = static_cast<int>(bbox.getRight());
                const int y         = static_cast<int>(bbox.getTop()) - 1; // Subtract 1 to test the tile directly *above* Rockman

                isInTunnel = false;

                int topLeftTile = room->getAttributeAt(startingX / gridSize, y / gridSize);
                int topRightTile = room->getAttributeAt((endingX - 1) / gridSize, y / gridSize);

                // Only check the top left and top right points
                if(((topLeftTile != Room::NO_TILE) && TileAttribute::hasAttribute(topLeftTile, TileAttribute::SOLID))
                    || ((topRightTile != Room::NO_TILE) && TileAttribute::hasAttribute(topRightTile, TileAttribute::SOLID))) {
                    isInTunnel = true;
                }
            }

            if(actionController) {
                if(actionController->shouldMoveUp()) {
                    if(isTouchingLadder && !isOnLadder) {
                        if(!isTouchingLadderWithFeet) {
                            changeMobilityState(std::unique_ptr<MobilityState>(new ClimbingMobilityState(this)));
                        }
                    }
                } else if(actionController->shouldMoveDown()) {
                    if(isTouchingLadder && !isOnLadder && (!body.isOnGround() || isTouchingLadderWithFeet)) {
                        changeMobilityState(std::unique_ptr<MobilityState>(new ClimbingMobilityState(this)));
                    }
                }
            }

            if(shootingState) {
                shootingState->update(dt);
            }

            // Do mobility updating...
            if(mobilityState) {
                mobilityState->update(dt);
            }
        }

        Entity::update(dt);
    }

    bool Hero::canJump() {
        return !isAirborn || isOnLadder;
    }

    bool Hero::canSlide() {
        return !isAirborn; /* TODO: Check if currently sliding too */
    }

    void Hero::performJump() {
        if(isOnLadder) {
            isOnLadder = false;
            setVelocityY(0);
            std::cout << "Started fall form ladder at " << getPosition().getY() << std::endl;
        } else {
            setVelocityY(jumpVelocity.getY());
            std::cout << "Started jump at " << getPosition().getY() << std::endl;
        }
    }

    void Hero::performTeleport() {
        changeMobilityState(std::unique_ptr<MobilityState>(new TeleportingMobilityState(this)));
    }

    void Hero::performMorph() {
        isMorphing = true;
    }

    void Hero::performSlide() {
        std::cout << "Started sliding!" << std::endl;
    }

    void Hero::changeMobilityState(std::unique_ptr<MobilityState> && newState) {
        if(newState) {
            if(mobilityState) {
                mobilityState->exit();
            }
            mobilityState = std::move(newState);
            mobilityState->enter();
        }
    }

    void Hero::changeShootingState(std::unique_ptr<ShootingState> && newState) {
        if(newState) {
            if(shootingState) {
                shootingState->exit();
            }
            shootingState = std::move(newState);
            shootingState->enter();
        }
    }

    void Hero::chooseAnimation() {
        if(isTeleporting) {
            if(isMorphing) {
                changeAnimation("morphing");
            } else {
                changeAnimation("teleporting");
            }
        } else {
            // Idle animations
            if(isStanding) {
                if(isDecelerating) {
                    changeAnimation("running-stopping");
                } else {
                    if(isShooting) {
                        changeAnimation("standing-shooting");
                    } else {
                        changeAnimation("standing");
                    }
                }
            }
            // Walking animations
            else if(isWalking) {
                if(!isFullyAccelerated) {
                    changeAnimation("running-accelerating");
                } else {
                    if(isShooting) {
                        changeAnimation("running-shooting");
                    } else {
                        changeAnimation("running");
                    }
                }
            }
            // Sliding
            else if(isSliding) {
                changeAnimation("sliding");
            }
            // Falling or Jumping
            else if(isAirborn) {
                if(isShooting) {
                    changeAnimation("jumping-shooting");
                } else {
                    changeAnimation("jumping");
                }
            }
        }
    }

    void Hero::render(sf::RenderTarget &target) {
        Entity::render(target);
    }

    void Hero::handleCollision(Movable& body, CollisionInfo& info) {
        if(info.isCollisionY && info.directionY == Directions::Down) {
            isAirborn = false;
        }

        //
        // Check if we hit spikes; if we did then we're dead!
        //
        if(info.isCollisionY && TileAttribute::hasAttribute(info.tileType, TileAttribute::SPIKE)) {
            HIKARI_LOG(debug4) << "Hit spikes; send kill message!";
        }
    }

    //
    // MobilityState
    //
    Hero::MobilityState::MobilityState(Hero * hero)
        : hero(hero)
    {

    }

    Hero::MobilityState::~MobilityState() {

    }

    //
    // TeleportingMobilityState
    //
    Hero::TeleportingMobilityState::TeleportingMobilityState(Hero * hero)
        : MobilityState(hero)
        , morphingLimit(0.2167f) // ~13 frames
        , morphingCounter(0.0f)
    {

    }

    Hero::TeleportingMobilityState::~TeleportingMobilityState() {

    }

    void Hero::TeleportingMobilityState::enter() {
        hero->isTeleporting = true;
        hero->isMorphing = false;
        hero->chooseAnimation();
        hero->setVelocityX(0.0f);
        hero->setVelocityY(0.0f);
    }

    void Hero::TeleportingMobilityState::exit() {
        hero->isTeleporting = false;
        hero->isMorphing = false;
    }

    void Hero::TeleportingMobilityState::update(const float & dt) {
        if(hero->isMorphing) {
            if(morphingCounter == 0.0f) {
                hero->chooseAnimation();
            }

            morphingCounter += dt;

            if(morphingCounter >= morphingLimit) {
                // Time to change!
                hero->changeMobilityState(std::unique_ptr<MobilityState>(new IdleMobilityState(hero)));
                // Emit event or play a sound
            }
        }
    }

    //
    // IdleMobilityState
    //
    Hero::IdleMobilityState::IdleMobilityState(Hero * hero)
        : MobilityState(hero)
    {

    }

    Hero::IdleMobilityState::~IdleMobilityState() {

    }

    void Hero::IdleMobilityState::enter() {
        hero->isStanding = true;
        hero->isFullyAccelerated = false;
        hero->chooseAnimation();
        hero->setVelocityX(0.0f);
    }

    void Hero::IdleMobilityState::exit() {
        hero->isStanding = false;
    }

    void Hero::IdleMobilityState::update(const float & dt) {
        if(hero->actionController) {
            auto const * controller = hero->actionController.get();

            if(hero->isDecelerating) {
                hero->isDecelerating = false;
                hero->chooseAnimation();
            }

            // Disappearing blocks, moving platform, who knows...
            if(!hero->body.isOnGround()) {
                hero->changeMobilityState(std::unique_ptr<MobilityState>(new AirbornMobilityState(hero)));
                return;
            } else {
                if((controller->shouldMoveLeft() && !controller->shouldMoveRight())
                    || (controller->shouldMoveRight() && !controller->shouldMoveLeft())) {
                    hero->changeMobilityState(std::unique_ptr<MobilityState>(new WalkingMobilityState(hero)));
                    return;
                }

                if(controller->shouldSlide() && hero->canSlide()) {
                    hero->changeMobilityState(std::unique_ptr<MobilityState>(new SlidingMobilityState(hero)));
                    return;
                }

                if(hero->canJump() && controller->shouldJump()) {
                    hero->changeMobilityState(std::unique_ptr<MobilityState>(new AirbornMobilityState(hero)));
                    return;
                }
            }
        }
    }

    //
    // WalkingMobilityState
    //
    Hero::WalkingMobilityState::WalkingMobilityState(Hero * hero)
        : MobilityState(hero)
        , accelerationDelay(0)
        , accelerationDelayThreshold(6) // stall for 6 frames
        , isDecelerating(false)
        , lastDirection(Directions::None)
    {
        // std::cout << "WalkingMobilityState()" << std::endl;
    }

    Hero::WalkingMobilityState::~WalkingMobilityState() {
        // std::cout << "~WalkingMobilityState()" << std::endl;
    }

    void Hero::WalkingMobilityState::enter() {
        hero->isWalking = true;
        hero->isAirborn = false;
        hero->isStanding = false;
        hero->isDecelerating = false;
        isDecelerating = false;
    }

    void Hero::WalkingMobilityState::exit() {
        hero->isWalking = false;
    }

    void Hero::WalkingMobilityState::update(const float & dt) {
        if(hero->actionController) {
            auto const * controller = hero->actionController.get();

            lastDirection = hero->getDirection();

            if(!isDecelerating) {
                if(controller->shouldMoveLeft() && !controller->shouldMoveRight()) {
                    hero->setDirection(Directions::Left);
                    hero->setVelocityX(-(hero->walkVelocity.getX()));
                }

                if(controller->shouldMoveRight() && !controller->shouldMoveLeft()) {
                    hero->setDirection(Directions::Right);
                    hero->setVelocityX(hero->walkVelocity.getX());
                }

                // Handle direction switching (reset acceleration)
                if(hero->getDirection() != lastDirection) {
                    hero->isFullyAccelerated = false;
                    hero->getAnimationPlayer()->rewind();
                    accelerationDelay = 0;
                }

                // Handle acceleration
                if(!hero->isFullyAccelerated) {
                    hero->body.setApplyHorizontalVelocity(accelerationDelay == 0);

                    hero->isFullyAccelerated = !(accelerationDelay < accelerationDelayThreshold);
                    accelerationDelay += 1; // dt;

                    std::cout << "Accellerating..." << std::endl;

                    hero->chooseAnimation();
                } else {
                    hero->body.setApplyHorizontalVelocity(true);
                    hero->chooseAnimation();
                }
            }

            //
            // Other state conditions
            //

            // Moving in both directions at the same time causes Rock to idle.
            if(controller->shouldMoveLeft() && controller->shouldMoveRight()) {
                if(!isDecelerating) {
                    isDecelerating = true;
                } else {
                    hero->isDecelerating = true;
                    hero->changeMobilityState(std::unique_ptr<MobilityState>(new IdleMobilityState(hero)));
                    return;
                }
            }

            // Not moving in either direction, so go back to standing.
            if(!controller->shouldMoveLeft() && !controller->shouldMoveRight()){
                if(!isDecelerating) {
                    isDecelerating = true;
                } else {
                    hero->isDecelerating = true;
                    hero->changeMobilityState(std::unique_ptr<MobilityState>(new IdleMobilityState(hero)));
                    return;
                }
            }

            if(controller->shouldSlide() && hero->canSlide()) {
                hero->changeMobilityState(std::unique_ptr<MobilityState>(new SlidingMobilityState(hero)));
                return;
            }

            // Trying to jump so go ahead and jump.
            if(hero->canJump() && controller->shouldJump()) {
                hero->changeMobilityState(std::unique_ptr<MobilityState>(new AirbornMobilityState(hero)));
                return;
            }

            if(!hero->body.isOnGround()) {
                hero->changeMobilityState(std::unique_ptr<MobilityState>(new AirbornMobilityState(hero)));
                return;
            }
        }
    }

    Hero::SlidingMobilityState::SlidingMobilityState(Hero * hero)
        : MobilityState(hero)
        , oldBoundingBox(hero->getBoundingBox())
        , slideDuration(0.0f)
        , slideDurationThreshold(1.0f / 60.0f * 19.0f) // 19 frames, 0.3166 seconds
    {

    }

    Hero::SlidingMobilityState::~SlidingMobilityState() {

    }

    void Hero::SlidingMobilityState::enter() {
        slideDuration = 0.0f;

        hero->isWalking = false;
        hero->isStanding = false;
        hero->isAirborn = false;
        hero->isFullyAccelerated = true;
        hero->isSliding = true;
        hero->body.setApplyHorizontalVelocity(true);
        hero->body.setApplyVerticalVelocity(true);

        oldBoundingBox = hero->getBoundingBox();

        BoundingBoxF newBoundingBox(oldBoundingBox);
        newBoundingBox.setHeight(16.0f);
        newBoundingBox.setWidth(16.0f);
        newBoundingBox.setOrigin(8.0f, 14.0f);
        newBoundingBox.setBottom(oldBoundingBox.getBottom());

        hero->setBoundingBox(newBoundingBox);

        hero->chooseAnimation();
    }

    void Hero::SlidingMobilityState::exit() {
        const auto currentBoundingBox = hero->getBoundingBox();

        BoundingBoxF restoredBoundingBox(oldBoundingBox);
        restoredBoundingBox.setBottom(currentBoundingBox.getBottom());
        restoredBoundingBox.setPosition(currentBoundingBox.getPosition());

        hero->setBoundingBox(restoredBoundingBox);

        hero->isSliding = false;
        hero->isInTunnel = false;
    }

    void Hero::SlidingMobilityState::update(const float & dt) {
        slideDuration += dt;

        if(auto const * controller = hero->actionController.get()) {
            if(hero->isInTunnel || (slideDuration < slideDurationThreshold && (controller->shouldSlide() || !controller->shouldStopSliding()))) {
                if(controller->shouldMoveLeft()) {
                    hero->setDirection(Directions::Left);
                }

                if(controller->shouldMoveRight()) {
                    hero->setDirection(Directions::Right);
                }

                auto const direction = hero->getDirection();

                if(direction == Directions::Left) {
                    hero->setVelocityX(-(hero->slideVelocity.getX()));
                } else {
                    hero->setVelocityX(hero->slideVelocity.getX());
                }
            } else {
                if(!controller->shouldMoveLeft() && !controller->shouldMoveRight()){
                    hero->isDecelerating = true;
                    hero->changeMobilityState(std::unique_ptr<MobilityState>(new IdleMobilityState(hero)));
                    return;
                }

                if((controller->shouldMoveLeft() && !controller->shouldMoveRight())
                    || (controller->shouldMoveRight() && !controller->shouldMoveLeft())) {
                        hero->isFullyAccelerated = true;
                        hero->changeMobilityState(std::unique_ptr<MobilityState>(new WalkingMobilityState(hero)));
                        return;
                }

                if(hero->canJump() && controller->shouldJump()) {
                    hero->changeMobilityState(std::unique_ptr<MobilityState>(new AirbornMobilityState(hero)));
                    return;
                }

                if(!hero->body.isOnGround()) {
                    hero->changeMobilityState(std::unique_ptr<MobilityState>(new AirbornMobilityState(hero)));
                    return;
                }

                hero->isDecelerating = true;
                hero->changeMobilityState(std::unique_ptr<MobilityState>(new IdleMobilityState(hero)));
                return;
            }
        }
    }

    //
    // JumpingMobilityState
    //
    Hero::AirbornMobilityState::AirbornMobilityState(Hero * hero)
        : MobilityState(hero)
    {

    }

    Hero::AirbornMobilityState::~AirbornMobilityState() {

    }

    void Hero::AirbornMobilityState::enter() {
        hero->isAirborn = true;
        hero->body.setApplyHorizontalVelocity(true);

        if(hero->actionController) {
            auto const * controller = hero->actionController.get();
            if(controller->shouldJump() && !hero->isFalling) {
                hero->setVelocityY(hero->jumpVelocity.getY());
                hero->isJumping = true;
            } else if(!hero->body.isOnGround()) {
                hero->isFalling = true;
            }
        }

        hero->chooseAnimation();
    }

    void Hero::AirbornMobilityState::exit() {
        hero->isJumping = false;
    }

    void Hero::AirbornMobilityState::update(const float & dt) {
        if(hero->actionController) {
            auto const * controller = hero->actionController.get();

            // Handle movement in the air
            if(controller->shouldMoveLeft()) {
                hero->setDirection(Directions::Left);
                hero->setVelocityX(-(hero->walkVelocity.getX()));
            } else if(controller->shouldMoveRight()) {
                hero->setDirection(Directions::Right);
                hero->setVelocityX(hero->walkVelocity.getX());
            } else {
                hero->setVelocityX(0.0f);
            }

            if(hero->getVelocityY() > 0) {
                hero->countDecendingFrames++;
            } else if(hero->getVelocityY() < 0) {
                hero->countAscendingFrames++;
            }

            //
            // Other state conditions
            //
            if(hero->isJumping && controller->shouldStopJumping() && hero->getVelocityY() < 0.0f) {
                hero->isJumping = false;
                hero->isFalling = true;
                hero->setVelocityY(hero->suddenFallVelocity.getY());
            }

            if(hero->body.isOnGround()) {
                if(controller->shouldMoveLeft() || controller->shouldMoveRight()) {
                    hero->isFullyAccelerated = true;
                    hero->changeMobilityState(std::unique_ptr<MobilityState>(new WalkingMobilityState(hero)));
                    return;
                } else {
                    hero->changeMobilityState(std::unique_ptr<MobilityState>(new IdleMobilityState(hero)));
                    return;
                }
            }
        }
    }

    Hero::ClimbingMobilityState::ClimbingMobilityState(Hero * hero)
        : MobilityState(hero)
    {

    }

    Hero::ClimbingMobilityState::~ClimbingMobilityState() {

    }

    void Hero::ClimbingMobilityState::enter() {
        hero->isOnLadder = true;
        hero->isFalling = false;
        hero->isAirborn = false;
        hero->isFullyAccelerated = false;
        hero->isJumping = false;
        hero->isStanding = false;
        hero->isDecelerating = false;
        hero->isWalking = false;

        hero->setVelocityX(0.0f);
        hero->setPosition(static_cast<float>(hero->ladderPositionX), hero->getPosition().getY());
        hero->body.setGravitated(false);
        hero->body.setTreatLadderTopAsGround(false);
    }

    void Hero::ClimbingMobilityState::exit() {
        hero->isOnLadder = false;
        hero->body.setGravitated(true);
        hero->body.setTreatLadderTopAsGround(true);
        hero->getAnimationPlayer()->unpause();
    }

    void Hero::ClimbingMobilityState::update(const float & dt) {
        hero->body.setTreatLadderTopAsGround(false);

        if(hero->actionController) {
            auto const * controller = hero->actionController.get();

            hero->setVelocityY(0);
            hero->getAnimationPlayer()->pause();

            if(controller->shouldMoveUp()) {
                if(hero->isTouchingLadderTop) {
                    hero->changeAnimation("climbing-top");
                } else {
                    hero->changeAnimation("climbing");
                }
                hero->setVelocityY(-hero->climbVelocity.getY());
                hero->getAnimationPlayer()->unpause();

                // If we climb to the top of the ladder, we need to be put in the ground instead of move up too high.
                // TODO: clean this code up... it's messy!
                if(!hero->isTouchingLadder) {
                    hero->setVelocityY(0.0f);

                    auto positionPixels = hero->body.getPosition();
                    auto offsetPixels = hero->body.getBoundingBox().getOrigin();
                    auto gridSize = hero->getRoom()->getGridSize();
                    auto bottom = hero->body.getBoundingBox().getBottom();
                    int newBottom = static_cast<int>(std::ceil(bottom) / static_cast<float>(gridSize)) * gridSize; // Quantize the bottom pixels
                    int newY = newBottom;// + offsetPixels.getY();
                    hero->body.setPosition(positionPixels.getX(), static_cast<float>(newY));
                    hero->body.setOnGround(true);
                    //std::cout << "new y = " << newY << std::endl;
                }
            } else if(controller->shouldMoveDown()) {
                if(hero->isTouchingLadderTop) {
                    hero->changeAnimation("climbing-top");
                } else {
                    hero->changeAnimation("climbing");
                }
                hero->setVelocityY(hero->climbVelocity.getY());
                hero->getAnimationPlayer()->unpause();
            } else if(controller->shouldJump()) {
                // If you're holding up or down the jump button is ignored
                // So that's why it's at the end of this if/else branch
                hero->isFalling = true;
                hero->changeMobilityState(std::unique_ptr<MobilityState>(new AirbornMobilityState(hero)));
                return;
            }

            // Only change directions if you're shooting
            // If you're not shooting then you can't change directions left/right

            // Can jump (amd fall down) from ladders


            if(hero->body.isOnGround() && !hero->isTouchingLadderWithFeet) {
                hero->changeMobilityState(std::unique_ptr<MobilityState>(new IdleMobilityState(hero)));
                return;
            }

            // Can reach the top of a ladder and not be climbing anymore
            if(!hero->isTouchingLadder && !hero->isTouchingLadderWithFeet) {
                if(controller->shouldMoveDown()) {
                    hero->isFalling = true;
                    hero->changeMobilityState(std::unique_ptr<MobilityState>(new AirbornMobilityState(hero)));
                    return;
                }

                hero->changeMobilityState(std::unique_ptr<MobilityState>(new IdleMobilityState(hero)));
                return;
            }
        }
    }


    Hero::ShootingState::ShootingState(Hero & hero)
        : hero(hero)
    {
    }

    Hero::ShootingState::~ShootingState() {

    }

    Hero::IsShootingState::IsShootingState(Hero & hero)
        : ShootingState(hero)
        , cooldown(0.0f)
        , cooldownTimer(0.0f)
    {

    }

    Hero::IsShootingState::~IsShootingState() {

    }

    void Hero::IsShootingState::enter() {
        hero.isShooting = true;
        cooldown = (15.0f * (1.0f / 60.0f)); // this should actually be the weapon's cooldown...
        cooldownTimer = cooldown;
        std::cout << "Shooting now!" << std::endl;
        hero.chooseAnimation();
    }

    void Hero::IsShootingState::exit() {

    }

    void Hero::IsShootingState::update(const float & dt) {
        cooldownTimer -= dt;

        if(hero.actionController) {
            auto const * controller = hero.actionController.get();

            // TODO: Actually fix this to use a real cooldown
            if(controller->shouldShootWeapon()) {
                cooldownTimer = cooldown;
            }
        }

        if(cooldownTimer <= 0.0f) {
            hero.changeShootingState(std::unique_ptr<ShootingState>(new NotShootingState(hero)));
            return;
        }
    }

    Hero::NotShootingState::NotShootingState(Hero & hero)
        : ShootingState(hero)
    {

    }

    Hero::NotShootingState::~NotShootingState() {

    }

    void Hero::NotShootingState::enter() {
        hero.isShooting = false;
        std::cout << "Not shooting!" << std::endl;
        hero.chooseAnimation();
    }

    void Hero::NotShootingState::exit() {

    }

    void Hero::NotShootingState::update(const float & dt) {
        if(hero.actionController) {
            auto const * controller = hero.actionController.get();

            if(controller->shouldShootWeapon()) {
                hero.changeShootingState(std::unique_ptr<ShootingState>(new IsShootingState(hero)));
            }
        }
    }

} // hikari
