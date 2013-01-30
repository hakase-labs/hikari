#include "hikari/client/game/RealTimeInput.hpp"

namespace hikari {

    RealTimeInput::RealTimeInput()
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

    const bool RealTimeInput::isUp(const Button &button) const {
        return !isDown(button);
    }

    const bool RealTimeInput::isDown(const Button &button) const {
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

    const bool RealTimeInput::isHeld(const Button &button) const {
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

    const bool RealTimeInput::wasPressed(const Button &button) const {
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

    const bool RealTimeInput::wasReleased(const Button &button) const {
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

    void RealTimeInput::update() {
        previousUp = currentUp;
        previousRight = currentRight;
        previousDown = currentDown;
        previousLeft = currentLeft;
        previousShoot = currentShoot;
        previousJump = currentJump;

        currentUp = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
        currentRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
        currentDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
        currentLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
        currentShoot = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        currentJump = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    }

} // hikari