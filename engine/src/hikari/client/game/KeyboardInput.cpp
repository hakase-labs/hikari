#include "hikari/client/game/KeyboardInput.hpp"
#include "hikari/core/util/Log.hpp"
namespace hikari {

    KeyboardInput::KeyboardInput()
        : previousUp(false)
        , previousRight(false)
        , previousDown(false)
        , previousLeft(false)
        , previousShoot(false)
        , previousJump(false)
        , currentUp(false)
        , currentRight(false)
        , currentDown(false)
        , currentLeft(false)
        , currentShoot(false)
        , currentJump(false) {

    }
        
    const bool KeyboardInput::isUp(const Button &button) const {
        return !isDown(button);
    }

    const bool KeyboardInput::isDown(const Button &button) const {
        switch(button) {
            case Input::BUTTON_UP:
                return currentUp;
                break;
            case Input::BUTTON_RIGHT:
                return currentRight;
                break;
            case Input::BUTTON_DOWN:
                return currentDown;
                break;
            case Input::BUTTON_LEFT:
                return currentLeft;
                break;
            case Input::BUTTON_SHOOT:
                return currentShoot;
                break;
            case Input::BUTTON_JUMP:
                return currentJump;
                break;
            default:
                return !BUTTON_PUSHED;
                break;
        }
    }

    const bool KeyboardInput::isHeld(const Button &button) const {
        switch(button) {
            case Input::BUTTON_UP:
                return (currentUp & previousUp);
                break;
            case Input::BUTTON_RIGHT:
                return (currentRight & previousRight);
                break;
            case Input::BUTTON_DOWN:
                return (currentDown & previousDown);
                break;
            case Input::BUTTON_LEFT:
                return (currentLeft & previousLeft);
                break;
            case Input::BUTTON_SHOOT:
                return (currentShoot & previousShoot);
                break;
            case Input::BUTTON_JUMP:
                return (currentJump & previousJump);
                break;
            default:
                return !BUTTON_PUSHED;
                break;
        }
    }

    const bool KeyboardInput::wasPressed(const Button &button) const {
        switch(button) {
            case Input::BUTTON_UP:
                return (currentUp == BUTTON_PUSHED) && (previousUp != BUTTON_PUSHED);
                break;
            case Input::BUTTON_RIGHT:
                return (currentRight == BUTTON_PUSHED) && (previousRight != BUTTON_PUSHED);
                break;
            case Input::BUTTON_DOWN:
                return (currentDown == BUTTON_PUSHED) && (previousDown != BUTTON_PUSHED);
                break;
            case Input::BUTTON_LEFT:
                return (currentLeft == BUTTON_PUSHED) && (previousLeft != BUTTON_PUSHED);
                break;
            case Input::BUTTON_SHOOT:
                return (currentShoot && !previousShoot);
                break;
            case Input::BUTTON_JUMP:
                return (currentJump && !previousJump);
                break;
            default:
                return !BUTTON_PUSHED;
                break;
        }
    }

    const bool KeyboardInput::wasReleased(const Button &button) const {
        switch(button) {
            case Input::BUTTON_UP:
                return (currentUp != BUTTON_PUSHED) && (previousUp == BUTTON_PUSHED);
                break;
            case Input::BUTTON_RIGHT:
                return (currentRight != BUTTON_PUSHED) && (previousRight == BUTTON_PUSHED);
                break;
            case Input::BUTTON_DOWN:
                return (currentDown != BUTTON_PUSHED) && (previousDown == BUTTON_PUSHED);
                break;
            case Input::BUTTON_LEFT:
                return (currentLeft != BUTTON_PUSHED) && (previousLeft == BUTTON_PUSHED);
                break;
            case Input::BUTTON_SHOOT:
                return (!currentShoot && previousShoot);
                break;
            case Input::BUTTON_JUMP:
                return (!currentJump && previousJump);
                break;
            default:
                return !BUTTON_PUSHED;
                break;
        }
    }

    void KeyboardInput::update(const sf::Event &keyboardEvent) {
        HIKARI_LOG(debug2) << "Key event being processed!";
        if(keyboardEvent.type == sf::Event::KeyPressed) {
            HIKARI_LOG(debug3) << "Key event KeyPressed";
            switch(keyboardEvent.key.code) {
                case sf::Keyboard::Up:
                    previousUp = currentUp;
                    currentUp = BUTTON_PUSHED;
                    break;
                case sf::Keyboard::Right:
                    previousRight = currentRight;
                    currentRight = BUTTON_PUSHED;
                    break;
                case sf::Keyboard::Down:
                    previousDown = currentDown;
                    currentDown = BUTTON_PUSHED;
                    break;
                case sf::Keyboard::Left:
                    previousLeft = currentLeft;
                    currentLeft = BUTTON_PUSHED;
                    break;
                case sf::Keyboard::A:
                    previousShoot = currentShoot;
                    currentShoot = BUTTON_PUSHED;
                    break;
                case sf::Keyboard::S:
                    previousJump = currentJump;
                    currentJump = BUTTON_PUSHED;
                    break;
                default:
                    break;
            }
        } else if(keyboardEvent.type == sf::Event::KeyReleased) {
            HIKARI_LOG(debug3) << "Key event KeyReleased";
            switch(keyboardEvent.key.code) {
                case sf::Keyboard::Up:
                    previousUp = currentUp;
                    currentUp = !BUTTON_PUSHED;
                    break;
                case sf::Keyboard::Right:
                    previousRight = currentRight;
                    currentRight = !BUTTON_PUSHED;
                    break;
                case sf::Keyboard::Down:
                    previousDown = currentDown;
                    currentDown = !BUTTON_PUSHED;
                    break;
                case sf::Keyboard::Left:
                    previousLeft = currentLeft;
                    currentLeft = !BUTTON_PUSHED;
                    break;
                case sf::Keyboard::A:
                    previousShoot = currentShoot;
                    currentShoot = !BUTTON_PUSHED;
                    break;
                case sf::Keyboard::S:
                    previousJump = currentJump;
                    currentJump = !BUTTON_PUSHED;
                    break;
                default:
                    break;
            }
        }
    }

} // hikari