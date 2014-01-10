#ifndef HIKARI_CORE_GAME_STATETRANSITION
#define HIKARI_CORE_GAME_STATETRANSITION

#include <map>
#include <string>
#include <memory>
#include "hikari/core/Platform.hpp"

namespace sf {
    class RenderTarget;
}

namespace hikari {

    class GameState;

    /**
     * A class that handles the transition between two GameState instances. This
     * class enables things like fading to black, crossfading, and other kinds 
     * of screen effects when one state changes to another.
     */
    class HIKARI_API StateTransition {
    private:
        bool completeFlag;

    protected:
        std::shared_ptr<GameState> exitingState;
        std::shared_ptr<GameState> enteringState;
        void setComplete(bool completeFlag);

    public:
        StateTransition();
        virtual ~StateTransition();

        void setExitingState(const std::shared_ptr<GameState> & exitingState);
        void setEnteringState(const std::shared_ptr<GameState> & enteringState);

        /**
         * Determines if the transition is complete or not.
         * @return true if complete, false if still working
         */
        bool isComplete() const;
        virtual void render(sf::RenderTarget &target);
        virtual void update(float dt);
    };

} // hikari

#endif // HIKARI_CORE_GAME_STATETRANSITION
