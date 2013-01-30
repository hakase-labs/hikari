#include <hikari/client/Client.hpp>
#include <hikari/core/Hikari.hpp>
#include <hikari/core/util/PhysFS.hpp>
#include <hikari/core/game/GameController.hpp>

#include <hikari/client/ClientConfig.hpp>
#include <hikari/client/game/GameProgress.hpp>
#include <hikari/client/game/GamePlayState.hpp>
#include <hikari/client/game/StageSelectState.hpp>
#include <hikari/client/game/MapTestState.hpp>
#include <hikari/client/game/GuiTestState.hpp>
#include <hikari/client/game/SpriteTestState.hpp>
#include <hikari/client/audio/AudioService.hpp> 
#include <hikari/client/gui/CommandConsole.hpp>
#include <hikari/client/scripting/SquirrelService.hpp>
#include <hikari/client/Services.hpp>

#include <hikari/core/util/FileSystem.hpp>
#include <hikari/core/util/Log.hpp>
#include <hikari/core/gui/ImageFont.hpp>
#include <hikari/core/game/map/MapLoader.hpp>
#include <hikari/core/util/StringUtils.hpp>
#include <hikari/core/util/ImageCache.hpp>
#include <hikari/core/game/AnimationLoader.hpp>
#include <hikari/core/game/map/TilesetLoader.hpp>
#include <hikari/core/util/StringUtils.hpp>
#include <hikari/core/util/TilesetCache.hpp>
#include <hikari/core/util/AnimationSetCache.hpp>
#include <hikari/core/game/map/MapLoader.hpp>
#include <hikari/client/audio/NSFSoundStream.hpp>
#include <hikari/core/geom/Rectangle2D.hpp>
#include <hikari/core/geom/BoundingBox.hpp>
#include <hikari/core/util/ServiceLocator.hpp>
#include <hikari/core/math/RetroVector.hpp>
#include <hikari/core/math/Vector2.hpp>
#include <hikari/core/util/RedirectStream.hpp>
#include <hikari/core/util/HashedString.hpp>

#include <hikari/client/CommandProcessor.hpp>

#include <hikari/core/geom/BoundingBox.hpp>
#include <hikari/test/core/geom/Vector2DTests.hpp>

#include <SFML/Graphics.hpp>

#include <json/reader.h>

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <ios>
#include <iomanip>

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
    }

} // hikari

int main(int argc, char** argv) {
    //_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

    using std::shared_ptr;
    using namespace hikari;

    ::hikari::Log::setReportingLevel(debug2);

    initLogging(argc, argv);

    try {

        CommandProcessor cp;
        cp.registerHandler("echo", [](CommandProcessor::ArgumentList args) {
            auto begin = std::begin(args);
            auto end = std::end(args);

            for(; begin != end; begin++) {
                std::cout << *begin << "\t";
            }
        });

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

        bool showFPS = clientConfig.isFpsDisplayEnabled();

        auto imageCache        = std::make_shared<ImageCache>(ImageCache::NO_SMOOTHING, ImageCache::USE_MASKING);
        auto animationSetCache = std::make_shared<AnimationSetCache>();
        auto animationLoader   = std::make_shared<AnimationLoader>();
        auto tilesetLoader     = std::make_shared<TilesetLoader>(imageCache, animationLoader);
        auto tilesetCache      = std::make_shared<TilesetCache>(tilesetLoader);
        auto mapLoader         = std::make_shared<MapLoader>(tilesetCache);
        auto gameProgress      = std::make_shared<GameProgress>();
        auto audioService      = std::make_shared<AudioService>(game["assets"]["audio"]);
        auto squirrelService   = std::make_shared<SquirrelService>(clientConfig.getScriptingStackSize());
        auto guiFont           = std::make_shared<ImageFont>(
            imageCache->get("assets/images/gui-font.png"), 
            " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", 
            8,
            8
        );

        ServiceLocator services;
        services.registerService(Services::AUDIO,             audioService);
        services.registerService(Services::GAMEPROGRESS,      gameProgress);
        services.registerService(Services::IMAGECACHE,        imageCache);
        services.registerService(Services::ANIMATIONSETCACHE, animationSetCache);
        services.registerService(Services::MAPLOADER,         mapLoader);
        services.registerService(Services::SCRIPTING,         squirrelService);
        services.registerService(Services::GUIFONT,           guiFont);

        squirrelService->runScriptFile("assets/scripts/Environment.nut");
        squirrelService->runScriptFile("assets/scripts/Bootstrap.nut");

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
            services
        );

        mapTestState->setRenderWindow(&window);
                
        StatePtr guiTestState(new GuiTestState("guitest", guiFont));
        StatePtr spriteTestState(new SpriteTestState("spritetest"));
        StatePtr stageSelectState(new StageSelectState("stageselect", game["states"]["select"], services));
        StatePtr gamePlayState(new GamePlayState("gameplay", game, services));

        controller.addState(mapTestState->getName(), mapTestState);
        controller.addState(guiTestState->getName(), guiTestState);
        controller.addState(spriteTestState->getName(), spriteTestState);
        controller.addState(stageSelectState->getName(), stageSelectState); 
        controller.addState(gamePlayState->getName(), gamePlayState);

        controller.setState(game.get("initial-state", "default").asString());

        NSFSoundStream sound(2046);
        sound.open("assets/sound/mega-man-3-nes-[NSF-ID2016].nsf"); 
        sound.setCurrentTrack(9);
        sound.stop();

        //
        // Register some commands
        //
        cp.registerHandler("sound", [&sound](CommandProcessor::ArgumentList args) {
            const std::string& arg0 = args.at(0);

            if(arg0 == "off") {
                sound.stop();
            } else if(arg0 == "on") {
                sound.play();
            } else if(arg0 == "next"){
                sound.setCurrentTrack(sound.getCurrentTrack() + 1);
            } else if(arg0 == "previous") {
                sound.setCurrentTrack(sound.getCurrentTrack() - 1);
            }
        });

        cp.registerHandler("setstate", [&controller](CommandProcessor::ArgumentList args) {
            const std::string& stateName = args.at(0);
            controller.setState(stateName);
        });

        //
        // Create our window
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

        // Game loop
        float t = 0.0f;
        const float dt = 1.0f/60.0f;
        float speedMultiplier = 1.0f;

        sf::Time currentTime = clock.getElapsedTime();
        float accumulator = 0.0f;

        bool quit = false;
        bool fullscreen = false;
        sf::Event event;

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

                    //
                    // Key presses
                    //
                    if((event.type == sf::Event::KeyPressed)) {

                        if(event.key.code == sf::Keyboard::F7) {
                            speedMultiplier += 0.1f;
                        }

                        if(event.key.code == sf::Keyboard::F8) {
                            speedMultiplier -= 0.1f;
                        }

                        if(event.key.code == sf::Keyboard::F11) {
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
                        }

                        if(event.key.code == sf::Keyboard::Tilde) {
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
            // screenBuffer.setView(window.getDefaultView());
            window.setView(screenBufferView);

            if(showFPS) {
                guiFont->renderText(screenBuffer, "FPS:", 8, 8);
                guiFont->renderText(screenBuffer, StringUtils::toString<float>(fps), 40, 8);
            }
            
            console.render(screenBuffer);

            //window.draw(screenBuffer);
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
    PhysFS::addToSearchPath(PhysFS::getBaseDir() + PATH_CONTENT_PACKAGE);
    PhysFS::setWriteDir(PhysFS::getBaseDir());

    HIKARI_LOG(debug) << "File system successfully initialized.";
}

void initConfiguration(const std::string &fileName, Json::Value &value) {
    using namespace hikari;
    HIKARI_LOG(debug) << "Loading configuration file...";

    auto fs = FileSystem::openFile(fileName);
    Json::Reader reader;

    bool success = reader.parse(*fs, value, false);

    if(!success) {
        HIKARI_LOG(info) << "Configuration file could not be found or was corrupt, using defaults.";
        // Set defaults...
    }
}

void initGame(const std::string &fileName, Json::Value &value) {
    using namespace hikari;
    HIKARI_LOG(debug) << "Loading game file...";

    auto fs = FileSystem::openFile(fileName);
    Json::Reader reader;

    bool success = reader.parse(*fs, value, false);

    if(!success) {
        HIKARI_LOG(info) << "Game file could not be found or was corrupt, uh oh!";
    }
}