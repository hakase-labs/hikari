#ifndef HIKARI_CORE_GAME_GAMECONTROLLER
#define HIKARI_CORE_GAME_GAMECONTROLLER

#include <map>
#include <string>
#include <memory>
#include "hikari/core/Platform.hpp"
#include "hikari/core/game/GameState.hpp"
#include "hikari/core/game/StateTransition.hpp"

#if (_WIN32 && _MSC_VER)
    #pragma warning(push)
    #pragma warning(disable:4251)
#endif

namespace sf {
    class RenderTarget;
}

namespace hikari {

    typedef std::shared_ptr<GameState> StatePtr;
    class StateTransition;

    class HIKARI_API GameController {
    private:
        StatePtr state;
        StatePtr enqueuedNextState;
        std::string currState;
        std::string prevState;
        std::string nextState;
        std::map< std::string, StatePtr > states;
        std::unique_ptr<StateTransition> outTransition;
        std::unique_ptr<StateTransition> inTransition;

        void gotoNextState();

        /**
            DefaultGameState is used by a GameController instance as it's current state
            at time of construction. It is a simple State which does nothing -- it's
            simply a placeholder.
        */
        class DefaultGameState : public GameState {
        private:
            const std::string name;
        public:
            DefaultGameState();
            virtual ~DefaultGameState() { }
            virtual void handleEvent(sf::Event &event);
            virtual void render(sf::RenderTarget &target);
            virtual bool update(const float &dt);
            virtual void onEnter();
            virtual void onExit();
            virtual const std::string &getName() const;
        };

        class DefaultStateTransition : public StateTransition {
        public:
            DefaultStateTransition();
            virtual ~DefaultStateTransition();

            virtual void render(sf::RenderTarget &target);
            virtual void update(float dt);
        };

    public:
        GameController();

        void addState(const std::string &name, const StatePtr &state);

        /**
            Immediately trasitions from the current state in to the new state specified
            by name. If no state can be identified for the given name an exception will
            be thrown.

            The name supplied will also be set as the "next" state.
        */
        void setState(const std::string &name);
        void setState(const StatePtr & statePtr);

        void requestStateChange(const std::string & stateName);
        void requestStateChange(const std::string & stateName,
                std::unique_ptr<StateTransition> outTransition,
                std::unique_ptr<StateTransition> inTransition);

        /**
            Sets the next state to transition to but does not cause the current state to
            transition.
        */
        void setNextState(const std::string &name);

        void handleEvent(sf::Event &event);
        void render(sf::RenderTarget &target);
        void update(float dt);
    };

} // hikari

#if (_WIN32 && _MSC_VER)
    #pragma warning(pop)
#endif

#endif // HIKARI_CORE_GAME_GAMECONTROLLER
