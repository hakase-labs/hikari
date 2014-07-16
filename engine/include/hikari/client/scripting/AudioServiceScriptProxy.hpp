#ifndef HIKARI_CLIENT_AUDIOSERVICESCRIPTPROXY
#define HIKARI_CLIENT_AUDIOSERVICESCRIPTPROXY

#include <memory>
#include <string>

namespace hikari {

    class AudioService;

    /**
     * A proxy class for exposing an AudioService instance as a static set of
     * methods. This should be used to bind the service to the scripting layer.
     * 
     * @see AudioService
     */
    class AudioServiceScriptProxy {
    private:
        static std::weak_ptr<AudioService> audioService;

        AudioServiceScriptProxy();                                            // Prevent construction
        AudioServiceScriptProxy(const AudioServiceScriptProxy& other);        // Prevent copy-construction
        AudioServiceScriptProxy & operator=(const AudioServiceScriptProxy &); // Prevent copy-by-assignment

    public:
        static std::weak_ptr<AudioService> getWrappedService();
        static void setWrappedService(const std::weak_ptr<AudioService> & audioService);

        // static void playMusic(int id);
        static void playMusic(const std::string & name);
        static void stopMusic();
        // static void playSample(int id);
        static void playSample(const std::string & name);
        static void stopAllSamples();

        static bool isMusicLoaded();
        static bool isSamplesLoaded();
    };

} // hikari

#endif // HIKARI_CLIENT_AUDIOSERVICESCRIPTPROXY
