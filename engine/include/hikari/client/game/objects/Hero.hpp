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
        class ShootingState;

        bool isDecelerating;            // Currently running but need to stop
        bool isStanding;                // Standing idle
        bool isWalking;                 // Moving left or right on purpose
        bool isSliding;                 // Currently engaged in a slide
        bool isInTunnel;                // Surrounded by solid surface (sliding in a narrow space)
        bool isJumping;                 // Moving upward with no solid ground underneath
        bool isFalling;                 // Falling with no solid ground underneath
        bool isAirborn;                 // In air; falling or jumping
        bool isOnLadder;                // Mounted on a ladder (climbing)
        bool isTouchingLadder;          // Touching a ladder with any part of the body
        bool isTouchingLadderTop;       // Colliding with a "ladder top"
        bool isTouchingLadderWithFeet;  // Feet are standing on top of a ladder
        bool isFullyAccelerated;        // Moving at full speed horizontally
        bool isShooting;                // Shooting a weapon
        bool isTeleporting;             // Teleporting from sky
        bool isMorphing;                // Morphing from teleport to other state
        int countAscendingFrames;
        int countDecendingFrames;
        int accelerationDelay;
        int accelerationDelayThreshold;
        int ladderPositionX;
        Vector2<float> walkVelocity;
        Vector2<float> jumpVelocity;
        Vector2<float> climbVelocity;
        Vector2<float> suddenFallVelocity;
        Vector2<float> slideVelocity;
        Vector2<float> hurtVelocity;
        std::shared_ptr<HeroActionController> actionController;

        std::unique_ptr<MobilityState> mobilityState;
        std::unique_ptr<MobilityState> nextMobilityState;
        std::unique_ptr<ShootingState> shootingState;
        std::unique_ptr<ShootingState> nextShootingState;

        /**
            Determines whether the Hero can jump right now or not.
        */
        bool canJump();

        /**
            Determines whther the Hero can slide right now or not.
        */
        bool canSlide();

        void performJump();
        void performSlide();

        void changeMobilityState(std::unique_ptr<MobilityState> && newState);
        void requestMobilityStateChange(std::unique_ptr<MobilityState> && newState);
        void changeShootingState(std::unique_ptr<ShootingState> && newState);
        void requestShootingStateChange(std::unique_ptr<ShootingState> && newState);

        void chooseAnimation();

        class MobilityState {
        protected:
            MobilityState(Hero * hero);
            Hero * hero;
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

        class TeleportingMobilityState : public MobilityState {
        private:
            float morphingLimit;
            float morphingCounter;
        public:
            TeleportingMobilityState(Hero * hero);
            virtual ~TeleportingMobilityState();
            virtual void enter();
            virtual void exit();
            virtual StateChangeAction update(const float & dt);
        };

        class IdleMobilityState : public MobilityState {
        public:
            IdleMobilityState(Hero * hero);
            virtual ~IdleMobilityState();
            virtual void enter();
            virtual void exit();
            virtual StateChangeAction update(const float & dt);
        };

        class WalkingMobilityState : public MobilityState {
        private:
            int accelerationDelay;
            int accelerationDelayThreshold;
            bool isDecelerating;
            Direction lastDirection;
        public:
            WalkingMobilityState(Hero * hero);
            virtual ~WalkingMobilityState();
            virtual void enter();
            virtual void exit();
            virtual StateChangeAction update(const float & dt);
        };

        class SlidingMobilityState : public MobilityState {
        private:
            float slideDuration;
            float slideDurationThreshold;
            BoundingBoxF oldBoundingBox;
        public:
            SlidingMobilityState(Hero * hero);
            virtual ~SlidingMobilityState();
            virtual void enter();
            virtual void exit();
            virtual StateChangeAction update(const float & dt);
        };

        class AirbornMobilityState : public MobilityState {
        public:
            AirbornMobilityState(Hero * hero);
            virtual ~AirbornMobilityState();
            virtual void enter();
            virtual void exit();
            virtual StateChangeAction update(const float & dt);
        };

        class ClimbingMobilityState : public MobilityState {
        public:
            ClimbingMobilityState(Hero * hero);
            virtual ~ClimbingMobilityState();
            virtual void enter();
            virtual void exit();
            virtual StateChangeAction update(const float & dt);
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
        Hero(const int& id, std::shared_ptr<Room> room);
        virtual ~Hero();

        const std::shared_ptr<HeroActionController>& getActionController() const;
        void setActionController(const std::shared_ptr<HeroActionController>& actionController);

        void performTeleport();
        void performMorph();

        virtual void update(const float& dt);
        virtual void render(sf::RenderTarget &target);
        virtual void handleCollision(Movable& body, CollisionInfo& info);
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_HERO
