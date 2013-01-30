#ifndef HIKARI_CLIENT_GAME_OBJECTS_CONTROLLERS_PLAYERINPUTHEROACTIONCONTROLLER
#define HIKARI_CLIENT_GAME_OBJECTS_CONTROLLERS_PLAYERINPUTHEROACTIONCONTROLLER

#include "hikari/client/game/objects/controllers/HeroActionController.hpp"
#include <memory>

namespace hikari {

    class Input;

    class PlayerInputHeroActionController : public HeroActionController {
    private:
        std::weak_ptr<Input> input;

    public:
        PlayerInputHeroActionController(const std::weak_ptr<Input>& input);
        virtual ~PlayerInputHeroActionController();

        const std::weak_ptr<Input>& getInput() const;
        void setInput(const std::weak_ptr<Input> input);

        virtual bool shouldMoveUp() const;
        virtual bool shouldMoveRight() const;
        virtual bool shouldMoveDown() const;
        virtual bool shouldMoveLeft() const;
        virtual bool shouldJump() const;
        virtual bool shouldStopJumping() const;
        virtual bool shouldShootWeapon() const;
        virtual bool shouldChargeWeapon() const;
        virtual bool shouldSlide() const;
        virtual bool shouldStopSliding() const;
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_CONTROLLERS_PLAYERINPUTHEROACTIONCONTROLLER
