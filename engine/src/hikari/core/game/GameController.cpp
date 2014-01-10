#include "hikari/core/game/GameController.hpp"
#include "hikari/core/game/GameControllerException.hpp"
#include "hikari/core/game/StateTransition.hpp"
#include "hikari/core/game/FadeStateTransition.hpp"
#include "hikari/core/game/SliceStateTransition.hpp"
#include "hikari/core/util/Log.hpp"
#include <iostream>

namespace hikari {

    GameController::GameController()
        : state(new DefaultGameState())
        , enqueuedNextState(nullptr)
        , currState(state->getName())
        , prevState(state->getName())
        , nextState(state->getName()) 
    {

    }

    void GameController::addState(const std::string &name, const StatePtr &state) {
        if(states.find(name) == std::end(states)) {
            states[name] = state;
            HIKARI_LOG(debug) << "<GameController> added state \"" << name << "\".";
        } else {
            throw GameControllerException("Tried to register more than one state: \"" + name + "\".");
        }
    }

    void GameController::setState(const std::string &name) {
        auto found = states.find(name);

        if(found == std::end(states)) {
            throw GameControllerException("Tried to set state to an unregistered state: \"" + name + "\".");
        } else {
            state->onExit();
            HIKARI_LOG(debug) << "<GameController> exited \"" << state->getName() << "\" state.";
            prevState = state->getName();
            currState = name;
            state = found->second;
            state->onEnter();
            HIKARI_LOG(debug) << "<GameController> entered \"" << state->getName() << "\" state.";
        }
    }

    void GameController::setState(const StatePtr & statePtr) {
        if(statePtr) {
            state->onExit();
            HIKARI_LOG(debug) << "<GameController> exited \"" << state->getName() << "\" state.";
            prevState = state->getName();
            currState = statePtr->getName();
            state = statePtr;
            state->onEnter();
            HIKARI_LOG(debug) << "<GameController> entered \"" << state->getName() << "\" state.";
        }
    }

    void GameController::gotoNextState() {
        if(enqueuedNextState) {
            setState(enqueuedNextState);
        }

        enqueuedNextState.reset();
    }

    void GameController::setNextState(const std::string &name) {
        nextState = name;
    }

    void GameController::requestStateChange(const std::string & stateName) {
        requestStateChange(
            stateName,
            // std::unique_ptr<StateTransition>(new SliceStateTransition(SliceStateTransition::SLICE_LEFT, 1.066666666666667f)),
            // std::unique_ptr<StateTransition>(new DefaultStateTransition(DefaultStateTransition::ENTERING))
            std::unique_ptr<StateTransition>(new FadeStateTransition(FadeStateTransition::FADE_OUT, sf::Color::Black, (1.0f/60.0f*13.0f))),
            std::unique_ptr<StateTransition>(new FadeStateTransition(FadeStateTransition::FADE_IN, sf::Color::Black, (1.0f/60.0f*13.0f)))
        );
    }

    void GameController::requestStateChange(const std::string & stateName,
            std::unique_ptr<StateTransition> outTransition,
            std::unique_ptr<StateTransition> inTransition) {

        auto found = states.find(stateName);

        HIKARI_LOG(debug) << "State change requested";
        if(found != std::end(states)) {
            HIKARI_LOG(debug) << "Found the next state: " << stateName;
            enqueuedNextState = found->second;

            if(outTransition) {
                outTransition->setExitingState(state);
                outTransition->setEnteringState(enqueuedNextState);
                this->outTransition = std::move(outTransition);
            }

            if(inTransition) {
                inTransition->setExitingState(state);
                inTransition->setEnteringState(enqueuedNextState);
                this->inTransition = std::move(inTransition);
            }
        }
    }

    void GameController::handleEvent(sf::Event &event) {
        if(!state) {
            throw GameControllerException("Current game state is null, cannot handle event.");
        }

        state->handleEvent(event);
    }

    void GameController::render(sf::RenderTarget &target) {
        if(!state) {
            throw GameControllerException("Current game state is null, cannot render.");
        }

        if(outTransition) {
            outTransition->render(target);
        } else if(inTransition) {
            inTransition->render(target);
        } else {
            state->render(target);                
        }
    }

    void GameController::update(float dt) {
        if(!state) {
            throw GameControllerException("Current game state is null, cannot update.");
        }

        if(enqueuedNextState) {
            if(outTransition) {
                HIKARI_LOG(debug4) << "Has out transition";
                if(outTransition->isComplete()) {
                    HIKARI_LOG(debug4) << "Transition complete!";
                    outTransition.reset();
                    gotoNextState();
                } else {
                    outTransition->update(dt);
                }
            }
        } else {
            if(inTransition) {
                HIKARI_LOG(debug4) << "Has in transition";
                if(inTransition->isComplete()) {
                    HIKARI_LOG(debug4) << "In transition complete!";
                    inTransition.reset();
                } else {
                    inTransition->update(dt);
                }
            } else {
                state->update(dt);
            }
        }
    }

    GameController::DefaultGameState::DefaultGameState()
        : name("default") {

    }

    void GameController::DefaultGameState::handleEvent(sf::Event &event) {

    }

    void GameController::DefaultGameState::render(sf::RenderTarget &target) {

    }

    bool GameController::DefaultGameState::update(float dt) {
        return false;
    }

    void GameController::DefaultGameState::onEnter() { 
    
    }

    void GameController::DefaultGameState::onExit() { 
    
    }

    const std::string &GameController::DefaultGameState::getName() const {
        return name;
    }

    GameController::DefaultStateTransition::DefaultStateTransition(StateDirection direction)
        : direction(direction)
    {
        setComplete(true);
    }

    GameController::DefaultStateTransition::~DefaultStateTransition() {

    }

    void GameController::DefaultStateTransition::render(sf::RenderTarget &target) {
        if(direction == ENTERING) {
            if(enteringState) {
                enteringState->render(target);
            }
        } else {
            if(exitingState) {
                exitingState->render(target);
            }
        }
    }

    void GameController::DefaultStateTransition::update(float dt) {
        HIKARI_LOG(debug4) << "DefaultStateTransition::update()";
    }


} // hikari