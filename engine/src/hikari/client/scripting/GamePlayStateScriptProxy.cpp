#include "hikari/client/scripting/GamePlayStateScriptProxy.hpp"
#include "hikari/client/game/GamePlayState.hpp"

namespace hikari {

    std::weak_ptr<GamePlayState> GamePlayStateScriptProxy::gamePlayState = std::weak_ptr<GamePlayState>();

    GamePlayStateScriptProxy::GamePlayStateScriptProxy() {
        // Private to prevent construction
    }

    GamePlayStateScriptProxy::GamePlayStateScriptProxy(const GamePlayStateScriptProxy& other) {
        // Private to prevent copy construction
    }

    std::weak_ptr<GamePlayState> GamePlayStateScriptProxy::getWrappedService() {
        return gamePlayState;
    }

    void GamePlayStateScriptProxy::setWrappedService(const std::weak_ptr<GamePlayState> & gamePlayState) {
        GamePlayStateScriptProxy::gamePlayState = gamePlayState;
    }

    void GamePlayStateScriptProxy::refillPlayerEnergy(int amount) {
        if(const auto & state = gamePlayState.lock()) {
            state->refillPlayerEnergy(amount);
        }
    }

    void GamePlayStateScriptProxy::refillWeaponEnergy(int amount) {
        if(const auto & state = gamePlayState.lock()) {
            state->refillWeaponEnergy(amount);
        }
    }

} // hikari
