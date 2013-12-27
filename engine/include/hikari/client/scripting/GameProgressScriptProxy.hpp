#ifndef HIKARI_CLIENT_GAMEPROGRESSSCRIPTPROXY
#define HIKARI_CLIENT_GAMEPROGRESSSCRIPTPROXY

#include <memory>

namespace hikari {

    class GameProgress;

    /**
     * A proxy class for exposing an GameProgress instance as a static set of
     * methods. This should be used to bind the service to the scripting layer.
     * Also provides some conveninence functions for typical functions used by
     * scripts.
     *
     * @see GameProgress
     */
    class GameProgressScriptProxy {
    private:
        static std::weak_ptr<GameProgress> gameProgress;

        GameProgressScriptProxy();                                            // Prevent construction
        GameProgressScriptProxy(const GameProgressScriptProxy& other);        // Prevent copy-construction
        GameProgressScriptProxy & operator=(const GameProgressScriptProxy &); // Prevent copy-by-assignment

    public:
        static std::weak_ptr<GameProgress> getWrappedService();
        static void setWrappedService(const std::weak_ptr<GameProgress> & gameProgress);

        static int getLives();
        static unsigned char getETanks();
        static unsigned char getMTanks();

        static void setLives(int value);
        static void setETanks(unsigned char value);
        static void setMTanks(unsigned char value);
    };

} // hikari

#endif // HIKARI_CLIENT_GAMEPROGRESSSCRIPTPROXY
