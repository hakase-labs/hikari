#include <hikari/client/Client.hpp>
#include <hikari/core/Hikari.hpp>
#include <hikari/core/util/PhysFS.hpp>
#include <hikari/core/game/GameController.hpp>

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
#include <hikari/core/game/map/MapLoader.hpp>
#include <hikari/client/audio/NSFSoundStream.hpp>
#include <hikari/core/geom/Rectangle2D.hpp>
#include <hikari/core/geom/BoundingBox.hpp>
#include <hikari/core/util/ServiceLocator.hpp>
#include <hikari/core/math/RetroVector.hpp>
#include <hikari/core/math/Vector2.hpp>
#include <hikari/core/util/RedirectStream.hpp>
#include <hikari/core/util/TeeStream.hpp>
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
        initializeLogging();
        initializeVirtualFileSystem();
        initializeServices();

        // loadConfiguration("", config);

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

    void Client::initializeGame() {

    }

    void Client::initializeLogging() {
        // TODO: Create logging tee buffer and stuff to stdout + file

        #ifdef _DEBUG
        ::hikari::Log::setReportingLevel(debug4);
        #else
        ::hikari::Log::setReportingLevel(info);
        #endif
    }

    void Client::initializeServices() {

    }

    void Client::initializeVirtualFileSystem() {

    }

} // hikari

int main(int argc, char** argv) {
    //_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

    using std::shared_ptr;
    using namespace hikari;

    //std::freopen("stdout.txt", "w", stdout);

    ::hikari::Log::setReportingLevel(debug2);

    HIKARI_LOG(fatal) << "I totally logged this!";
    HIKARI_LOG(error) << "I totally logged this!";
    HIKARI_LOG(warning) << "I totally logged this!";
    HIKARI_LOG(info) << "I totally logged this!";
    HIKARI_LOG(debug) << "I totally logged this!";
    HIKARI_LOG(debug1) << "I totally logged this!";
    HIKARI_LOG(debug2) << "I totally logged this!";
    HIKARI_LOG(debug3) << "I totally logged this!";
    HIKARI_LOG(debug4) << "I totally logged this!";

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

        // Create and initialize the timing and video systems
        sf::Clock clock;
        sf::VideoMode videoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);
        sf::RenderWindow window;

        //window.show(false);
        bool showFPS = config.get("showfps", false).asBool();

        auto imageCache = std::make_shared<ImageCache>(ImageCache::NO_SMOOTHING, ImageCache::USE_MASKING);
        shared_ptr<AnimationLoader> animationLoader(new AnimationLoader());
        shared_ptr<TilesetLoader> tilesetLoader(new TilesetLoader(imageCache, animationLoader));
        shared_ptr<TilesetCache> tilesetCache(new TilesetCache(tilesetLoader));
        shared_ptr<MapLoader> mapLoader(new MapLoader(tilesetCache));
        shared_ptr<GameProgress> gameProgress(new GameProgress());
        shared_ptr<AudioService> audioService(
            new AudioService(game["assets"]["audio"])
        );
        auto squirrelService = std::make_shared<SquirrelService>(1024);

        squirrelService->runScriptString("::print(\"Squirrel Service works!\");");

        ServiceLocator services;
        services.registerService(Services::AUDIO, audioService);
        services.registerService(Services::GAMEPROGRESS, gameProgress);
        services.registerService(Services::IMAGECACHE, imageCache);
        services.registerService(Services::MAPLOADER, mapLoader);
        services.registerService(Services::SCRIPTING, squirrelService);

        shared_ptr<ImageFont> guiFont(
            new ImageFont(
                imageCache->get("assets/images/gui-font.png"), 
                " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", 
                8,
                8
            )
        );

        // TODO: Remove this as a shared "service", it belongs in the GUI.
        // Make the GUI a service.
        services.registerService(Services::GUIFONT, guiFont);

        CommandConsole console(guiFont);

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
        // sound.play();

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

        window.create(videoMode, APPLICATION_TITLE);
        window.setActive(true);
        window.setVerticalSyncEnabled(config.get("vsync", true).asBool());
        window.setKeyRepeatEnabled(false);

        // Game loop
        float t = 0.0f;
        const float dt = 1.0f/60.0f;
        float speedMultiplier = 1.0f;

        sf::Time currentTime = clock.getElapsedTime();
        float accumulator = 0.0f;

        bool quit = false;
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

                        if(event.key.code == sf::Keyboard::F12) {
                            speedMultiplier += 0.1f;
                        }

                        if(event.key.code == sf::Keyboard::F11) {
                            speedMultiplier -= 0.1f;
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
                                        //controller.setState(console.getCommandBuffer());
                                        cp.processCommand(console.getCommandBuffer());
                                    } catch(std::exception &ex) {
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
            window.clear(sf::Color::Magenta);
            controller.render(window);
            window.setView(window.getDefaultView());

            if(showFPS) {
                guiFont->renderText(window, "FPS:", 8, 8);
                guiFont->renderText(window, StringUtils::toString<float>(fps), 40, 8);
            }

            //guiFont->renderText(window, StringUtils::toString<float>(speedMultiplier), 200, 16);
            
            console.render(window);

            window.display();
        }

        //sound.stop();

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