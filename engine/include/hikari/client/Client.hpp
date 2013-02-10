#ifndef HIKARI_CLIENT
#define HIKARI_CLIENT

#include "hikari/client/ClientConfig.hpp"
#include "hikari/core/util/ServiceLocator.hpp"

#include <json/value.h>
#include <string>

//
// Forward declarations
//
namespace hikari {
    class ItemFactory;
    class SquirrelService;
    class AnimationSetCache;
    class ImageCache;
}

//
// Constants and stuff
//
const int SCREEN_WIDTH = 256;
const int SCREEN_HEIGHT = 240;
const int SCREEN_BPP = 32;

const std::string APPLICATION_TITLE("Hikari");
const std::string PATH_CONTENT_PACKAGE("content.zip");

int main(int argc, char** argv);

void initLogging(int argc, char** argv);
void initFileSystem(int argc, char** argv);
void initConfiguration(const std::string &fileName, Json::Value& value);
void initGame(const std::string &fileName, Json::Value &value);

void populateCollectableItemFactory(
    std::weak_ptr<hikari::ItemFactory> & factory,
    hikari::SquirrelService & squirrel, 
    hikari::AnimationSetCache & animationSetCache, 
    hikari::ImageCache & imageCache
);

namespace hikari {

    class Client {
    private:
        void initializeGame();
        void initializeServices();
        void loadConfiguration(const std::string& fileName, Json::Value& value);

        Json::Value clientConfigJson;
        Json::Value gameConfigJson;
        ClientConfig clientConfig;
        ServiceLocator services;
    public:
        Client(int argc, char** argv);
        int run();
    };

}

#endif // HIKARI_CLIENT