#include "hikari/client/Client.hpp"
#include "hikari/core/Hikari.hpp"
#include "hikari/core/util/PhysFS.hpp"
#include "hikari/core/game/GameController.hpp"

#include "hikari/client/ClientConfig.hpp"
#include "hikari/client/game/GameProgress.hpp"
#include "hikari/client/game/GamePlayState.hpp"
#include "hikari/client/game/StageSelectState.hpp"
#include "hikari/client/game/MapTestState.hpp"
#include "hikari/client/game/GuiTestState.hpp"
#include "hikari/client/game/PasswordState.hpp"
#include "hikari/client/game/SpriteTestState.hpp"
#include "hikari/client/game/TitleState.hpp"
#include "hikari/client/audio/AudioService.hpp"
#include "hikari/client/gui/GuiService.hpp"
#include "hikari/client/gui/CommandConsole.hpp"
#include "hikari/client/scripting/SquirrelService.hpp"
#include "hikari/client/scripting/AudioServiceScriptProxy.hpp"
#include "hikari/client/Services.hpp"
#include "hikari/client/game/objects/GameObject.hpp"
#include "hikari/client/game/objects/CollectableItem.hpp"
#include "hikari/client/game/Effect.hpp"
#include "hikari/client/game/Weapon.hpp"
#include "hikari/client/game/WeaponAction.hpp"
#include "hikari/client/game/SpawnProjectileWeaponAction.hpp"
#include "hikari/client/game/WeaponTable.hpp"
#include "hikari/client/game/objects/effects/NothingEffect.hpp"
#include "hikari/client/game/objects/effects/ScriptedEffect.hpp"
#include "hikari/client/game/objects/ItemFactory.hpp"
#include "hikari/client/game/objects/EnemyFactory.hpp"
#include "hikari/client/game/objects/ProjectileFactory.hpp"
#include "hikari/client/game/objects/FactoryHelpers.hpp"

#include "hikari/core/util/FileSystem.hpp"
#include "hikari/core/util/Log.hpp"
#include "hikari/core/gui/ImageFont.hpp"
#include "hikari/core/game/map/MapLoader.hpp"
#include "hikari/core/util/StringUtils.hpp"
#include "hikari/core/util/ImageCache.hpp"
#include "hikari/core/game/AnimationLoader.hpp"
#include "hikari/core/game/map/TilesetLoader.hpp"
#include "hikari/core/util/StringUtils.hpp"
#include "hikari/core/util/TilesetCache.hpp"
#include "hikari/core/util/AnimationSetCache.hpp"
#include "hikari/core/game/map/MapLoader.hpp"
#include "hikari/client/audio/NSFSoundStream.hpp"
#include "hikari/core/geom/Rectangle2D.hpp"
#include "hikari/core/geom/BoundingBox.hpp"
#include "hikari/core/util/ServiceLocator.hpp"
#include "hikari/core/math/NESNumber.hpp"
#include "hikari/core/math/Vector2.hpp"
#include "hikari/core/util/RedirectStream.hpp"
#include "hikari/core/util/HashedString.hpp"

#include "hikari/client/CommandProcessor.hpp"

#include "hikari/core/geom/BoundingBox.hpp"
#include "hikari/test/core/geom/Vector2DTests.hpp"

#include "hikari/client/game/events/EventManager.hpp"
#include "hikari/client/game/events/EventManagerImpl.hpp"
#include "hikari/client/game/events/TransitionCollisionEventData.hpp"

#include <SFML/Graphics.hpp>

#include <guichan/sfml.hpp>
#include <guichan/gui.hpp>
#include <guichan/widgets/container.hpp>
#include <guichan/widgets/label.hpp>
#include "hikari/client/gui/Panel.hpp"

#include <json/reader.h>

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>

namespace hikari {
    Client::Client(int argc, char** argv) {
        loadConfiguration("conf.json", clientConfigJson);
        initializeServices();
        initializeGame();
    }

    int Client::run() {
        try {
            // Get things ROLLING!
        } catch(HikariException& ex) {
            HIKARI_LOG(fatal) << ex.what();
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }

    void Client::loadConfiguration(const std::string& fileName, Json::Value& value) {
        HIKARI_LOG(debug) << "Loading configuration file...";

        auto fs = FileSystem::openFile(fileName);
        Json::Reader reader;

        bool success = reader.parse(*fs, value, false);

        if(success) {
            clientConfig = ClientConfig(value);
        } else {
            HIKARI_LOG(info) << "Configuration file could not be found or was corrupt, using defaults.";
        }
    }

    void Client::initializeGame() {

    }

    void Client::initializeServices() {
        auto animationSetCache = std::make_shared<AnimationSetCache>();
        auto animationLoader   = std::make_shared<AnimationLoader>();
        auto audioService      = std::make_shared<AudioService>(gameConfigJson["assets"]["audio"]);
        auto gameProgress      = std::make_shared<GameProgress>();
        auto imageCache        = std::make_shared<ImageCache>(ImageCache::NO_SMOOTHING, ImageCache::USE_MASKING);
        auto tilesetLoader     = std::make_shared<TilesetLoader>(imageCache, animationLoader);
        auto tilesetCache      = std::make_shared<TilesetCache>(tilesetLoader);
        auto mapLoader         = std::make_shared<MapLoader>(tilesetCache);
        auto squirrelService   = std::make_shared<SquirrelService>(clientConfig.getScriptingStackSize());
        auto guiFont           = std::make_shared<ImageFont>(
            imageCache->get("assets/images/gui-font.png"),
            " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz",
            8,
            8
        );

        services.registerService(Services::AUDIO,             audioService);
        services.registerService(Services::GAMEPROGRESS,      gameProgress);
        services.registerService(Services::IMAGECACHE,        imageCache);
        services.registerService(Services::ANIMATIONSETCACHE, animationSetCache);
        services.registerService(Services::MAPLOADER,         mapLoader);
        services.registerService(Services::SCRIPTING,         squirrelService);
        services.registerService(Services::GUIFONT,           guiFont);
        // services.registerService("GUI",                       guiWrapper);
    }

} // hikari

int main(int argc, char** argv) {
    //_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

    using std::shared_ptr;
    using namespace hikari;
    
    ::hikari::Log::setReportingLevel(debug4);

    initLogging(argc, argv);

    try {
        HIKARI_LOG(info) << "Hikari engine v" << hkrHikariVersion() << " started.";

        initFileSystem(argc, argv);

        Json::Value config;
        Json::Value game;

        initConfiguration("conf.json", config);
        initGame("game.json", game);

        ClientConfig clientConfig(config);

        sf::Clock clock;

        sf::VideoMode videoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);
        sf::RenderWindow window;

        //
        // Create our window
        // We have to create the window before we load any textures since there is
        // peculiar behavior with texture on Mac.
        //
        // Another aolution is to cause `ensureGlContext()` to get called.
        //
        window.create(videoMode, APPLICATION_TITLE, sf::Style::Default);
        window.setActive(true);
        window.setVerticalSyncEnabled(clientConfig.isVsyncEnabled());
        window.setKeyRepeatEnabled(false);

        //
        // Create a "buffer" texture -- this allows us to apply screen-wide shader effects.
        // This also helps avoid tearing/small geometry gaps when scaling the scren up.
        //
        sf::RenderTexture screenBuffer;
        screenBuffer.create(videoMode.width, videoMode.height);

        //
        // Use a view to achieve resolution-independent rendering of the "buffer" texture.
        //
        sf::View screenBufferView;

        screenBufferView.setSize(
            static_cast<float>(videoMode.width),
            static_cast<float>(videoMode.height));

        screenBufferView.setCenter(
            static_cast<float>(videoMode.width / 2),
            static_cast<float>(videoMode.height / 2));

        // HIKARI_LOG(debug) << "Created render window...";

        bool showFPS = clientConfig.isFpsDisplayEnabled();

        auto imageCache        = std::make_shared<ImageCache>(ImageCache::NO_SMOOTHING, ImageCache::USE_MASKING);
        // HIKARI_LOG(debug) << "Created imageCache";
        auto animationSetCache = std::make_shared<AnimationSetCache>();
        // HIKARI_LOG(debug) << "Created animationSetCache";
        auto animationLoader   = std::make_shared<AnimationLoader>();
        // HIKARI_LOG(debug) << "Created animationLoader";
        auto tilesetLoader     = std::make_shared<TilesetLoader>(imageCache, animationLoader);
        // HIKARI_LOG(debug) << "Created tilesetLoader";
        auto tilesetCache      = std::make_shared<TilesetCache>(tilesetLoader);
        // HIKARI_LOG(debug) << "Created tilesetCache";
        auto mapLoader         = std::make_shared<MapLoader>(tilesetCache);
        // HIKARI_LOG(debug) << "Created mapLoader";
        auto gameProgress      = std::make_shared<GameProgress>();
        // HIKARI_LOG(debug) << "Created gameProgress";
        auto audioService      = std::make_shared<AudioService>(game["assets"]["audio"]);
        // HIKARI_LOG(debug) << "Created audioService";
        auto squirrelService   = std::make_shared<SquirrelService>(clientConfig.getScriptingStackSize());
        // HIKARI_LOG(debug) << "Created squirrelService";
        auto guiFont           = std::make_shared<ImageFont>(
            imageCache->get("assets/images/gui-font.png"),
            " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz",
            8,
            8
        );
        // HIKARI_LOG(debug) << "Created guiFont";

        auto guiService        = std::make_shared<GuiService>(game, imageCache, screenBuffer);
        
        auto itemFactory       = std::make_shared<ItemFactory>(animationSetCache, imageCache, squirrelService);
        auto enemyFactory      = std::make_shared<EnemyFactory>(animationSetCache, imageCache, squirrelService);
        auto projectileFactory = std::make_shared<ProjectileFactory>(animationSetCache, imageCache, squirrelService);
        auto weaponTable       = std::make_shared<WeaponTable>();
        
        // HIKARI_LOG(debug) << "Created itemFactory";

        ServiceLocator services;
        services.registerService(Services::AUDIO,             audioService);
        services.registerService(Services::GAMEPROGRESS,      gameProgress);
        services.registerService(Services::IMAGECACHE,        imageCache);
        services.registerService(Services::ANIMATIONSETCACHE, animationSetCache);
        services.registerService(Services::MAPLOADER,         mapLoader);
        services.registerService(Services::SCRIPTING,         squirrelService);
        services.registerService(Services::GUIFONT,           guiFont);
        services.registerService(Services::GUISERVICE,        guiService);
        services.registerService(Services::ITEMFACTORY,       itemFactory);
        services.registerService(Services::ENEMYFACTORY,      enemyFactory);
        services.registerService(Services::PROJECTILEFACTORY, projectileFactory);
        services.registerService(Services::WEAPONTABLE,       weaponTable);

        AudioServiceScriptProxy::setWrappedService(std::weak_ptr<AudioService>(audioService));
        
        squirrelService->runScriptFile("assets/scripts/Environment.nut");
        squirrelService->runScriptFile("assets/scripts/Bootstrap.nut");

        // When this runs all of the scripts have to have been run already
        FactoryHelpers::populateCollectableItemFactory(
            "assets/templates/items.json",
            std::weak_ptr<ItemFactory>(itemFactory),
            services
        );

        FactoryHelpers::populateEnemyFactory(
            "assets/templates/enemies.json",
            std::weak_ptr<EnemyFactory>(enemyFactory),
            services
        );

        FactoryHelpers::populateProjectileFactory(
            "assets/templates/projectiles.json",
            std::weak_ptr<ProjectileFactory>(projectileFactory),
            services
        );

        loadWeapons(weaponTable);

        gui::CommandConsole console(guiFont);

        // Create and initialize the game controller and game states
        // This need to be a factory or something that can be determined
        // at runtime by user data.
        GameController controller;

        std::string mapName = game["states"]["maptest"]["mapname"].asString();
        std::string tileSet = game["states"]["maptest"]["tileset"].asString();

        auto mapTestState = std::make_shared<MapTestState>(
            "maptest",
            mapLoader,
            mapName,
            tileSet,
            imageCache,
            guiFont,
            std::weak_ptr<ItemFactory>(itemFactory),
            std::weak_ptr<EnemyFactory>(enemyFactory),
            services
        );

        mapTestState->setRenderWindow(&window);

        //StatePtr guiTestState(new GuiTestState("guitest", guiFont));
        StatePtr spriteTestState(
            new SpriteTestState(
                "spritetest", 
                services.locateService<AnimationSetCache>(Services::ANIMATIONSETCACHE),
                services.locateService<ImageCache>(Services::IMAGECACHE)
            )
        );

        StatePtr stageSelectState(new StageSelectState("stageselect", game["states"]["select"], services));
        StatePtr gamePlayState(new GamePlayState("gameplay", game, services));
        StatePtr passwordState(new PasswordState("password", game, services));
        StatePtr titleState(new TitleState("title", game, services));

        controller.addState(mapTestState->getName(), mapTestState);
        //controller.addState(guiTestState->getName(), guiTestState);
        controller.addState(spriteTestState->getName(), spriteTestState);
        controller.addState(stageSelectState->getName(), stageSelectState);
        controller.addState(gamePlayState->getName(), gamePlayState);
        controller.addState(passwordState->getName(), passwordState);
        controller.addState(titleState->getName(), titleState);

        controller.setState(game.get("initial-state", "default").asString());

        std::unique_ptr<gcn::Label> guiFpsLabel(new gcn::Label());
        auto & topContainer = guiService->getHudContainer();
        topContainer.add(guiFpsLabel.get(), 8, 8);

        //
        // Register some commands
        //
        CommandProcessor cp;
        cp.registerHandler("echo", [](CommandProcessor::ArgumentList args) {
            auto begin = std::begin(args);
            auto end = std::end(args);

            for(; begin != end; begin++) {
                std::cout << *begin << "\t";
            }
        });

        cp.registerHandler("gc", [&squirrelService](CommandProcessor::ArgumentList args) {
            squirrelService->collectGarbage();
        });

        cp.registerHandler("sound", [&audioService](CommandProcessor::ArgumentList args) {
            const auto & arg0 = args.at(0);
            const auto & arg1 = args.at(1);

            if(arg0 == "music") {
                if(arg1 == "stop") {
                    audioService->stopMusic();
                } else {
                    try {
                        auto trackNumber = hikari::StringUtils::fromString<int>(arg1);
                        audioService->playMusic(trackNumber);
                    } catch(...) {
                        HIKARI_LOG(::hikari::error) << "Couldn't play music because of bad track number.";
                    }
                }
            } else if(arg0 == "sample") {
                if(arg1 == "stop") {
                    audioService->stopAllSamples();
                } else {
                    try {
                        auto sampleNumber = hikari::StringUtils::fromString<int>(arg1);
                        audioService->playSample(sampleNumber);
                    } catch(...) {
                        HIKARI_LOG(::hikari::error) << "Couldn't sample music because of bad track number.";
                    }
                }
            }
        });

        cp.registerHandler("setstate", [&controller](CommandProcessor::ArgumentList args) {
            const std::string& stateName = args.at(0);
            controller.setState(stateName);
        });

        // Game loop
        float t = 0.0f;
        const float dt = 1.0f/60.0f;
        float speedMultiplier = 1.0f;

        sf::Time currentTime = clock.getElapsedTime();
        float accumulator = 0.0f;

        bool quit = false;
        bool fullscreen = false;
        sf::Event event;

        gcn::Gui & gui = guiService->getGui();

        while(!quit) {

            sf::Time newTime = clock.getElapsedTime();
            float frameTime = newTime.asSeconds() - currentTime.asSeconds();
            float fps = 1.0f / frameTime;
            currentTime = newTime;

            accumulator += frameTime;

            while(accumulator >= dt) {

                while(window.pollEvent(event)) {
                    if(event.type == sf::Event::Closed) {
                        quit = true;
                    }

                    guiService->processEvent(event);

                    //
                    // Key presses
                    //
                    if(event.type == sf::Event::KeyPressed) {

                        if(event.key.code == sf::Keyboard::F2) {
                            sf::Image screenShotBuffer = screenBuffer.getTexture().copyToImage();
                            screenShotBuffer.saveToFile("screenshot.png");
                        }

                        if(event.key.code == sf::Keyboard::F7) {
                            speedMultiplier += 0.1f;
                        }

                        if(event.key.code == sf::Keyboard::F8) {
                            speedMultiplier -= 0.1f;
                        }

                        if(event.key.code == sf::Keyboard::Num5) {
                            Entity::enableDebug(true);
                        }

                        if(event.key.code == sf::Keyboard::Num6) {
                            Entity::enableDebug(false);
                        }

                        if(event.key.code == sf::Keyboard::F6) {
                            if(fullscreen) {
                                window.create(videoMode, APPLICATION_TITLE, sf::Style::Default);
                            } else {
                                auto modes = sf::VideoMode::getFullscreenModes();
                                auto fullScreenMode = modes.at(0);
                                window.create(fullScreenMode, APPLICATION_TITLE, sf::Style::Fullscreen);
                            }

                            fullscreen = !fullscreen;

                            window.setActive(true);
                            window.setVerticalSyncEnabled(clientConfig.isVsyncEnabled());
                            window.setKeyRepeatEnabled(false);
                        }

                        if(event.key.code == sf::Keyboard::F1) {
                            showFPS = !showFPS;
                            guiFpsLabel->setVisible(showFPS);
                        }

                        if(event.key.code == sf::Keyboard::F3) {
                            window.setKeyRepeatEnabled(!console.isOpen());
                            console.toggle();
                        }

                        if(console.isOpen()) {
                            if(event.key.code == sf::Keyboard::Return) {
                                if(console.getCommandBuffer() == "exit") {
                                    quit = true;
                                } else {
                                    try {
                                        cp.processCommand(console.getCommandBuffer());
                                    } catch(std::exception &ex) {
                                        HIKARI_LOG(debug1) << "Exception while proccessing console command: " << ex.what();
                                        console.setCommandBuffer("BAD COMMAND");
                                    }
                                }
                                console.commitCommand();
                            }
                        }
                    }

                    if(console.isOpen() && (event.type == sf::Event::TextEntered)) {
                        if((event.text.unicode < 128) && // ascii range
                            (event.text.unicode != 96) && // tilde
                            (event.text.unicode != 13) && // enter key
                            (event.text.unicode != 8)) { // backspace
                            char letter = static_cast<char>(event.text.unicode);
                            console.setCommandBuffer(console.getCommandBuffer() + letter);
                        } else if(event.text.unicode == 8) {
                            std::string oldBuffer = console.getCommandBuffer();

                            if(oldBuffer.size() > 0) {
                                oldBuffer.pop_back();
                            }

                            console.setCommandBuffer(oldBuffer);
                        }
                    }

                    if(!console.isOpen()) {
                        controller.handleEvent(event);
                    }

                    if(gui.getTop()) {
                        gui.logic();    
                    }
                }

                console.update(dt);
                controller.update(dt * speedMultiplier);
                accumulator -= dt;
                t += dt;
            }

            // Render
            window.clear(sf::Color::Blue);
            screenBuffer.clear(sf::Color::Magenta);
            controller.render(screenBuffer);
            window.setView(screenBufferView);

            if(showFPS) {
                guiFpsLabel->setCaption(StringUtils::toString(fps));
                guiFpsLabel->adjustSize();
            }

            if(gui.getTop()) {
                gui.draw();
            }

            console.render(screenBuffer);

            screenBuffer.display();

            sf::Sprite renderSprite(screenBuffer.getTexture());

            window.draw(renderSprite);
            window.display();
        }

        PhysFS::deinit();

    } catch(std::exception& ex) {
        HIKARI_LOG(fatal) << "Uncaught exception: " << ex.what();
        throw;
    }

    return EXIT_SUCCESS;
}

void initLogging(int argc, char** argv) {

}

void initFileSystem(int argc, char** argv) {
    using namespace hikari;
    HIKARI_LOG(debug) << "Initialization file system...";

    // Create virtual file system
    PhysFS::init(argv[0]);
    PhysFS::addToSearchPath(PhysFS::getBaseDir());

    // Load standard content
    try {
        PhysFS::addToSearchPath(PhysFS::getBaseDir() + PATH_CONTENT_PACKAGE);
    } catch(PhysFS::Exception & ex) {
        HIKARI_LOG(error) << "Couldn't load content package. Reason: " << ex.what();
    }

    // Load custom content (if it exists)
    try {
        PhysFS::addToSearchPath(PhysFS::getBaseDir() + PATH_CUSTOM_PACKAGE);
    } catch(PhysFS::Exception & ex) {
        HIKARI_LOG(warning) << "Couldn't load custom content package. Reason: " << ex.what();
    }

    PhysFS::setWriteDir(PhysFS::getBaseDir());

    HIKARI_LOG(debug) << "File system successfully initialized.";
}

void initConfiguration(const std::string &fileName, Json::Value &value) {
    using namespace hikari;
    HIKARI_LOG(debug) << "Loading configuration file...";

    if(FileSystem::exists(fileName)) {
        auto fs = FileSystem::openFile(fileName);
        Json::Reader reader;

        bool success = reader.parse(*fs, value, false);

        if(!success) {
            HIKARI_LOG(info) << "Configuration file could not be found or was corrupt, using defaults.";
            // Set defaults...
        }
    } else {
        HIKARI_LOG(fatal) << "Couldn't find configuration file '" << fileName << "'";
    }
}

void initGame(const std::string &fileName, Json::Value &value) {
    using namespace hikari;
    HIKARI_LOG(debug) << "Loading game file...";

    if(FileSystem::exists(fileName)) {
        auto fs = FileSystem::openFile(fileName);
        Json::Reader reader;

        HIKARI_LOG(debug) << "Opened game file...";

        bool success = reader.parse(*fs, value, false);

        if(!success) {
            HIKARI_LOG(info) << "Game file could not be found or was corrupt, uh oh!";
        } else {
            HIKARI_LOG(debug) << "Game file loaded!";
        }
    } else {
        HIKARI_LOG(fatal) << "Couldn't find game file '" << fileName << "'";
    }
}

void loadWeapons(const std::shared_ptr<hikari::WeaponTable> & weaponTable) {
    std::string fileName = "assets/weapons/weapons.json";

    using namespace hikari;

    // This should be another function
    auto parseWeaponAction = [](const Json::Value & json) -> std::shared_ptr<WeaponAction> {
        using namespace hikari; // Necessary for MSVC 2010

        const auto type           = json["type"].asString();
        const auto projectileType = json["projectileType"].asString();
        const auto direction      = json["direction"].asString();
        const auto motion         = json["motion"];

        std::shared_ptr<WeaponAction> action;

        if(type == "spawnProjectile") {
            action.reset(new SpawnProjectileWeaponAction(projectileType));

            if(!motion.isNull()) {
                const auto motionType = motion["type"].asString();

                if(type == "Linear") {
                    const auto velocity = motion["velocity"];
                }
            }
        }

        return action;
    };

    if(FileSystem::exists(fileName)) {
        if(weaponTable) {
            auto fs = FileSystem::openFile(fileName);
            Json::Reader reader;
            Json::Value root;
            bool success = reader.parse(*fs, root, false);

            if(!success) {
                HIKARI_LOG(info) << "Weapons couldn't be loaded!";
            } else {
                HIKARI_LOG(debug) << "Loading weapon definitions...";

                auto templateCount = root.size();

                if(templateCount > 0) {
                    for(decltype(templateCount) i = 0; i < templateCount; ++i) {
                        const auto & templateObject = root[i];

                        const auto name           = templateObject["name"].asString();
                        const auto projectileType = templateObject["projectileType"].asString();
                        const auto limit          = templateObject["limit"].asInt();
                        const auto usageCost      = static_cast<float>(templateObject["usageCost"].asDouble());
                        const auto usageSound     = templateObject["usageSound"].asString();
                        const auto usageActions   = templateObject["usageActions"];

                        auto weaponInstance = std::make_shared<Weapon>(name, usageCost);
                        auto weaponActions = std::vector<std::shared_ptr<WeaponAction>>();

                        auto actionCount = usageActions.size();

                        for(decltype(actionCount) actionIndex = 0; actionIndex < actionCount; ++actionIndex) {
                            weaponActions.push_back(parseWeaponAction(usageActions[actionIndex]));
                        }

                        //weaponActions.push_back(std::make_shared<WeaponAction>());

                        weaponInstance->setActions(weaponActions);
                        weaponTable->addWeapon(weaponInstance);
                    }
                }
            }
        }
    }
}
