#ifndef HIKARI_CLIENT_GAME_KEYBOARDINPUT
#define HIKARI_CLIENT_GAME_KEYBOARDINPUT

#include "hikari/core/Platform.hpp"
#include "hikari/client/game/Input.hpp"
#include <SFML/Window/Event.hpp>

namespace hikari {

    class KeyboardInput : public Input {
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

    public:
        KeyboardInput(/* key mapping */);
        virtual ~KeyboardInput() { }
        
        virtual const bool isUp(const Button &button) const;
        virtual const bool isDown(const Button &button) const;
        virtual const bool isHeld(const Button &button) const;
        virtual const bool wasPressed(const Button &button) const;
        virtual const bool wasReleased(const Button &button) const;

        void update(const sf::Event &keyboardEvent);
    };
    
} // hikari

#endif // HIKARI_CLIENT_GAME_KEYBOARDINPUT