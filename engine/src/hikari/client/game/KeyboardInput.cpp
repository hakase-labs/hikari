#include "hikari/client/game/KeyboardInput.hpp"
#include "hikari/core/util/Log.hpp"
namespace hikari {

    KeyboardInput::KeyboardInput()
        : currentState()
        , previousState()
    {

    }
        
    const bool KeyboardInput::isUp(const Button &button) const {
        return !isDown(button);
    }

    const bool KeyboardInput::isDown(const Button &button) const {
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

    const bool KeyboardInput::isHeld(const Button &button) const {
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

    const bool KeyboardInput::wasPressed(const Button &button) const {
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

    const bool KeyboardInput::wasReleased(const Button &button) const {
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

    void KeyboardInput::processEvent(const sf::Event &keyboardEvent) {
        if(keyboardEvent.type == sf::Event::KeyPressed) {
                    HIKARI_LOG(debug3) << "Pressed a key!";

            switch(keyboardEvent.key.code) {
                case sf::Keyboard::Up:
                    currentState.buttonUp = BUTTON_PUSHED;
                    break;
                case sf::Keyboard::Right:
                    currentState.buttonRight = BUTTON_PUSHED;
                    break;
                case sf::Keyboard::Down:
                    currentState.buttonDown = BUTTON_PUSHED;
                    break;
                case sf::Keyboard::Left:
                    currentState.buttonLeft = BUTTON_PUSHED;
                    break;
                case sf::Keyboard::A:
                    currentState.buttonShoot = BUTTON_PUSHED;
                    currentState.buttonStart = BUTTON_PUSHED;
                    break;
                case sf::Keyboard::S:
                    currentState.buttonJump = BUTTON_PUSHED;
                    break;
                case sf::Keyboard::Return:
                    currentState.buttonStart = BUTTON_PUSHED;
                    break;
                case sf::Keyboard::Escape:
                    currentState.buttonCancel = BUTTON_PUSHED;
                    break;
                default:
                    break;
            }
        } else if(keyboardEvent.type == sf::Event::KeyReleased) {
                    HIKARI_LOG(debug3) << "Released a key!";

            switch(keyboardEvent.key.code) {
                case sf::Keyboard::Up:
                    currentState.buttonUp = !BUTTON_PUSHED;
                    break;
                case sf::Keyboard::Right:
                    currentState.buttonRight = !BUTTON_PUSHED;
                    break;
                case sf::Keyboard::Down:
                    currentState.buttonDown = !BUTTON_PUSHED;
                    break;
                case sf::Keyboard::Left:
                    currentState.buttonLeft = !BUTTON_PUSHED;
                    break;
                case sf::Keyboard::A:
                    currentState.buttonStart = !BUTTON_PUSHED;
                    currentState.buttonShoot = !BUTTON_PUSHED;
                    break;
                case sf::Keyboard::S:
                    currentState.buttonJump = !BUTTON_PUSHED;
                    break;
                case sf::Keyboard::Return:
                    currentState.buttonStart = !BUTTON_PUSHED;
                    break;
                case sf::Keyboard::Escape:
                    currentState.buttonCancel = !BUTTON_PUSHED;
                    break;
                default:
                    break;
            }
        }
    }

    void KeyboardInput::update(float dt) {
        previousState = currentState;
    }
} // hikari