#include "hikari/core/game/GameController.hpp"
#include "hikari/core/game/GameControllerException.hpp"
#include "hikari/core/game/StateTransition.hpp"
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

    void GameController::requestStateChange(const std::string & stateName, std::unique_ptr<StateTransition> stateTransition) {
        auto found = states.find(stateName);
        HIKARI_LOG(debug) << "State change requested";
        if(found != std::end(states)) {
            HIKARI_LOG(debug) << "Found the next state: " << stateName;
            enqueuedNextState = found->second;

            if(stateTransition) {
                stateTransition->setExitingState(state);
                stateTransition->setEnteringState(enqueuedNextState);
                this->stateTransition = std::move(stateTransition);
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

        if(stateTransition) {
            stateTransition->render(target);
        } else {
            state->render(target);
        }
    }

    void GameController::update(float dt) {
        if(!state) {
            throw GameControllerException("Current game state is null, cannot update.");
        }

        if(enqueuedNextState) {
            if(stateTransition) {
                HIKARI_LOG(debug4) << "Has transition";
                if(stateTransition->isComplete()) {
                    HIKARI_LOG(debug4) << "Transition complete!";
                    stateTransition.reset();
                    gotoNextState();
                } else {
                    stateTransition->update(dt);
                }
            } else {
                gotoNextState();
            }
        } else {
            state->update(dt);
        }
    }

    GameController::DefaultGameState::DefaultGameState()
        : name("default") {

    }

    void GameController::DefaultGameState::handleEvent(sf::Event &event) {

    }

    void GameController::DefaultGameState::render(sf::RenderTarget &target) {

    }

    bool GameController::DefaultGameState::update(const float &dt) {
        return false;
    }

    void GameController::DefaultGameState::onEnter() { 
    
    }

    void GameController::DefaultGameState::onExit() { 
    
    }

    const std::string &GameController::DefaultGameState::getName() const {
        return name;
    }

    GameController::DefaultStateTransition::DefaultStateTransition() {
        setComplete(true);
    }

    GameController::DefaultStateTransition::~DefaultStateTransition() {

    }

    void GameController::DefaultStateTransition::render(sf::RenderTarget &target) {
        if(exitingState) {
            exitingState->render(target);
        }
    }

    void GameController::DefaultStateTransition::update(float dt) {
        HIKARI_LOG(debug4) << "DefaultStateTransition::update()";
    }


} // hikari