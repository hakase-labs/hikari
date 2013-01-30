#ifndef HIKARI_CLIENT_GAME_REALTIMEINPUT
#define HIKARI_CLIENT_GAME_REALTIMEINPUT

#include "hikari/core/Platform.hpp"
#include "hikari/client/game/Input.hpp"
#include <SFML/Window/Keyboard.hpp>

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

    public:
        RealTimeInput();
        virtual ~RealTimeInput() { }
        
        virtual const bool isUp(const Button &button) const;
        virtual const bool isDown(const Button &button) const;
        virtual const bool isHeld(const Button &button) const;
        virtual const bool wasPressed(const Button &button) const;
        virtual const bool wasReleased(const Button &button) const;

        void update();
    };
    
} // hikari

#endif // HIKARI_CLIENT_GAME_REALTIMEINPUT