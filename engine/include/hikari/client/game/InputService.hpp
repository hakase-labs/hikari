#ifndef HIKARI_CLIENT_GAME_INPUTSERVICE
#define HIKARI_CLIENT_GAME_INPUTSERVICE

#include "hikari/client/game/Input.hpp"
#include "hikari/core/util/Service.hpp"

#include <memory>

namespace hikari {

    /**
     * InputService is a decroator for an Input instance that exposes
     * the wrapped Input as a service.
     */
    class InputService : public Input, public Service {
    private:
        std::shared_ptr<Input> input;

    public:
        InputService(const std::shared_ptr<Input> & input);
        virtual ~InputService();

        virtual const bool isUp(const Input::Button &button) const;
        virtual const bool isDown(const Input::Button &button) const;
        virtual const bool isHeld(const Input::Button &button) const;
        virtual const bool wasPressed(const Input::Button &button) const;
        virtual const bool wasReleased(const Input::Button &button) const;
        virtual void update(float dt);
    };
    
} // hikari

#endif // HIKARI_CLIENT_GAME_INPUTSERVICE