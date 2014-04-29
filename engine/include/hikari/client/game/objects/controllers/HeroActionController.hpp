#ifndef HIKARI_CLIENT_GAME_OBJECTS_CONTROLLERS_HEROACTIONCONTROLLER
#define HIKARI_CLIENT_GAME_OBJECTS_CONTROLLERS_HEROACTIONCONTROLLER

namespace hikari {

    class HeroActionController {
    public:
        virtual ~HeroActionController() { }

        virtual bool shouldMoveUp() const = 0;
        virtual bool shouldMoveRight() const = 0;
        virtual bool shouldMoveDown() const = 0;
        virtual bool shouldMoveLeft() const = 0;
        virtual bool shouldJump() const = 0;
        virtual bool shouldSuperJump() const = 0;
        virtual bool shouldStopJumping() const = 0;
        virtual bool shouldShootWeapon() const = 0;
        virtual bool shouldChargeWeapon() const = 0;
        virtual bool shouldSlide() const = 0;
        virtual bool shouldStopSliding() const = 0;
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_CONTROLLERS_HEROACTIONCONTROLLER
