#include "hikari/core/game/StateTransition.hpp"
#include "hikari/core/game/GameState.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace hikari {

    StateTransition::StateTransition()
        : exitingState(nullptr)
        , enteringState(nullptr)
        , completeFlag(false)
    {

    }

    StateTransition::~StateTransition() {

    }

    void StateTransition::setComplete(bool completeFlag) {
        this->completeFlag = completeFlag;
    }

    void StateTransition::setExitingState(const std::shared_ptr<GameState> & exitingState) {
        this->exitingState = exitingState;
    }

    void StateTransition::setEnteringState(const std::shared_ptr<GameState> & enteringState) {
        this->enteringState = enteringState;
    }

    bool StateTransition::isComplete() const {
        return completeFlag;
    }

    void StateTransition::render(sf::RenderTarget & target) {

    }

    void StateTransition::update(float dt) {

    }

    //     /**
    //      * Determines if the transition is complete or not.
    //      * @return true if complete, false if still working
    //      */
    //     bool isComplete() const;
    //     void render(sf::RenderTarget &target);
    //     void update(float dt);

} // hikari