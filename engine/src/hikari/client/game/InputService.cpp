#include "hikari/client/game/InputService.hpp"
#include "hikari/client/game/Input.hpp"

namespace hikari {

    InputService::InputService(const std::shared_ptr<Input> & input)
        : Input()
        , Service()
        , input(input)
    {

    }

    InputService::~InputService() {

    }

    const bool InputService::isUp(const Button &button) const {
        if(input) {
            return input->isUp(button);
        }

        return false;
    }

    const bool InputService::isDown(const Button &button) const {
        if(input) {
            return input->isDown(button);
        }

        return false;
    }

    const bool InputService::isHeld(const Button &button) const {
        if(input) {
            return input->isHeld(button);
        }

        return false;
    }

    const bool InputService::wasPressed(const Button &button) const {
        if(input) {
            return input->wasPressed(button);
        }

        return false;
    }

    const bool InputService::wasReleased(const Button &button) const {
        if(input) {
            return input->wasReleased(button);
        }

        return false;
    }

    void InputService::update(float dt) {
        if(input) {
            input->update(dt);
        }
    }

} // hikari
