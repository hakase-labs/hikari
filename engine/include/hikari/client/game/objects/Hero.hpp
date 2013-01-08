#ifndef HIKARI_CLIENT_GAME_OBJECTS_HERO
#define HIKARI_CLIENT_GAME_OBJECTS_HERO

#include <hikari/client/game/objects/Entity.hpp>
#include <hikari/client/game/objects/controllers/HeroActionController.hpp>
#include <hikari/core/math/Vector2.hpp>
#include <memory>

namespace hikari {

    class Hero : public Entity {
    private:
        class MobilityState;
        class ShootingState;

        bool isStopping;
        bool isStanding;
        bool isWalking;
        bool isJumping;
        bool isFalling;
        bool isAirborn;
        bool isOnLadder;
        bool isTouchingLadder;
        bool isTouchingLadderTop;
        bool isTouchingLadderWithFeet;
        bool isFullyAccelerated;
        bool isShooting;
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
        std::unique_ptr<ShootingState> shootingState;

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

        void changeMobilityState(std::unique_ptr<MobilityState> & newState);
        void changeShootingState(std::unique_ptr<ShootingState> & newState);

        void chooseAnimation();

        class MobilityState {
        protected:
            MobilityState(Hero * hero);
            Hero * hero;
        public:
            virtual ~MobilityState();
            virtual void enter() = 0;
            virtual void exit() = 0;
            virtual void update(const float & dt) = 0;
        };

        class IdleMobilityState : public MobilityState {
        public:
            IdleMobilityState(Hero * hero);
            virtual ~IdleMobilityState();
            virtual void enter();
            virtual void exit();
            virtual void update(const float & dt);
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
            virtual void update(const float & dt);
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
            virtual void update(const float & dt);
        };

        class AirbornMobilityState : public MobilityState {
        public:
            AirbornMobilityState(Hero * hero);
            virtual ~AirbornMobilityState();
            virtual void enter();
            virtual void exit();
            virtual void update(const float & dt);
        };

        class ClimbingMobilityState : public MobilityState {
        public:
            ClimbingMobilityState(Hero * hero);
            virtual ~ClimbingMobilityState();
            virtual void enter();
            virtual void exit();
            virtual void update(const float & dt);
        };

        class ShootingState {
        protected:
            Hero & hero;
            ShootingState(Hero & hero);

        public:
            virtual ~ShootingState();
            virtual void enter() = 0;
            virtual void exit() = 0;
            virtual void update(const float & dt) = 0;
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
            virtual void update(const float & dt);
        };

        class NotShootingState : public ShootingState {
        public:
            NotShootingState(Hero & hero);
            virtual ~NotShootingState();
            virtual void enter();
            virtual void exit();
            virtual void update(const float & dt);
        };

    public:
        Hero(const int& id, std::shared_ptr<Room> room);
        virtual ~Hero();

        const std::shared_ptr<HeroActionController>& getActionController() const;
        void setActionController(const std::shared_ptr<HeroActionController>& actionController);

        virtual void update(const float& dt);
        virtual void render(sf::RenderTarget &target);
        virtual void handleCollision(Movable& body, CollisionInfo& info);
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_HERO
