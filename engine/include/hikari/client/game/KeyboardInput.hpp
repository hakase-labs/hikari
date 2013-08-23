#ifndef HIKARI_CLIENT_GAME_KEYBOARDINPUT
#define HIKARI_CLIENT_GAME_KEYBOARDINPUT

#include "hikari/core/Platform.hpp"
#include "hikari/client/game/Input.hpp"
#include <SFML/Window/Event.hpp>

namespace hikari {

    class KeyboardInput : public Input {
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

    public:
        KeyboardInput(/* key mapping */);
        virtual ~KeyboardInput() { }
        
        virtual const bool isUp(const Button &button) const;
        virtual const bool isDown(const Button &button) const;
        virtual const bool isHeld(const Button &button) const;
        virtual const bool wasPressed(const Button &button) const;
        virtual const bool wasReleased(const Button &button) const;

        void processEvent(const sf::Event &keyboardEvent);
        virtual void update(float dt);
    };
    
} // hikari

#endif // HIKARI_CLIENT_GAME_KEYBOARDINPUT