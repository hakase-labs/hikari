#include "hikari/client/game/objects/Hero.hpp"
#include "hikari/client/game/objects/HeroClimbingMobilityState.hpp"
#include "hikari/client/game/objects/HeroIdleMobilityState.hpp"
#include "hikari/client/game/objects/HeroTeleportingMobilityState.hpp"
#include "hikari/client/game/events/EventManager.hpp"
#include "hikari/client/game/events/EventData.hpp"
#include "hikari/client/game/events/WeaponFireEventData.hpp"
#include "hikari/core/game/Animation.hpp"
#include "hikari/core/game/map/Room.hpp"
#include "hikari/core/math/NESNumber.hpp"
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
        , actionController(nullptr)
        , mobilityState(nullptr)
        , nextMobilityState(nullptr)
        , shootingState(nullptr)
        , nextShootingState(nullptr) 
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

#ifdef HIKARI_DEBUG_HERO_PHYSICS
            this->countAscendingFrames = 0;
            this->countDecendingFrames = 0;
#endif // HIKARI_DEBUG_HERO_PHYSICS
        });

        walkVelocity = Vector2<float>(NESNumber(0x01, 0x4C).toFloat(), 0.0f);
        climbVelocity = Vector2<float>(walkVelocity.getY(), walkVelocity.getX()); // Climbs at same speed as he walks
        jumpVelocity = Vector2<float>(0.0f, -(NESNumber(0x04, 0xA5) + NESNumber(0, 0x40) + NESNumber(0, 0x40)).toFloat());
        suddenFallVelocity = Vector2<float>(0.0f, NESNumber(0, 0x80).toFloat());
        slideVelocity = Vector2<float>(NESNumber(0x02, 0x80).toFloat(), 0.0f);
        hurtVelocity = Vector2<float>();

        accelerationDelay = 0;
        accelerationDelayThreshold = 6;
#ifdef HIKARI_DEBUG_HERO_PHYSICS
        this->countAscendingFrames = 0;
        this->countDecendingFrames = 0;
#endif // HIKARI_DEBUG_HERO_PHYSICS

        isFullyAccelerated = false;

        changeMobilityState(std::unique_ptr<MobilityState>(new IdleMobilityState(*this)));
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

    void Hero::update(float dt) {
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

                            // HIKARI_LOG(debug4) << "Overlap Right: " << overlap;
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

                            // HIKARI_LOG(debug4) << "Overlap Left: " << overlap;
                        }
                    }

                    if(touchingLadderTop) {
                        // Find overlap
                        unsigned char ladderTopOverlap = (static_cast<int>(getPosition().getY()) % gridSize);

                        if(ladderTopOverlap <= 8) {
                            isTouchingLadderTop = true;
                        }

                        // HIKARI_LOG(debug4) << "Ladder top overlap: " << static_cast<int>(ladderTopOverlap);
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
                            changeMobilityState(std::unique_ptr<MobilityState>(new ClimbingMobilityState(*this)));
                        }
                    }
                } else if(actionController->shouldMoveDown()) {
                    if(isTouchingLadder && !isOnLadder && (!body.isOnGround() || isTouchingLadderWithFeet)) {
                        changeMobilityState(std::unique_ptr<MobilityState>(new ClimbingMobilityState(*this)));
                    }
                }
            }

            //
            // State machine updates
            //

            if(shootingState) {
                // Handle state change request actions...
                ShootingState::StateChangeAction action = shootingState->update(dt);

                if(ShootingState::NEXT == action) {
                    if(nextShootingState) {
                        changeShootingState(std::move(nextShootingState));
                    }
                }
            }

            if(mobilityState) {
                // Handle state change request actions...
                MobilityState::StateChangeAction action = mobilityState->update(dt);

                if(MobilityState::NEXT == action) {
                    if(nextMobilityState) {
                        changeMobilityState(std::move(nextMobilityState));
                    }
                }
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
            HIKARI_LOG(debug4) << "Started fall form ladder at " << getPosition().getY();
        } else {
            setVelocityY(jumpVelocity.getY());
            HIKARI_LOG(debug4) << "Started jump at " << getPosition().getY();
        }
    }

    void Hero::performTeleport() {
        changeMobilityState(std::unique_ptr<MobilityState>(new TeleportingMobilityState(*this)));
    }

    void Hero::performMorph() {
        isMorphing = true;
    }

    void Hero::performSlide() {
        HIKARI_LOG(debug4) << "Started sliding!";
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

    void Hero::requestMobilityStateChange(std::unique_ptr<MobilityState> && newState) {
        if(newState) {
            nextMobilityState = std::move(newState);
        }
    }

    void Hero::requestShootingStateChange(std::unique_ptr<ShootingState> && newState) {
        if(newState) {
            nextShootingState = std::move(newState);
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

    Hero::MobilityState::MobilityState(Hero & hero)
        : hero(hero)
    {

    }

    Hero::MobilityState::~MobilityState() {

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
        HIKARI_LOG(debug4) << "Shooting now!";
        hero.chooseAnimation();

        if(auto events = hero.getEventManager().lock()) {
            EventDataPtr weaponFire(new WeaponFireEventData(hero.getWeaponId(), hero.getId()));
            events->triggerEvent(weaponFire);
        } else {
            HIKARI_LOG(debug4) << "No event manager.";
        }
    }

    void Hero::IsShootingState::exit() {

    }

    Hero::ShootingState::StateChangeAction Hero::IsShootingState::update(const float & dt) {
        cooldownTimer -= dt;

        if(hero.actionController) {
            auto const * controller = hero.actionController.get();

            // TODO: Actually fix this to use a real cooldown
            if(controller->shouldShootWeapon()) {
                cooldownTimer = cooldown;
            }
        }

        if(cooldownTimer <= 0.0f) {
            hero.requestShootingStateChange(std::unique_ptr<ShootingState>(new NotShootingState(hero)));
            return ShootingState::NEXT;
        }

        return ShootingState::CONTINUE;
    }

    Hero::NotShootingState::NotShootingState(Hero & hero)
        : ShootingState(hero)
    {

    }

    Hero::NotShootingState::~NotShootingState() {

    }

    void Hero::NotShootingState::enter() {
        hero.isShooting = false;
        HIKARI_LOG(debug4) << "Not shooting!";
        hero.chooseAnimation();
    }

    void Hero::NotShootingState::exit() {

    }

    Hero::ShootingState::StateChangeAction Hero::NotShootingState::update(const float & dt) {
        if(hero.actionController) {
            auto const * controller = hero.actionController.get();

            if(controller->shouldShootWeapon()) {
                hero.requestShootingStateChange(std::unique_ptr<ShootingState>(new IsShootingState(hero)));
                return ShootingState::NEXT;
            }
        }

        return ShootingState::CONTINUE;
    }

} // hikari
