#ifndef HIKARI_CLIENT_GAME_REALTIMEINPUT
#define HIKARI_CLIENT_GAME_REALTIMEINPUT

#include "hikari/core/Platform.hpp"
#include "hikari/client/game/Input.hpp"

#include <SFML/Window/Keyboard.hpp>

#include <unordered_map>

namespace hikari {

    class RealTimeInput : public Input {
    private:
        static const bool BUTTON_PUSHED = true;

        bool previousUp;
        bool previousRight;
        bool previousDown;
        bool previousLeft;
        bool previousShoot;
        bool previousJump;
        bool currentUp;
        bool currentRight;
        bool currentDown;
        bool currentLeft;
        bool currentShoot;
        bool currentJump;

        std::unordered_map<Button, sf::Keyboard::Key> keybindings;

    public:
        RealTimeInput();
        virtual ~RealTimeInput() { }
        
        virtual const bool isUp(const Button &button) const;
        virtual const bool isDown(const Button &button) const;
        virtual const bool isHeld(const Button &button) const;
        virtual const bool wasPressed(const Button &button) const;
        virtual const bool wasReleased(const Button &button) const;

        void update();

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