#ifndef HIKARI_CLIENT_GAMEPLAYSTATESCRIPTSPROXY
#define HIKARI_CLIENT_GAMEPLAYSTATESCRIPTSPROXY

#include <memory>

namespace hikari {

    class GamePlayState;

    /**
     * A proxy class for exposing an GamePlayState instance as a static set of
     * methods. This should be used to bind the service to the scripting layer.
     * Also provides some conveninence functions for typical functions used by
     * scripts.
     *
     * @see GamePlayState
     */
    class GamePlayStateScriptProxy {
    private:
        static std::weak_ptr<GamePlayState> gamePlayState;

        GamePlayStateScriptProxy();                                             // Prevent construction
        GamePlayStateScriptProxy(const GamePlayStateScriptProxy& other);        // Prevent copy-construction
        GamePlayStateScriptProxy & operator=(const GamePlayStateScriptProxy &); // Prevent copy-by-assignment

    public:
        static std::weak_ptr<GamePlayState> getWrappedService();
        static void setWrappedService(const std::weak_ptr<GamePlayState> & gameProgress);

        static void refillPlayerEnergy(int amount);
        static void refillWeaponEnergy(int amount);
    };

} // hikari

#endif // HIKARI_CLIENT_GAMEPLAYSTATESCRIPTSPROXY
