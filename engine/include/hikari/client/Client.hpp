#ifndef HIKARI_CLIENT
#define HIKARI_CLIENT
 
#include "hikari/client/ClientConfig.hpp"
#include "hikari/core/util/ServiceLocator.hpp"

#include <json/value.h>

#include <SFML/Graphics.hpp>

#include <string>
 
namespace hikari {
 
    class Client {
    private:
        static const std::string APP_TITLE;
        static const std::string PATH_CONTENT;
        static const std::string PATH_CUSTOM_CONTENT;
        static const std::string PATH_CONFIG_FILE;
        static const std::string PATH_GAME_CONFIG_FILE;
 
        static const unsigned int SCREEN_WIDTH;
        static const unsigned int SCREEN_HEIGHT;
        static const unsigned int SCREEN_BITS_PER_PIXEL;
        
        /**
         * Initializes the client from the configuration file.
         */
        void initConfig();

        /**
         * Initializes the virtual file system used to load content files.
         *
         * @param  argc the argument count
         * @param  argv c-style string containing program arguments
         */
        void initFileSystem(int argc, char** argv);
        void initGame();
        void initLogging(int argc, char** argv);
        void initServices();
 
        void deinitFileSystem();
 
        void loadScriptingEnvironment();
        void loadObjectTemplates();

        void loop();
        
        Json::Value gameConfigJson;
        ClientConfig clientConfig;
        ServiceLocator services;
        sf::VideoMode videoMode;
        sf::RenderWindow window;
        sf::RenderTexture screenBuffer;
 
    public:
        Client(int argc, char** argv);
        int run();
    };
 
}
 
#endif // HIKARI_CLIENT

// #ifndef HIKARI_CLIENT
// #define HIKARI_CLIENT

// #include "hikari/client/ClientConfig.hpp"
// #include "hikari/core/util/ServiceLocator.hpp"

// #include <json/value.h>
// #include <string>
// #include <memory>

// //
// // Forward declarations
// //
// namespace hikari {
//     class ItemFactory;
//     class SquirrelService;
//     class AnimationSetCache;
//     class ImageCache;
//     class WeaponTable;
// }

// //
// // Constants and stuff
// //
// const int SCREEN_WIDTH = 256;
// const int SCREEN_HEIGHT = 240;
// const int SCREEN_BPP = 32;

// const std::string APPLICATION_TITLE("Hikari");
// const std::string PATH_CONTENT_PACKAGE("content.zip");
// const std::string PATH_CUSTOM_PACKAGE("custom.zip");

// int main(int argc, char** argv);

// void initLogging(int argc, char** argv);
// void initFileSystem(int argc, char** argv);
// void initConfiguration(const std::string &fileName, Json::Value& value);
// void initGame(const std::string &fileName, Json::Value &value);

// void loadWeapons(const std::shared_ptr<hikari::WeaponTable> & weaponTable);

// namespace hikari {

//     class Client {
//     private:
//         void initializeGame();
//         void initializeServices();
//         void loadConfiguration(const std::string& fileName, Json::Value& value);

//         Json::Value clientConfigJson;
//         Json::Value gameConfigJson;
//         ClientConfig clientConfig;
//         ServiceLocator services;
//     public:
//         Client(int argc, char** argv);
//         int run();
//     };

// }

// #endif // HIKARI_CLIENT
