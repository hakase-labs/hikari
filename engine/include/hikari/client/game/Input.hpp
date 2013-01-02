#ifndef HIKARI_CLIENT_GAME_INPUT
#define HIKARI_CLIENT_GAME_INPUT

#include <hikari/core/Platform.hpp>

namespace hikari {

    class Input {
    public:
        typedef int Button;

        static const Button BUTTON_UP = (1 << 0);
        static const Button BUTTON_RIGHT = (1 << 1);
        static const Button BUTTON_DOWN = (1 << 2);
        static const Button BUTTON_LEFT = (1 << 3);
        static const Button BUTTON_SHOOT = (1 << 4);
        static const Button BUTTON_JUMP = (1 << 5);

        virtual ~Input() { }
        
        virtual const bool isUp(const Button &button) const = 0;
        virtual const bool isDown(const Button &button) const = 0;
        virtual const bool isHeld(const Button &button) const = 0;
        virtual const bool wasPressed(const Button &button) const = 0;
        virtual const bool wasReleased(const Button &button) const = 0;
    };
    
} // hikari

#endif // HIKARI_CLIENT_GAME_INPUT