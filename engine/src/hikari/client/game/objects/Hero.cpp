#include "hikari/client/game/objects/Hero.hpp"
#include "hikari/client/game/objects/HeroClimbingMobilityState.hpp"
#include "hikari/client/game/objects/HeroIdleMobilityState.hpp"
#include "hikari/client/game/objects/HeroTeleportingMobilityState.hpp"
#include "hikari/client/game/objects/HeroDamagedMobilityState.hpp"
#include "hikari/client/game/objects/PalettedAnimatedSprite.hpp"
#include "hikari/client/game/events/EventBus.hpp"
#include "hikari/client/game/events/EventData.hpp"
#include "hikari/client/game/events/EntityDeathEventData.hpp"
#include "hikari/client/game/events/EntityStateChangeEventData.hpp"
#include "hikari/client/game/events/WeaponFireEventData.hpp"
#include "hikari/core/game/Animation.hpp"
#include "hikari/core/game/map/Room.hpp"
#include "hikari/core/math/NESNumber.hpp"
#include "hikari/core/game/SpriteAnimator.hpp"
#include "hikari/core/game/map/Tileset.hpp"
#include "hikari/core/util/Log.hpp"
#include "hikari/core/geom/GeometryUtils.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

namespace hikari {

    Hero::Hero(int id, std::shared_ptr<Room> room)
        : Entity(id, room)
        , isDecelerating(false)
        , isStanding(false)
        , isWalking(false)
        , isSliding(false)
        , isInTunnel(false)
        , isJumping(false)
        , isFalling(false)
        , isAirborn(false)
        , isClimbing(false)
        , isTouchingLadderTop(false)
        , isFullyAccelerated(false)
        , isShooting(false)
        , isTeleporting(false)
        , isMorphing(false)
        , isStunned(false)
        , isInvincible(false)
        , isUnderWater(false)
        , wasUnderWaterLastFrame(false)
        , climbableRegion(0, 0, 0, 0)
        , actionController(nullptr)
        , mobilityState(nullptr)
        , nextMobilityState(nullptr)
        , temporaryMobilityState(nullptr)
        , shootingState(nullptr)
        , nextShootingState(nullptr) 
    {
        setDeathType(EntityDeathType::Hero);

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

            if(auto events = this->getEventBus().lock()) {
                events->triggerEvent(EventDataPtr(new EntityStateChangeEventData(getId(), "landed")));
            }

#ifdef HIKARI_DEBUG_HERO_PHYSICS
            this->countAscendingFrames = 0;
            this->countDecendingFrames = 0;
#endif // HIKARI_DEBUG_HERO_PHYSICS
        });

        invincibilityTimer = 0;
        blinkTimer = 0;
        isBlinking = false;
        isVisible = true;

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

        setFaction(Faction::Hero);

        changeMobilityState(std::unique_ptr<MobilityState>(new IdleMobilityState(*this)));
        changeShootingState(std::unique_ptr<ShootingState>(new NotShootingState(*this)));

        getAnimatedSprite()->setUsePalette(true);
        getAnimatedSprite()->setUseSharedPalette(true);
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
        if(wasUnderWaterLastFrame != isUnderWater) {
            if(wasUnderWaterLastFrame) {
                HIKARI_LOG(debug4) << "I'm not in water anymore!";
                body.setGravityApplicationThreshold(1);
            } else {
                HIKARI_LOG(debug4) << "I'm jumping in the water NOW!";
                body.setGravityApplicationThreshold(3);

                // Only emit event when plunging in to a body of water.
                if(auto events = this->getEventBus().lock()) {
                    events->triggerEvent(EventDataPtr(new EntityStateChangeEventData(getId(), "water")));
                }
            }
        }

        wasUnderWaterLastFrame = isUnderWater;

        if(const auto & room = getRoom()) {
            const int gridSize = room->getGridSize();

            if(invincibilityTimer > 0.0f) {
                invincibilityTimer -= dt;
                isInvincible = true;
            } else {
                isInvincible = false;
                isBlinking = false;
                isVisible = true;
            }

            if(isBlinking) {
                blinkTimer -= dt;

                if(blinkTimer <= 0.0f) {
                    blinkTimer = 0.0667f;
                    isVisible = !isVisible;
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

            // Check if we're under water or starting to enter water
            {
                int bodyPositionTile = room->getAttributeAt(
                    static_cast<int>(getPosition().getX()) / gridSize,
                    static_cast<int>(getPosition().getY()) / gridSize);

                if((bodyPositionTile != Room::NO_TILE) && TileAttribute::hasAttribute(bodyPositionTile, TileAttribute::WATER)) {
                    isUnderWater = true;
                } else {
                    isUnderWater = false;
                }
            }

            //
            // State machine updates
            //

            if(temporaryMobilityState) {
                MobilityState::StateChangeAction action = temporaryMobilityState->update(dt);

                if(MobilityState::NEXT == action) {
                    popTemporaryMobilityState();
                }
            } else {
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
        }

        Entity::update(dt);
    }

    bool Hero::canJump() {
        return !isAirborn || isClimbing;
    }

    bool Hero::canSlide() {
        return !isAirborn && !isSliding && !isStunned; /* TODO: Check if currently sliding too */
    }

    void Hero::playAnimation(float dt) {
        const auto & animSprite = getAnimatedSprite();

        if(animSprite) {
            animSprite->update(dt);
        }
    }

    void Hero::performJump() {
        if(isClimbing) {
            isClimbing = false;
            setVelocityY(0);
            HIKARI_LOG(debug4) << "Started fall from ladder at " << getPosition().getY();
        } else {
            setVelocityY(jumpVelocity.getY());
            HIKARI_LOG(debug4) << "Started jump at " << getPosition().getY();
        }
    }

    void Hero::performTeleport() {
        changeMobilityState(std::unique_ptr<MobilityState>(new TeleportingMobilityState(*this)));
        popTemporaryMobilityState();
        isBlinking = false;
        blinkTimer = 0.0f;
        invincibilityTimer = 0.0f;
        isVisible = true;
    }

    void Hero::performMorph() {
        isMorphing = true;
    }

    void Hero::performSlide() {
        HIKARI_LOG(debug4) << "Started sliding!";
    }

    void Hero::performStun() {
        if(!isStunned) {
            pushTemporaryMobilityState(std::unique_ptr<MobilityState>(new DamagedMobilityState(*this)));
        }

        isStunned = true;
    }

    void Hero::stopShooting() {
        changeShootingState(std::unique_ptr<ShootingState>(new NotShootingState(*this)));
    }

    void Hero::requestClimbingAttachment(const BoundingBox<float> & climbableRegion) {
        if(!isClimbing) {
            if(actionController) {
                if(actionController->shouldMoveUp()) {
                    // Check to see if we're still able to climb up
                    float heroFeetY = body.getBoundingBox().getBottom();
                    float distanceFromPlatform = heroFeetY - climbableRegion.getTop();
                    bool touchingTopOfLadder = distanceFromPlatform >= 1;

                    if(touchingTopOfLadder) {
                        changeMobilityState(std::unique_ptr<MobilityState>(new ClimbingMobilityState(*this, climbableRegion)));
                    }
                } else if(actionController->shouldMoveDown()) {
                    // Check if standing on top of a ladder
                    float heroFeetY = body.getBoundingBox().getBottom();
                    bool touchingTopOfLadder = heroFeetY <= climbableRegion.getTop() + 1;

                    if(body.isOnGround()) {
                        if(!isClimbing && touchingTopOfLadder) {
                            changeMobilityState(std::unique_ptr<MobilityState>(new ClimbingMobilityState(*this, climbableRegion)));
                        }
                    }
                }
            }
        } else {
            // It's possible that we lose the ladder after a transition
            // so this is accounted for here.
            if(climbableRegion != this->climbableRegion) {
                changeMobilityState(std::unique_ptr<MobilityState>(new ClimbingMobilityState(*this, climbableRegion)));
            }
        }
    }

    void Hero::kill() {
        if(auto events = getEventBus().lock()) {
            EventDataPtr imDeadNow(new EntityDeathEventData(getId(), EntityDeathEventData::Hero));
            events->queueEvent(imDeadNow);
        } else {
            HIKARI_LOG(debug4) << "No event manager.";
        }
    }

    bool Hero::isVulnerable() {
        return !isInvincible;
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

    void Hero::pushTemporaryMobilityState(std::unique_ptr<MobilityState> && temporaryState) {
        temporaryMobilityState = std::move(temporaryState);

        if(temporaryMobilityState) {
            temporaryMobilityState->enter();
        }
    }

    void Hero::popTemporaryMobilityState() {
        if(temporaryMobilityState) {
            temporaryMobilityState->exit();
            chooseAnimation();
        }

        pushTemporaryMobilityState(std::unique_ptr<MobilityState>(nullptr));
    }

    void Hero::requestShootingStateChange(std::unique_ptr<ShootingState> && newState) {
        if(newState) {
            nextShootingState = std::move(newState);
        }
    }

    void Hero::chooseAnimation() {
        if(isStunned) {
            changeAnimation("damaged-standing");
        }
        else if(isTeleporting) {
            if(isMorphing) {
                changeAnimation("morphing");
            } else {
                changeAnimation("teleporting");
            }
        } else {
            if(isClimbing) {
                if(isShooting) {
                    changeAnimation("climbing-shooting");
                } else {
                    if(isTouchingLadderTop) {
                        changeAnimation("climbing-top");
                    } else {
                        changeAnimation("climbing");
                    }
                }
            }
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
        if(isVisible) {
            Entity::render(target);
        }
    }

    void Hero::handleCollision(Movable& body, CollisionInfo& info) {
        if(info.isCollisionY && info.directionY == Directions::Down) {
            isAirborn = false;
        }

        //
        // Check if we hit spikes; if we did then we're dead!
        //
        if(isVulnerable()) {
            if(TileAttribute::hasAttribute(info.tileType, TileAttribute::SPIKE)
                || TileAttribute::hasAttribute(info.tileType, TileAttribute::ABYSS)) {
                kill();
            }
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

        if(hero.actionController) {
            auto const * controller = hero.actionController.get();
            if(controller->shouldMoveLeft()) {
                hero.setDirection(Directions::Left);
            } else if(controller->shouldMoveRight()) {
                hero.setDirection(Directions::Right);
            }
        }
        hero.chooseAnimation();
        hero.fireWeapon();
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
                hero.fireWeapon();
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

            if(controller->shouldShootWeapon() && hero.canFireWeapon() && !hero.isSliding) {
                hero.requestShootingStateChange(std::unique_ptr<ShootingState>(new IsShootingState(hero)));
                return ShootingState::NEXT;
            }
        }

        return ShootingState::CONTINUE;
    }

} // hikari
