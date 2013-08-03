#include "hikari/client/Client.hpp"
#include "hikari/client/Services.hpp"
#include "hikari/client/audio/AudioService.hpp"
#include "hikari/client/game/GameProgress.hpp"
#include "hikari/client/game/StageSelectState.hpp"
#include "hikari/client/game/GamePlayState.hpp"

#include "hikari/client/game/PasswordState.hpp"
#include "hikari/client/game/TitleState.hpp"
#include "hikari/client/game/WeaponTable.hpp"
#include "hikari/client/game/DamageTable.hpp"
#include "hikari/client/game/objects/EnemyFactory.hpp"
#include "hikari/client/game/objects/FactoryHelpers.hpp"
#include "hikari/client/game/objects/ItemFactory.hpp"
#include "hikari/client/game/objects/ProjectileFactory.hpp"
#include "hikari/client/game/objects/ParticleFactory.hpp"
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

#include <guichan/gui.hpp>

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
        , gameConfig()
        , services()
        , videoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BITS_PER_PIXEL)
        , window()
        , screenBuffer()
    {
        initLogging(argc, argv);
        initFileSystem(argc, argv);
        initConfig();
    }

    Client::~Client() {
        deinitFileSystem();
    }
 
    void Client::initConfig() {
        // Load client config first
        if(FileSystem::exists(PATH_CONFIG_FILE)) {
            auto fs = FileSystem::openFileRead(PATH_CONFIG_FILE);

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
            auto fs = FileSystem::openFileRead(PATH_GAME_CONFIG_FILE);

            Json::Reader reader;
            Json::Value value;

            bool success = reader.parse(*fs, value, false);

            if(!success) {
                HIKARI_LOG(fatal) << "Game configuration file could not be found or was corrupt.";
            } else {
                gameConfigJson = value;
                gameConfig = GameConfig(value);
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

        if(auto damageTable = services.locateService<DamageTable>(Services::DAMAGETABLE).lock()) {
            damageTable->addEntry(0, 0.0f);
            damageTable->addEntry(1, 10.0f);
            damageTable->addEntry(4, 6.0f);
            damageTable->addEntry(7, 1.0f);
        }

        // Create controller and game states
        StatePtr stageSelectState(new StageSelectState("stageselect", gameConfigJson["states"]["select"], controller, services));
        StatePtr gamePlayState(new GamePlayState("gameplay", controller, gameConfigJson, services));
        StatePtr passwordState(new PasswordState("password", gameConfigJson, services));
        StatePtr titleState(new TitleState("title", gameConfigJson, services));

        controller.addState(stageSelectState->getName(), stageSelectState);
        controller.addState(gamePlayState->getName(), gamePlayState);
        controller.addState(passwordState->getName(), passwordState);
        controller.addState(titleState->getName(), titleState);

        controller.setState(gameConfig.getInitialState());
    }
 
    void Client::initLogging(int argc, char** argv) {
        // #ifdef DEBUG
        //::hikari::Log::setReportingLevel(debug4);
        // #else
        ::hikari::Log::setReportingLevel(warning);
        // #endif
    }
 
    void Client::initServices() {
        auto imageCache        = std::make_shared<ImageCache>(ImageCache::NO_SMOOTHING, ImageCache::USE_MASKING);
        auto animationLoader   = std::make_shared<AnimationLoader>(std::weak_ptr<ImageCache>(imageCache));
        auto animationSetCache = std::make_shared<AnimationSetCache>(animationLoader);
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
        auto particleFactory   = std::make_shared<ParticleFactory>(animationSetCache, imageCache);
        auto weaponTable       = std::make_shared<WeaponTable>();
        auto damageTable       = std::make_shared<DamageTable>();

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
        services.registerService(Services::PARTICLEFACTORY,   particleFactory);
        services.registerService(Services::WEAPONTABLE,       weaponTable);
        services.registerService(Services::DAMAGETABLE,       damageTable);

        AnimationLoader::setImageCache(std::weak_ptr<ImageCache>(imageCache));

        // Script wrappers/proxy classes
        AudioServiceScriptProxy::setWrappedService(std::weak_ptr<AudioService>(audioService));
    }

    void Client::initWindow() {
        std::string videoScale = clientConfig.getVideoMode();
        bool enabledFullScreen = false;
        unsigned int screenScaler = 1;

        if(videoScale == ClientConfig::VIDEO_SCALE_1X) {
            screenScaler = 1;
        } else if(videoScale == ClientConfig::VIDEO_SCALE_2X) {
            screenScaler = 2;
        } else if(videoScale == ClientConfig::VIDEO_SCALE_3X) {
            screenScaler = 3;
        } else {
            if(videoScale == ClientConfig::VIDEO_SCALE_FULL) {
                enabledFullScreen = true;
            }
        }

        videoMode.width  = SCREEN_WIDTH  * screenScaler;
        videoMode.height = SCREEN_HEIGHT * screenScaler;

        // Due to some weirdness between OSX, Windows, and Linux, the window
        // needs to be created before anything serious can be done.
        window.create(videoMode, APP_TITLE, (enabledFullScreen ? sf::Style::Fullscreen : sf::Style::Default));
        window.setActive(true);
        window.setVerticalSyncEnabled(clientConfig.isVsyncEnabled());
        window.setKeyRepeatEnabled(false);

        // Screen buffer is hard-coded to be the size of the render area, in 
        // other words, it doesn't scale with the window size. When it is
        // rendered it will be stretched to fit the window. This makes it retain
        // its (desired) pixelated quality.
        screenBuffer.create(SCREEN_WIDTH, SCREEN_HEIGHT);

        screenBufferView.setSize(
            static_cast<float>(SCREEN_WIDTH),
            static_cast<float>(SCREEN_HEIGHT));

        screenBufferView.setCenter(
            static_cast<float>(SCREEN_WIDTH / 2),
            static_cast<float>(SCREEN_HEIGHT / 2));
    }
 
    void Client::deinitFileSystem() {
        PhysFS::deinit();
    }
 
    void Client::loadScriptingEnvironment() {
        if(auto squirrelService = services.locateService<SquirrelService>(Services::SCRIPTING).lock()) {
            const std::vector<std::string> & startupScripts = gameConfig.getStartUpScripts();

            std::for_each(std::begin(startupScripts), std::end(startupScripts), [&](const std::string & scriptPath) {
                squirrelService->runScriptFile(scriptPath);
            });
        }
    }
 
    void Client::loadObjectTemplates() {
        if(auto itemFactory = services.locateService<ItemFactory>(Services::ITEMFACTORY).lock()) {
            FactoryHelpers::populateCollectableItemFactory(
                gameConfig.getItemTemplatePath(),
                std::weak_ptr<ItemFactory>(itemFactory),
                services
            );
        }

        if(auto enemyFactory = services.locateService<EnemyFactory>(Services::ENEMYFACTORY).lock()) {
            FactoryHelpers::populateEnemyFactory(
                gameConfig.getEnemyTemplatePath(),
                std::weak_ptr<EnemyFactory>(enemyFactory),
                services
            );
        }

        if(auto particleFactory = services.locateService<ParticleFactory>(Services::PARTICLEFACTORY).lock()) {
            FactoryHelpers::populateParticleFactory(
                gameConfig.getParticleTemplatePath(),
                std::weak_ptr<ParticleFactory>(particleFactory),
                services
            );
        }

        if(auto projectileFactory = services.locateService<ProjectileFactory>(Services::PROJECTILEFACTORY).lock()) {
            FactoryHelpers::populateProjectileFactory(
                gameConfig.getProjectileTemplatePath(),
                std::weak_ptr<ProjectileFactory>(projectileFactory),
                services
            );
        }

        if(auto weaponTable = services.locateService<WeaponTable>(Services::WEAPONTABLE).lock()) {
            FactoryHelpers::populateWeaponTable(
                gameConfig.getWeaponTemplatePath(),
                std::weak_ptr<WeaponTable>(weaponTable),
                services
            );
        }
    }

    void Client::loop() {
        sf::Clock clock;
        sf::Event event;
        bool quit = false;

        const float dt = 1.0f/60.0f;
        float totalRuntime = 0.0f;
        float speedMultiplier = 1.0f;

        sf::Time currentTime = clock.getElapsedTime();
        float accumulator = 0.0f;

        auto guiService = services.locateService<GuiService>(Services::GUISERVICE).lock();

        gcn::Gui & gui = guiService->getGui();

        while(!quit) {

            //
            // Logic
            //

            sf::Time newTime = clock.getElapsedTime();
            float frameTime = newTime.asSeconds() - currentTime.asSeconds();
            currentTime = newTime;
            accumulator += frameTime;
            // float fps = 1.0f / frameTime;

            while(accumulator >= dt) {
                while(window.pollEvent(event)) {
                    if(event.type == sf::Event::Closed) {
                        quit = true;
                    }

                    if(event.type == sf::Event::KeyPressed) {
                        controller.handleEvent(event);
                    }

                    if(guiService) {
                        guiService->processEvent(event);
                    }

                    if(gui.getTop()) {
                        gui.logic();    
                    }
                }

                controller.update(dt * speedMultiplier);
                accumulator -= dt;
                totalRuntime += dt;
            }

            //
            // Rendering
            //
            
            window.clear(sf::Color::Blue);
            screenBuffer.clear(sf::Color::Magenta);
            controller.render(screenBuffer);
            window.setView(screenBufferView);

            // if(showFPS) {
            //     guiFpsLabel->setCaption(StringUtils::toString(fps));
            //     guiFpsLabel->adjustSize();
            // }

            if(gui.getTop()) {
                gui.draw();
            }

            // console.render(screenBuffer);

            screenBuffer.display();

            sf::Sprite renderSprite(screenBuffer.getTexture());

            window.draw(renderSprite);
            window.display();
        }

        HIKARI_LOG(debug) << "Quitting; total run time = " << totalRuntime << " seconds.";
    }

    int Client::run() {
        initWindow();
        initServices();
        initGame();

        loop();

        return 0;
    }
 
} // hikari
