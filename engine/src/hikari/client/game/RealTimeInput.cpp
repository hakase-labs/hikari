#include "hikari/client/game/RealTimeInput.hpp"

namespace hikari {

    RealTimeInput::RealTimeInput()
        : currentState()
        , previousState()
        , keybindings()
    {
        // Set up defaut key bindings
        bindKey(Input::BUTTON_UP,    sf::Keyboard::Up);
        bindKey(Input::BUTTON_RIGHT, sf::Keyboard::Right);
        bindKey(Input::BUTTON_DOWN,  sf::Keyboard::Down);
        bindKey(Input::BUTTON_LEFT,  sf::Keyboard::Left);
        bindKey(Input::BUTTON_SHOOT, sf::Keyboard::A);
        bindKey(Input::BUTTON_JUMP,  sf::Keyboard::S);
        bindKey(Input::BUTTON_START, sf::Keyboard::Return);
        bindKey(Input::BUTTON_SELECT, sf::Keyboard::RShift);
        bindKey(Input::BUTTON_CANCEL, sf::Keyboard::Escape);
    }

    const bool RealTimeInput::isUp(const Button &button) const {
        return !isDown(button);
    }

    const bool RealTimeInput::isDown(const Button &button) const {
        switch(button) {
            case Input::BUTTON_UP:
                return currentState.buttonUp;
                break;
            case Input::BUTTON_RIGHT:
                return currentState.buttonRight;
                break;
            case Input::BUTTON_DOWN:
                return currentState.buttonDown;
                break;
            case Input::BUTTON_LEFT:
                return currentState.buttonLeft;
                break;
            case Input::BUTTON_SHOOT:
                return currentState.buttonShoot;
                break;
            case Input::BUTTON_JUMP:
                return currentState.buttonJump;
                break;
            case Input::BUTTON_START:
                return currentState.buttonStart;
                break;
            case Input::BUTTON_SELECT:
                return currentState.buttonSelect;
                break;
            case Input::BUTTON_CANCEL:
                return currentState.buttonCancel;
                break;
            default:
                return !BUTTON_PUSHED;
                break;
        }
    }

    const bool RealTimeInput::isHeld(const Button &button) const {
        switch(button) {
            case Input::BUTTON_UP:
                return (currentState.buttonUp & previousState.buttonUp);
                break;
            case Input::BUTTON_RIGHT:
                return (currentState.buttonRight & previousState.buttonRight);
                break;
            case Input::BUTTON_DOWN:
                return (currentState.buttonDown & previousState.buttonDown);
                break;
            case Input::BUTTON_LEFT:
                return (currentState.buttonLeft & previousState.buttonLeft);
                break;
            case Input::BUTTON_SHOOT:
                return (currentState.buttonShoot & previousState.buttonShoot);
                break;
            case Input::BUTTON_JUMP:
                return (currentState.buttonJump & previousState.buttonJump);
                break;
            case Input::BUTTON_START:
                return (currentState.buttonStart & previousState.buttonStart);
                break;
            case Input::BUTTON_SELECT:
                return (currentState.buttonSelect & previousState.buttonSelect);
                break;
            case Input::BUTTON_CANCEL:
                return (currentState.buttonCancel & previousState.buttonCancel);
                break;
            default:
                return !BUTTON_PUSHED;
                break;
        }
    }

    const bool RealTimeInput::wasPressed(const Button &button) const {
        switch(button) {
            case Input::BUTTON_UP:
                return (currentState.buttonUp == BUTTON_PUSHED) && (previousState.buttonUp != BUTTON_PUSHED);
                break;
            case Input::BUTTON_RIGHT:
                return (currentState.buttonRight == BUTTON_PUSHED) && (previousState.buttonRight != BUTTON_PUSHED);
                break;
            case Input::BUTTON_DOWN:
                return (currentState.buttonDown == BUTTON_PUSHED) && (previousState.buttonDown != BUTTON_PUSHED);
                break;
            case Input::BUTTON_LEFT:
                return (currentState.buttonLeft == BUTTON_PUSHED) && (previousState.buttonLeft != BUTTON_PUSHED);
                break;
            case Input::BUTTON_SHOOT:
                return (currentState.buttonShoot && !previousState.buttonShoot);
                break;
            case Input::BUTTON_JUMP:
                return (currentState.buttonJump && !previousState.buttonJump);
                break;
            case Input::BUTTON_START:
                return (currentState.buttonStart && !previousState.buttonStart);
                break;
            case Input::BUTTON_SELECT:
                return (currentState.buttonSelect && !previousState.buttonSelect);
                break;
            case Input::BUTTON_CANCEL:
                return (currentState.buttonCancel && !previousState.buttonCancel);
                break;
            default:
                return !BUTTON_PUSHED;
                break;
        }
    }

    const bool RealTimeInput::wasReleased(const Button &button) const {
        switch(button) {
            case Input::BUTTON_UP:
                return (currentState.buttonUp != BUTTON_PUSHED) && (previousState.buttonUp == BUTTON_PUSHED);
                break;
            case Input::BUTTON_RIGHT:
                return (currentState.buttonRight != BUTTON_PUSHED) && (previousState.buttonRight == BUTTON_PUSHED);
                break;
            case Input::BUTTON_DOWN:
                return (currentState.buttonDown != BUTTON_PUSHED) && (previousState.buttonDown == BUTTON_PUSHED);
                break;
            case Input::BUTTON_LEFT:
                return (currentState.buttonLeft != BUTTON_PUSHED) && (previousState.buttonLeft == BUTTON_PUSHED);
                break;
            case Input::BUTTON_SHOOT:
                return (!currentState.buttonShoot && previousState.buttonShoot);
                break;
            case Input::BUTTON_JUMP:
                return (!currentState.buttonJump && previousState.buttonJump);
                break;
            case Input::BUTTON_START:
                return (!currentState.buttonStart && previousState.buttonStart);
                break;
            case Input::BUTTON_SELECT:
                return (!currentState.buttonSelect && previousState.buttonSelect);
                break;
            case Input::BUTTON_CANCEL:
                return (!currentState.buttonCancel && previousState.buttonCancel);
                break;
            default:
                return !BUTTON_PUSHED;
                break;
        }
    }

    void RealTimeInput::update() {
        previousState = currentState;

        currentState.buttonUp     = sf::Keyboard::isKeyPressed(keybindings[Input::BUTTON_UP]);
        currentState.buttonRight  = sf::Keyboard::isKeyPressed(keybindings[Input::BUTTON_RIGHT]);
        currentState.buttonDown   = sf::Keyboard::isKeyPressed(keybindings[Input::BUTTON_DOWN]);
        currentState.buttonLeft   = sf::Keyboard::isKeyPressed(keybindings[Input::BUTTON_LEFT]);
        currentState.buttonShoot  = sf::Keyboard::isKeyPressed(keybindings[Input::BUTTON_SHOOT]);
        currentState.buttonJump   = sf::Keyboard::isKeyPressed(keybindings[Input::BUTTON_JUMP]);
        currentState.buttonStart  = sf::Keyboard::isKeyPressed(keybindings[Input::BUTTON_START]);
        currentState.buttonSelect = sf::Keyboard::isKeyPressed(keybindings[Input::BUTTON_SELECT]);
        currentState.buttonCancel = sf::Keyboard::isKeyPressed(keybindings[Input::BUTTON_CANCEL]);
    }

    void RealTimeInput::bindKey(const Button & button, sf::Keyboard::Key key) {
        keybindings[button] = key;
    }

} // hikari