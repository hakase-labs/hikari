#include "hikari/client/Client.hpp"
#include "hikari/client/Services.hpp"
#include "hikari/client/audio/AudioService.hpp"
#include "hikari/client/game/GameProgress.hpp"
#include "hikari/client/game/StageSelectState.hpp"
#include "hikari/client/game/GamePlayState.hpp"
#include "hikari/client/game/GameOverState.hpp"
#include "hikari/client/game/KeyboardInput.hpp"
#include "hikari/client/game/InputService.hpp"
#include "hikari/client/game/EventBusService.hpp"
#include "hikari/client/game/events/EventBusImpl.hpp"
#include "hikari/client/game/events/EventListenerDelegate.hpp"
#include "hikari/client/game/events/GameQuitEventData.hpp"

#include "hikari/client/game/PasswordState.hpp"
#include "hikari/client/game/TitleState.hpp"
#include "hikari/client/game/OptionsState.hpp"
#include "hikari/client/game/WeaponTable.hpp"
#include "hikari/client/game/DamageTable.hpp"
#include "hikari/client/game/PaletteHelpers.hpp"
#include "hikari/client/game/objects/EnemyFactory.hpp"
#include "hikari/client/game/objects/FactoryHelpers.hpp"
#include "hikari/client/game/objects/ItemFactory.hpp"
#include "hikari/client/game/objects/ProjectileFactory.hpp"
#include "hikari/client/game/objects/ParticleFactory.hpp"
#include "hikari/client/game/objects/PalettedAnimatedSprite.hpp"
#include "hikari/client/gui/GuiService.hpp"
#include "hikari/client/scripting/SquirrelService.hpp"
#include "hikari/client/scripting/AudioServiceScriptProxy.hpp"
#include "hikari/client/scripting/GameProgressScriptProxy.hpp"
#include "hikari/client/scripting/GamePlayStateScriptProxy.hpp"
#include "hikari/client/game/objects/Enemy.hpp"


#include "hikari/core/game/AnimationLoader.hpp"
#include "hikari/core/game/SliceStateTransition.hpp"
#include "hikari/core/game/map/MapLoader.hpp"
#include "hikari/core/game/map/TilesetLoader.hpp"
#include "hikari/core/util/AnimationSetCache.hpp"
#include "hikari/core/util/FileSystem.hpp"
#include "hikari/core/util/ImageCache.hpp"
#include "hikari/core/util/Log.hpp"
#include "hikari/core/util/PhysFS.hpp"
#include "hikari/core/util/TilesetCache.hpp"

#include <squirrel.h>
#include <sqrat.h>

#include <SFML/Graphics/Color.hpp>

#include <guichan/gui.hpp>

#include <json/reader.h>

namespace hikari {

    const std::string Client::APP_TITLE             = "hikari";
    const std::string Client::PATH_CONTENT          = "content.zip";
    const std::string Client::PATH_CUSTOM_CONTENT   = "custom.zip";
    const std::string Client::PATH_CONFIG_FILE      = "conf.json";
    const std::string Client::PATH_GAME_CONFIG_FILE = "game.json";
    const std::string Client::PATH_DAMAGE_FILE      = "damage.json";

    const unsigned int Client::SCREEN_WIDTH          = 256;
    const unsigned int Client::SCREEN_HEIGHT         = 240;
    const unsigned int Client::SCREEN_BITS_PER_PIXEL = 32;

    Client::Client(int argc, char** argv)
        : gameConfigJson()
        , clientConfig()
        , gameConfig()
        , services()
        , globalInput(new KeyboardInput())
        , globalEventBus(new EventBusImpl("GlobalEvents", true))
        , videoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BITS_PER_PIXEL)
        , window()
        , screenBuffer()
        , quitGame(false)
    {
        initLogging(argc, argv);
        initFileSystem(argc, argv);
        initConfig();
        initEventBus();
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
                gameConfig = std::make_shared<GameConfig>(value);
            }
        } else {
            HIKARI_LOG(fatal) << "Couldn't find game configuration file '" << PATH_GAME_CONFIG_FILE << "'";
        }
    }

    void Client::initEventBus() {
        EventListenerDelegate quitRequestDelegate([&](EventDataPtr evt) {
            HIKARI_LOG(info) << "Quit requested!";
            quitGame = true;
        });

        globalEventBus->addListener(quitRequestDelegate, GameQuitEventData::Type);
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
        loadPalettes();
        loadScriptingEnvironment();
        loadObjectTemplates();
        loadDamageTable();

        auto gamePlayState = std::make_shared<GamePlayState>("gameplay", controller, gameConfigJson, gameConfig, services);
        GamePlayStateScriptProxy::setWrappedService(gamePlayState);

        // Create controller and game states
        StatePtr stageSelectState(new StageSelectState("stageselect", gameConfigJson["states"]["select"], controller, services));
        StatePtr gameOverState(new GameOverState("gameover", gameConfigJson, controller, services));
        StatePtr passwordState(new PasswordState("password", gameConfigJson, controller, services));
        StatePtr titleState(new TitleState("title", gameConfigJson, controller, services));
        StatePtr optionsState(new OptionsState("options", gameConfigJson, controller, services));

        controller.addState(stageSelectState->getName(), stageSelectState);
        controller.addState(gamePlayState->getName(), gamePlayState);
        controller.addState(gameOverState->getName(), gameOverState);
        controller.addState(passwordState->getName(), passwordState);
        controller.addState(titleState->getName(), titleState);
        controller.addState(optionsState->getName(), optionsState);

        controller.setState(gameConfig->getInitialState());
    }

    void Client::initLogging(int argc, char** argv) {
        // #ifdef DEBUG
        ::hikari::Log::setReportingLevel(debug4);
        // #else
        //::hikari::Log::setReportingLevel(warning);
        // #endif
    }

    void Client::initServices() {
        auto imageCache        = std::make_shared<ImageCache>(ImageCache::NO_SMOOTHING, ImageCache::USE_MASKING);
        auto animationLoader   = std::make_shared<AnimationLoader>(std::weak_ptr<ImageCache>(imageCache));
        auto animationSetCache = std::make_shared<AnimationSetCache>(animationLoader);
        auto tilesetLoader     = std::make_shared<TilesetLoader>(imageCache, animationLoader);
        auto tilesetCache      = std::make_shared<TilesetCache>(tilesetLoader);
        auto mapLoader         = std::make_shared<MapLoader>(animationSetCache, imageCache, tilesetCache);
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
        auto inputService      = std::make_shared<InputService>(globalInput);
        auto eventBusService   = std::make_shared<EventBusService>(globalEventBus);

        gameProgress->setEventBus(globalEventBus);

        // audioService->setSampleVolume(clientConfig.getSampleVolume());
        // audioService->setMusicVolume(clientConfig.getMusicVolume());

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
        services.registerService(Services::INPUT,             inputService);
        services.registerService(Services::EVENTBUS,          eventBusService);

        AnimationLoader::setImageCache(std::weak_ptr<ImageCache>(imageCache));

        // Script wrappers/proxy classes
        AudioServiceScriptProxy::setWrappedService(std::weak_ptr<AudioService>(audioService));
        GameProgressScriptProxy::setWrappedService(std::weak_ptr<GameProgress>(gameProgress));
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

        SliceStateTransition::createSharedTextures();
    }

    void Client::deinitFileSystem() {
        PhysFS::deinit();
    }

    void Client::loadPalettes() {
        PalettedAnimatedSprite::setShaderFile("assets/shaders/palette.frag");
        PalettedAnimatedSprite::createColorTable(
            PaletteHelpers::loadPaletteFile("assets/palettes.json"));
    }

    void Client::loadScriptingEnvironment() {
        if(auto squirrelService = services.locateService<SquirrelService>(Services::SCRIPTING).lock()) {
            const std::vector<std::string> & startupScripts = gameConfig->getStartUpScripts();

            std::for_each(std::begin(startupScripts), std::end(startupScripts), [&](const std::string & scriptPath) {
                std::cout << "SCRIPT IS GOING TO BE Running " << scriptPath << std::endl;
                squirrelService->runScriptFile(scriptPath);
                std::cout << "SCRIPT IS DONE Running " << scriptPath << std::endl;
            });
        }
    }

    void Client::loadObjectTemplates() {
        // if(auto itemFactory = services.locateService<ItemFactory>(Services::ITEMFACTORY).lock()) {
        //     FactoryHelpers::populateCollectableItemFactory(
        //         gameConfig->getItemTemplatePath(),
        //         std::weak_ptr<ItemFactory>(itemFactory),
        //         services
        //     );
        // }

        if(auto enemyFactory = services.locateService<EnemyFactory>(Services::ENEMYFACTORY).lock()) {
            FactoryHelpers::populateEnemyFactory(
                gameConfig->getEnemyTemplatePath(),
                std::weak_ptr<EnemyFactory>(enemyFactory),
                services
            );
        }

            auto vm = services.locateService<SquirrelService>(Services::SCRIPTING).lock()->getVmInstance();
        Sqrat::Script clientScript(vm);
        clientScript.CompileString(
            "class DerivedClient extends EnemyBehavior {"
            "  updateFunc = null;"
            "\n"
            "  constructor() {\n"
            "    base.constructor();\n"
            "  }\n"
            "\n"
            "  function attachHost(host, config = {}) {\n"
            "    base.attachHost(host, config);\n"
            "    ::print(\"Called the right attachHost!\");\n"
            "    updateFunc = _printHP;\n"
            "  }\n"
            "\n"
            "  function update(dt) {\n"
            "    if(updateFunc) {\n"
            "      updateFunc();\n"
            "    }\n"
            "    base.update(dt);\n"
            "  }\n"
            "  function _printHP() {\n"
            "    ::print(host.hitPoints);\n"
            "    ::print(host.weaponId);\n"
            "  }\n"
            "}"
        );
        clientScript.Run();

        Enemy * dynamicEnemy = new Enemy();
        dynamicEnemy->setHitPoints(55);

        Sqrat::PushVar(vm, Sqrat::RootTable(vm).GetSlot("DerivedClient"));
        sq_createinstance(vm, -1);

        Sqrat::Table configTable(vm);

        Sqrat::Object instance = Sqrat::Var<Sqrat::Object>(vm, -1).value;
        Sqrat::Function(instance, "constructor").Execute();
        sq_pop(vm, 2);
        
        Sqrat::Function attachProxy;
        attachProxy = Sqrat::Function(instance, "attachHost");
        //Sqrat::Function attachFunction2 = attachFunction;
        if(!attachProxy.IsNull()) {
            attachProxy.Execute(dynamicEnemy, configTable);
        }

        if(Sqrat::Error::Instance().Occurred(vm)) {
            std::cout << Sqrat::Error::Instance().Message(vm) << std::endl;
        } else {
            std::cout << "Everything went fine again!" << std::endl;
        }

        Sqrat::Function updateProxy;
        updateProxy = Sqrat::Function(instance, "update");

        if(!updateProxy.IsNull()) {
            updateProxy.Execute(3.1415f);
        }

        if(Sqrat::Error::Instance().Occurred(vm)) {
            std::cout << Sqrat::Error::Instance().Message(vm) << std::endl;
        } else {
            std::cout << "Everything went fine a third time!" << std::endl;
        }

        delete dynamicEnemy;

        if(auto particleFactory = services.locateService<ParticleFactory>(Services::PARTICLEFACTORY).lock()) {
            FactoryHelpers::populateParticleFactory(
                gameConfig->getParticleTemplatePath(),
                std::weak_ptr<ParticleFactory>(particleFactory),
                services
            );
        }

        if(auto projectileFactory = services.locateService<ProjectileFactory>(Services::PROJECTILEFACTORY).lock()) {
            FactoryHelpers::populateProjectileFactory(
                gameConfig->getProjectileTemplatePath(),
                std::weak_ptr<ProjectileFactory>(projectileFactory),
                services
            );
        }

        if(auto weaponTable = services.locateService<WeaponTable>(Services::WEAPONTABLE).lock()) {
            FactoryHelpers::populateWeaponTable(
                gameConfig->getWeaponTemplatePath(),
                std::weak_ptr<WeaponTable>(weaponTable),
                services
            );
        }
    }

    void Client::loadDamageTable() {
        if(auto damageTable = services.locateService<DamageTable>(Services::DAMAGETABLE).lock()) {
            auto fs = FileSystem::openFileRead(PATH_DAMAGE_FILE);

            Json::Reader reader;
            Json::Value value;

            bool success = reader.parse(*fs, value, false);

            if(success) {
                const auto & damageArray = value["damage"];

                for(unsigned int i = 0, length = damageArray.size(); i < length; i++) {
                    const auto & damageEntry = damageArray[i];
                    const float damageId = static_cast<float>(damageEntry["id"].asDouble());
                    const float damageAmount = static_cast<float>(damageEntry["amount"].asDouble());

                    damageTable->addEntry(damageId, damageAmount);
                }

                const auto & buffsArray = value["buffs"];

                for(unsigned int i = 0, length = buffsArray.size(); i < length; i++) {
                    // const auto & damageEntry = buffsArray[i];
                    // TODO: Add buffs to damage table
                }
            } else {
                HIKARI_LOG(info) << "Damage table file could not be found or was corrupt, using defaults.";
                damageTable->addEntry(0, 0.0f);
                damageTable->addEntry(1, 1.0f);
            }
        }
    }

    void Client::loop() {
        sf::Clock clock;
        sf::Event event;

        quitGame = false;

        const float dt = 1.0f/60.0f;
        float totalRuntime = 0.0f;
        float speedMultiplier = 1.0f;

        sf::Time currentTime = clock.getElapsedTime();
        float accumulator = 0.0f;

        auto guiService = services.locateService<GuiService>(Services::GUISERVICE).lock();
        auto audioService = services.locateService<AudioService>(Services::AUDIO).lock();

        gcn::Gui & gui = guiService->getGui();

        while(!quitGame) {

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
                        quitGame = true;
                    }

                    if(event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
                        // Audio tweaking code
                        if(event.key.code == sf::Keyboard::Y) {
                            audioService->setMusicVolume(audioService->getMusicVolume() + 10.0f);
                        }
                        if(event.key.code == sf::Keyboard::U) {
                            audioService->setMusicVolume(audioService->getMusicVolume() - 10.0f);
                        }
                        if(event.key.code == sf::Keyboard::H) {
                            audioService->mute();
                        }
                        if(event.key.code == sf::Keyboard::J) {
                            audioService->unmute();
                        }

                        globalInput->processEvent(event);
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
                // Update input after the game controller so you don't accidentally
                // skip an event that took place.
                globalInput->update(dt);

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

            guiService->renderHudContainer();

            screenBuffer.display();

            sf::Sprite renderSprite(screenBuffer.getTexture());

            window.draw(renderSprite);
            window.display();
        }

        PalettedAnimatedSprite::destroySharedResources();
        SliceStateTransition::destroySharedTextures();

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
