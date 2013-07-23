#ifndef HIKARI_CLIENT_GAME_INPUT
#define HIKARI_CLIENT_GAME_INPUT

#include "hikari/core/Platform.hpp"

namespace hikari {

    /**
     * Input is a class that abstracts away the computer's peripheral input
     * into a virtual "gamepad". The "gamepad" has several "buttons" that can
     * be pushed and released.
     *
     * Different Input implementations can map physical input devices with the
     * virtual "gamepad". This allows for handling keyboard, joystick, and
     * network-driven controllers through a common interface.
     */
    class Input {
    public:
        typedef int Button;

        enum Constants {
            BUTTON_UP = (1 << 0),
            BUTTON_RIGHT = (1 << 1),
            BUTTON_DOWN = (1 << 2),
            BUTTON_LEFT = (1 << 3),
            BUTTON_SHOOT = (1 << 4),
            BUTTON_JUMP = (1 << 5)
        };

        virtual ~Input() { }
        
        /**
         * Indicates if a particular virtual button is not currently being
         * pressed down.
         *
         * @param  button the button to check the state of
         * @return        true if the button is not being pressed down
         * @see Input::isDown
         */
        virtual const bool isUp(const Button &button) const = 0;

        /**
         * Indicates if a particular virtual button is currently being
         * pressed down.
         *
         * @param  button the button to check the state of
         * @return        true if the button is being pressed down
         * @see Input::isUp
         */
        virtual const bool isDown(const Button &button) const = 0;

        /**
         * Indicates if a particular virtual button is currently being
         * held down. A button is held down when it has been pressed down for
         * two or more consecutive frames.
         *
         * @param  button the button to check the state of
         * @return        true if the button is not being held down
         */
        virtual const bool isHeld(const Button &button) const = 0;

        /**
         * Indicates if a particular virtual button is was just pressed down.
         *
         * @param  button the button to check the state of
         * @return        true if the button was just pressed down
         */
        virtual const bool wasPressed(const Button &button) const = 0;

        /**
         * Indicates if a particular virtual button was just released.
         *
         * @param  button the button to check the state of
         * @return        true if the button was just released
         */
        virtual const bool wasReleased(const Button &button) const = 0;
    };
    
} // hikari

#endif // HIKARI_CLIENT_GAME_INPUT