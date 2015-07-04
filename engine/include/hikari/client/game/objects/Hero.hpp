#ifndef HIKARI_CLIENT_GAME_OBJECTS_HERO
#define HIKARI_CLIENT_GAME_OBJECTS_HERO

#include "hikari/client/game/objects/Entity.hpp"
#include "hikari/client/game/objects/controllers/HeroActionController.hpp"
#include "hikari/core/math/Vector2.hpp"
#include <memory>

namespace hikari {

    class Hero : public Entity {
    private:
        class MobilityState;
        class TeleportingMobilityState; // Subclass of MobilityState
        class IdleMobilityState;        // Subclass of MobilityState
        class WalkingMobilityState;     // Subclass of MobilityState
        class SlidingMobilityState;     // Subclass of MobilityState
        class AirbornMobilityState;     // Subclass of MobilityState
        class ClimbingMobilityState;    // Subclass of MobilityState
        class DamagedMobilityState;     // Subclass of MobilityState
        class ShootingState;
        class IsShootingState;          // Sublcass of ShootingState
        class NotShootingState;         // Sublcass of ShootingState

        bool isDecelerating;            // Currently running but need to stop
        bool isStanding;                // Standing idle
        bool isWalking;                 // Moving left or right on purpose
        bool isSliding;                 // Currently engaged in a slide
        bool isInTunnel;                // Surrounded by solid surface (sliding in a narrow space)
        bool isOnEdge;                  // On the edge of a ground plane
        bool isBlockedByWall;           // There is a wall in front of hero in whatever his direction is
        bool isJumping;                 // Moving upward with no solid ground underneath
        bool isFalling;                 // Falling with no solid ground underneath
        bool isAirborn;                 // In air; falling or jumping
        bool isClimbing;                // Mounted on a ladder (climbing)
        bool isTouchingLadder;          // Touching a ladder with any part of the body
        bool isTouchingLadderTop;       // Colliding with a "ladder top"
        bool isFullyAccelerated;        // Moving at full speed horizontally
        bool isShooting;                // Shooting a weapon
        bool isTeleporting;             // Teleporting from sky
        bool isMorphing;                // Morphing from teleport to other state
        bool isStunned;                 // Unable to be controlled; temporarily out of commission
        bool isInvincible;              // Impervious to spikes and other kinds of damage

        bool isBlinking;
        bool isVisible;

        bool isUnderWaterFlag;
        bool wasUnderWaterLastFrame;
        bool hasAvailableWeaponEnergy;

#ifdef HIKARI_DEBUG_HERO_PHYSICS
        int countAscendingFrames;
        int countDecendingFrames;
#endif // HIKARI_DEBUG_HERO_PHYSICS
        int accelerationDelay;
        int accelerationDelayThreshold;
        int ladderPositionX;

        float invincibilityTimer;
        float blinkTimer;

        Vector2<float> walkVelocity;
        Vector2<float> jumpVelocity;
        Vector2<float> climbVelocity;
        Vector2<float> suddenFallVelocity;
        Vector2<float> slideVelocity;
        Vector2<float> hurtVelocity;
        BoundingBox<float> climbableRegion;

        std::shared_ptr<HeroActionController> actionController;

        std::unique_ptr<MobilityState> mobilityState;
        std::unique_ptr<MobilityState> nextMobilityState;
        std::unique_ptr<MobilityState> temporaryMobilityState;
        std::unique_ptr<ShootingState> shootingState;
        std::unique_ptr<ShootingState> nextShootingState;

        /**
         * Determines whether the Hero can jump right now or not.
         */
        bool canJump();

        /**
         * Determines whther the Hero can slide right now or not.
         */
        bool canSlide();

        void performJump(bool isSuperJump = false);
        void performSlide();
        void stopShooting();

        /**
         * Changes from the current mobility state to another. Before changing,
         * the current state's exit() method will be called. After changing, the
         * new state's enter() method will be called.
         *
         * The change happens immediately.
         *
         * @param newState a pointer to a new MobilityState
         */
        void changeMobilityState(std::unique_ptr<MobilityState> && newState);

        /**
         * Enqueues a new MobilityState to change to. When the state machine gets
         * the signal to change states then the most recently-enqueued state will
         * be changed to.
         *
         * To change states the current MobilityState::update must return a
         * MobilityState::NEXT value.
         *
         * @param newState a pointer to a new MobilityState to enqueue
         */
        void requestMobilityStateChange(std::unique_ptr<MobilityState> && newState);

        void pushTemporaryMobilityState(std::unique_ptr<MobilityState> && temporaryState);
        void popTemporaryMobilityState();

        /**
         * Changes from the current shooting state to another. Before changing,
         * the current state's exit() method will be called. After changing, the
         * new state's enter() method will be called.
         *
         * The change happens immediately.
         *
         * @param newState a pointer to a new ShootingState
         */
        void changeShootingState(std::unique_ptr<ShootingState> && newState);

        /**
         * Enqueues a new ShootingState to change to. When the state machine gets
         * the signal to change states then the most recently-enqueued state will
         * be changed to.
         *
         * To change states the current ShootingState::update must return a
         * ShootingState::NEXT value.
         *
         * @param newState a pointer to a new ShootingState to enqueue
         */
        void requestShootingStateChange(std::unique_ptr<ShootingState> && newState);

        /**
         * Changes the hero's animation based on internal state variables. Rather
         * than explicitly changing animations from each state, just make a call
         * to this method and the correct animation will be determined and used.
         */
        void chooseAnimation();

        class MobilityState {
        protected:
            MobilityState(Hero & hero);
            Hero & hero;
        public:
            enum StateChangeAction {
                CONTINUE,
                NEXT
            };
            virtual ~MobilityState();
            virtual void enter() = 0;
            virtual void exit() = 0;
            virtual StateChangeAction update(const float & dt) = 0;
        };

        class ShootingState {
        protected:
            Hero & hero;
            ShootingState(Hero & hero);

        public:
            enum StateChangeAction {
                CONTINUE,
                NEXT
            };
            virtual ~ShootingState();
            virtual void enter() = 0;
            virtual void exit() = 0;
            virtual StateChangeAction update(const float & dt) = 0;
        };

        class IsShootingState : public ShootingState {
        private:
            float cooldown;
            float cooldownTimer;

        public:
            IsShootingState(Hero & hero);
            virtual ~IsShootingState();
            virtual void enter();
            virtual void exit();
            virtual StateChangeAction update(const float & dt);
        };

        class NotShootingState : public ShootingState {
        public:
            NotShootingState(Hero & hero);
            virtual ~NotShootingState();
            virtual void enter();
            virtual void exit();
            virtual StateChangeAction update(const float & dt);
        };

    public:
        Hero(int id, std::shared_ptr<Room> room);
        virtual ~Hero();

        const std::shared_ptr<HeroActionController>& getActionController() const;
        void setActionController(const std::shared_ptr<HeroActionController>& actionController);

        /**
         * Allows playing of the animation even if the Hero isn't being updated.
         * This is useful for transition sequences.
         */
        void playAnimation(float dt);

        void performTeleport();
        void performMorph();
        void performHurt();
        void performStun();
        void requestClimbingAttachment(const BoundingBox<float> & climbableRegion);
        void kill();

        bool isMountedOnLadder() const;
        bool isVulnerable() const;
        bool isNowShooting() const;
        bool isOnGround() const;
        bool isUnderWater() const;
        void setInvincibility(bool invincible);
        void setHasAvailableWeaponEnergy(bool hasEnergy);

        const Vector2<float>& getAmbientVelocity() const;
        void setAmbientVelocity(const Vector2<float> & velocity);

        virtual void update(float dt);
        virtual void render(sf::RenderTarget &target);
        virtual void handleCollision(Movable& body, CollisionInfo& info);
        virtual void fireWeapon();
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_HERO
