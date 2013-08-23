#ifndef HIKARI_CLIENT_GAME_REALTIMEINPUT
#define HIKARI_CLIENT_GAME_REALTIMEINPUT

#include "hikari/core/Platform.hpp"
#include "hikari/client/game/Input.hpp"

#include <SFML/Window/Keyboard.hpp>

#include <unordered_map>

namespace hikari {

    /**
     * RealTimeInput implements the Input iterface in was way that utilizes the
     * state of the computer's keyboard. It is not driven by events but relies
     * on querying the state of the keyboard every frame.
     *
     * When using this class it is important to call the RealTimeInput::update()
     * method every frame to ensure that they state of the keyboard is 
     * accurately reflected.
     *
     * Additionally this class allows they keyboard-to-virtual gamepad button
     * bindings to be set from outside the class.
     */
    class RealTimeInput : public Input {
    private:
        static const bool BUTTON_PUSHED = true;

        struct ButtonState {
            bool buttonUp;
            bool buttonRight;
            bool buttonDown;
            bool buttonLeft;
            bool buttonShoot;
            bool buttonJump;
            bool buttonStart;
            bool buttonSelect;
            bool buttonCancel;

            ButtonState() {
                reset();
            }

            /**
             * Resets the state of all buttons to false.
             */
            void reset() {
                buttonUp = false;
                buttonRight = false;
                buttonDown = false;
                buttonLeft = false;
                buttonShoot = false;
                buttonJump = false;
                buttonStart = false;
                buttonSelect = false;
                buttonCancel = false;
            }
        };

        ButtonState currentState;
        ButtonState previousState;

        std::unordered_map<Button, sf::Keyboard::Key> keybindings;

    public:
        RealTimeInput();
        virtual ~RealTimeInput() { }
        
        virtual const bool isUp(const Button &button) const;
        virtual const bool isDown(const Button &button) const;
        virtual const bool isHeld(const Button &button) const;
        virtual const bool wasPressed(const Button &button) const;
        virtual const bool wasReleased(const Button &button) const;

        /**
         * Updates the state of the virtual gamepad by reading the keyboard
         * state. This should be called at the beginning of every frame.
         */
        virtual void update(float dt);

        /**
         * Binds a particular keyboard key to a button.
         *
         * @param  button the virtual button to set binding for
         * @param  key    the keyboard key to trigger the button
         */
        void bindKey(const Button & button, sf::Keyboard::Key key);
    };
    
} // hikari

#endif // HIKARI_CLIENT_GAME_REALTIMEINPUT