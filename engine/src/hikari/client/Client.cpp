#include "hikari/client/Client.hpp"
#include "hikari/client/Services.hpp"
#include "hikari/client/audio/AudioService.hpp"
#include "hikari/client/game/GameProgress.hpp"
#include "hikari/client/game/WeaponTable.hpp"
#include "hikari/client/game/objects/EnemyFactory.hpp"
#include "hikari/client/game/objects/FactoryHelpers.hpp"
#include "hikari/client/game/objects/ItemFactory.hpp"
#include "hikari/client/game/objects/ProjectileFactory.hpp"
#include "hikari/client/gui/GuiService.hpp"
#include "hikari/client/scripting/SquirrelService.hpp"
#include "hikari/client/scripting/AudioServiceScriptProxy.hpp"

#include "hikari/core/game/AnimationLoader.hpp"
#include "hikari/core/game/map/MapLoader.hpp"
#include "hikari/core/game/map/TilesetLoader.hpp"
#include "hikari/core/util/AnimationSetCache.hpp"
#include "hikari/core/util/FileSystem.hpp"
#include "hikari/core/util/ImageCache.hpp"
#include "hikari/core/util/Log.hpp"
#include "hikari/core/util/PhysFS.hpp"
#include "hikari/core/util/TilesetCache.hpp"

#include <json/reader.h>
 
namespace hikari {
  
    const std::string Client::APP_TITLE             = "hikari";
    const std::string Client::PATH_CONTENT          = "content.zip";
    const std::string Client::PATH_CUSTOM_CONTENT   = "custom.zip";
    const std::string Client::PATH_CONFIG_FILE      = "conf.json";
    const std::string Client::PATH_GAME_CONFIG_FILE = "game.json";
 
    const unsigned int Client::SCREEN_WIDTH          = 256;
    const unsigned int Client::SCREEN_HEIGHT         = 240;
    const unsigned int Client::SCREEN_BITS_PER_PIXEL = 32;
 
    Client::Client(int argc, char** argv)
        : gameConfigJson()
        , clientConfig()
        , services()
        , videoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BITS_PER_PIXEL)
        , window()
        , screenBuffer()
    {
        initLogging(argc, argv);
        initFileSystem(argc, argv);
        initConfig();
    }
 
    void Client::initConfig() {
        // Load client config first
        if(FileSystem::exists(PATH_CONFIG_FILE)) {
            auto fs = FileSystem::openFile(PATH_CONFIG_FILE);

            Json::Reader reader;
            Json::Value value;

            bool success = reader.parse(*fs, value, false);

            if(!success) {
                HIKARI_LOG(info) << "Configuration file could not be found or was corrupt, using defaults.";
            } else {
                clientConfig = ClientConfig(value);
            }
        } else {
            HIKARI_LOG(fatal) << "Couldn't find configuration file '" << PATH_CONFIG_FILE << "'";
        }

        // Then load the game config
        if(FileSystem::exists(PATH_GAME_CONFIG_FILE)) {
            auto fs = FileSystem::openFile(PATH_GAME_CONFIG_FILE);

            Json::Reader reader;
            Json::Value value;

            bool success = reader.parse(*fs, value, false);

            if(!success) {
                HIKARI_LOG(fatal) << "Game configuration file could not be found or was corrupt.";
            } else {
                gameConfigJson = value;
            }
        } else {
            HIKARI_LOG(fatal) << "Couldn't find game configuration file '" << PATH_GAME_CONFIG_FILE << "'";
        }
    }
 
    void Client::initFileSystem(int argc, char** argv) {
        // Create virtual file system
        PhysFS::init(argv[0]);
        PhysFS::addToSearchPath(PhysFS::getBaseDir());

        // Load standard content
        try {
            PhysFS::addToSearchPath(PhysFS::getBaseDir() + PATH_CONTENT);
        } catch(PhysFS::Exception & ex) {
            HIKARI_LOG(error) << "Couldn't load content package. Reason: " << ex.what();
        }

        // Load custom content (if it exists)
        try {
            PhysFS::addToSearchPath(PhysFS::getBaseDir() + PATH_CUSTOM_CONTENT);
        } catch(PhysFS::Exception & ex) {
            HIKARI_LOG(warning) << "(This is normal) Couldn't load custom content package. Reason: " << ex.what();
        }

        PhysFS::setWriteDir(PhysFS::getBaseDir());
    }
 
    void Client::initGame() {
        loadScriptingEnvironment();
        loadObjectTemplates();
    }
 
    void Client::initLogging(int argc, char** argv) {
        ::hikari::Log::setReportingLevel(debug4);
    }
 
    void Client::initServices() {
        auto imageCache        = std::make_shared<ImageCache>(ImageCache::NO_SMOOTHING, ImageCache::USE_MASKING);
        auto animationSetCache = std::make_shared<AnimationSetCache>();
        auto animationLoader   = std::make_shared<AnimationLoader>();
        auto tilesetLoader     = std::make_shared<TilesetLoader>(imageCache, animationLoader);
        auto tilesetCache      = std::make_shared<TilesetCache>(tilesetLoader);
        auto mapLoader         = std::make_shared<MapLoader>(tilesetCache);
        auto gameProgress      = std::make_shared<GameProgress>();
        auto audioService      = std::make_shared<AudioService>(gameConfigJson["assets"]["audio"]);
        auto squirrelService   = std::make_shared<SquirrelService>(clientConfig.getScriptingStackSize());
        auto guiService        = std::make_shared<GuiService>(gameConfigJson, imageCache, screenBuffer);
        auto itemFactory       = std::make_shared<ItemFactory>(animationSetCache, imageCache, squirrelService);
        auto enemyFactory      = std::make_shared<EnemyFactory>(animationSetCache, imageCache, squirrelService);
        auto projectileFactory = std::make_shared<ProjectileFactory>(animationSetCache, imageCache, squirrelService);
        auto weaponTable       = std::make_shared<WeaponTable>();

        services.registerService(Services::AUDIO,             audioService);
        services.registerService(Services::GAMEPROGRESS,      gameProgress);
        services.registerService(Services::IMAGECACHE,        imageCache);
        services.registerService(Services::ANIMATIONSETCACHE, animationSetCache);
        services.registerService(Services::MAPLOADER,         mapLoader);
        services.registerService(Services::SCRIPTING,         squirrelService);
        services.registerService(Services::GUISERVICE,        guiService);
        services.registerService(Services::ITEMFACTORY,       itemFactory);
        services.registerService(Services::ENEMYFACTORY,      enemyFactory);
        services.registerService(Services::PROJECTILEFACTORY, projectileFactory);
        services.registerService(Services::WEAPONTABLE,       weaponTable);

        // Script wrappers/proxy classes
        AudioServiceScriptProxy::setWrappedService(std::weak_ptr<AudioService>(audioService));
    }
 
    void Client::deinitFileSystem() {
        PhysFS::deinit();
    }
 
    void Client::loadScriptingEnvironment() {
        if(auto squirrelService = services.locateService<SquirrelService>(Services::SCRIPTING).lock()) {
            squirrelService->runScriptFile("assets/scripts/Environment.nut");
            squirrelService->runScriptFile("assets/scripts/Bootstrap.nut");
        }
    }
 
    void Client::loadObjectTemplates() {
 
    }

    int Client::run() {
        sf::Clock clock;
        window.create(videoMode, APP_TITLE, sf::Style::Default);
        window.setActive(true);
        window.setVerticalSyncEnabled(clientConfig.isVsyncEnabled());
        window.setKeyRepeatEnabled(false);
        screenBuffer.create(videoMode.width, videoMode.height);

        initServices();
        initGame();

        sf::Event event;
        bool quit = false;

        while(!quit) {
            while(window.pollEvent(event)) {
                if(event.type == sf::Event::Closed) {
                    quit = true;
                }
            }
        }

        return 0;
    }
 
} // hikari
