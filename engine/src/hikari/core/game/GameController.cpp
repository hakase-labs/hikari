#include <hikari/core/game/GameController.hpp>
#include <hikari/core/game/GameControllerException.hpp>
#include <hikari/core/util/Log.hpp>
#include <iostream>

namespace hikari {

    GameController::GameController()
        : state(new DefaultGameState())
        , currState(state->getName())
        , prevState(state->getName())
        , nextState(state->getName()) 
    {

    }

    void GameController::addState(const std::string &name, const StatePtr &state) {
        if(states.find(name) == states.end()) {
            states[name] = state;
            HIKARI_LOG(debug) << "<GameController> added state \"" << name << "\".";
        } else {
            throw GameControllerException("Tried to register more than one state: \"" + name + "\".");
        }
    }

    void GameController::setState(const std::string &name) {
        auto found = states.find(name);

        if(found == states.end()) {
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

    void GameController::setNextState(const std::string &name) {
        nextState = name;
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

        state->render(target);
    }

    void GameController::update(const float &dt) {
        if(!state) {
            throw GameControllerException("Current game state is null, cannot update.");
        }

        bool transition = state->update(dt);
        
        if(transition) {
            HIKARI_LOG(debug) << "<GameController> state change requested, setting state to \"" << nextState << "\".";
            setState(nextState);
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

} // hikari