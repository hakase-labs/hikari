#include <hikari/client/game/objects/controllers/PlayerInputHeroActionController.hpp>
#include <hikari/client/game/Input.hpp>

namespace hikari {

    PlayerInputHeroActionController::PlayerInputHeroActionController(const std::weak_ptr<Input>& input)
        : input(input) 
    {

    }

    PlayerInputHeroActionController::~PlayerInputHeroActionController() {

    }

    const std::weak_ptr<Input>& PlayerInputHeroActionController::getInput() const {
        return input;
    }

    void PlayerInputHeroActionController::setInput(const std::weak_ptr<Input> input) {
        this->input = input;
    }

    bool PlayerInputHeroActionController::shouldMoveUp() const {
        if(!input.expired()) {
            auto in = input.lock();

            if(in) {
                return in->isDown(Input::BUTTON_UP);
            }
        }
        return false;
    }

    bool PlayerInputHeroActionController::shouldMoveRight() const {
        if(!input.expired()) {
            auto in = input.lock();

            if(in) {
                return in->isDown(Input::BUTTON_RIGHT);
            }
        }
        return false;
    }

    bool PlayerInputHeroActionController::shouldMoveDown() const {
        if(!input.expired()) {
            auto in = input.lock();

            if(in) {
                return in->isDown(Input::BUTTON_DOWN);
            }
        }
        return false;
    }

    bool PlayerInputHeroActionController::shouldMoveLeft() const {
        if(!input.expired()) {
            auto in = input.lock();

            if(in) {
                return in->isDown(Input::BUTTON_LEFT);
            }
        }
        return false;
    }

    bool PlayerInputHeroActionController::shouldJump() const {
        if(auto in = input.lock()) {
            return in->wasPressed(Input::BUTTON_JUMP);
        }
        return false;
    }

    bool PlayerInputHeroActionController::shouldStopJumping() const {
        if(auto in = input.lock()) {
            return in->wasReleased(Input::BUTTON_JUMP);
        }
        return false;
    }

    bool PlayerInputHeroActionController::shouldShootWeapon() const {
        if(!input.expired()) {
            auto in = input.lock();

            if(in) {
                return in->wasPressed(Input::BUTTON_SHOOT);
            }
        }
        return false;
    }

    bool PlayerInputHeroActionController::shouldChargeWeapon() const {
        if(!input.expired()) {
            auto in = input.lock();

            if(in) {
                return in->isHeld(Input::BUTTON_SHOOT);
            }
        }
        return false;
    }

    bool PlayerInputHeroActionController::shouldSlide() const {
        if(!input.expired()) {
            auto in = input.lock();

            if(in) {
                return in->isDown(Input::BUTTON_DOWN) && in->wasPressed(Input::BUTTON_JUMP);
            }
        }
        return false;
    }

    bool PlayerInputHeroActionController::shouldStopSliding() const {
        if(auto in = input.lock()) {
            return !(in->isHeld(Input::BUTTON_DOWN) && in->isHeld(Input::BUTTON_JUMP));
        }
        return false;
    }

} // hikari