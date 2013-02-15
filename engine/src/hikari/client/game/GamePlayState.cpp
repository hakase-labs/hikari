#include "hikari/client/game/GamePlayState.hpp"
#include "hikari/client/game/GameProgress.hpp"
#include "hikari/client/game/GameWorld.hpp"
#include "hikari/client/game/RealTimeInput.hpp"
#include "hikari/client/game/objects/GameObject.hpp"
#include "hikari/client/game/objects/Hero.hpp"
#include "hikari/client/game/objects/Spawner.hpp"
#include "hikari/client/game/objects/controllers/PlayerInputHeroActionController.hpp"
#include "hikari/client/scripting/SquirrelService.hpp"

#include "hikari/client/game/objects/GameObject.hpp"
#include "hikari/client/gui/EnergyMeter.hpp"
#include "hikari/client/Services.hpp"

#include "hikari/core/game/AnimationSet.hpp"
#include "hikari/core/game/AnimationLoader.hpp"
#include "hikari/core/game/TileMapCollisionResolver.hpp"
#include "hikari/core/game/map/MapLoader.hpp"
#include "hikari/core/game/map/MapRenderer.hpp"
#include "hikari/core/game/map/Room.hpp"
#include "hikari/core/game/map/RoomTransition.hpp"
#include "hikari/core/gui/ImageFont.hpp"
#include "hikari/core/util/ImageCache.hpp"
#include "hikari/core/util/JsonUtils.hpp"
#include "hikari/core/util/PhysFS.hpp"
#include "hikari/core/util/PhysFSUtils.hpp"
#include "hikari/core/util/ServiceLocator.hpp"
#include "hikari/core/util/StringUtils.hpp"

#include "hikari/core/util/Log.hpp"

#include <json/value.h>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>

#include <algorithm>
#include <memory>
#include <string>

namespace hikari {

    using gui::EnergyMeter;

    GamePlayState::GamePlayState(const std::string &name, const Json::Value &params, ServiceLocator &services)
        : name(name)
        , gameProgress(services.locateService<GameProgress>(Services::GAMEPROGRESS))
        , guiFont(services.locateService<ImageFont>(Services::GUIFONT))
        , imageCache(services.locateService<ImageCache>(Services::IMAGECACHE))
        , userInput(new RealTimeInput())
        , scriptEnv(services.locateService<SquirrelService>(Services::SCRIPTING))
        , collisionResolver(new TileMapCollisionResolver())
        , currentMap(nullptr)
        , currentRoom(nullptr)
        , hero(nullptr)
        , hudBossEnergyMeter(nullptr)
        , hudHeroEnergyMeter(nullptr)
        , hudCurrentWeaponMeter(nullptr)
        , mapRenderer(new MapRenderer(nullptr, nullptr))
        , subState(nullptr)
        , maps()
        , activeSpawners()
        , inactiveSpawners()
        , world()
        , camera(Rectangle2D<float>(0.0f, 0.0f, 256.0f, 240.0f))
        , view()
        , spawnerMarker()
        , leftBar(sf::Vector2f(8.0f, 240.0f))
        , drawBossEnergyMeter(true)
        , drawHeroEnergyMeter(false)
        , drawWeaponEnergyMeter(false)
        , drawInfamousBlackBar(false)
        , isViewingMenu(false)
    {
        loadAllMaps(services.locateService<MapLoader>(hikari::Services::MAPLOADER), params);

        //
        // Create/configure GUI
        //
        auto energyMeterTexture = imageCache->get("assets/images/meter-overlay.png");

        sf::Sprite energyMeterSprite(*energyMeterTexture);
        energyMeterSprite.setTextureRect(sf::IntRect(0, 0, 8, 56));

        hudBossEnergyMeter = std::make_shared<EnergyMeter>(energyMeterSprite, 56.0f);
        hudBossEnergyMeter->setMaximumValue(56.0f);
        hudBossEnergyMeter->setValue(46.0f);
        hudBossEnergyMeter->setFillColor(sf::Color(21, 95, 217));
        hudBossEnergyMeter->setPrimaryColor(sf::Color(100, 176, 255));
        hudBossEnergyMeter->setSecondaryColor(sf::Color(255, 255, 255));
        hudBossEnergyMeter->setPosition(sf::Vector2i(40, 25));

        hudHeroEnergyMeter = std::make_shared<EnergyMeter>(energyMeterSprite, 56.0f);
        hudHeroEnergyMeter->setMaximumValue(56.0f);
        hudHeroEnergyMeter->setValue(56.0f);
        hudHeroEnergyMeter->setPosition(sf::Vector2i(24, 25));

        hudCurrentWeaponMeter = std::make_shared<EnergyMeter>(energyMeterSprite, 56.0f);
        hudCurrentWeaponMeter->setMaximumValue(56.0f);
        hudCurrentWeaponMeter->setValue(56.0f);
        hudCurrentWeaponMeter->setPosition(sf::Vector2i(16, 25));

        leftBar.setFillColor(sf::Color::Black);

        //
        // Create/configure Rockman
        //
        auto heroId = GameObject::generateObjectId();
        auto heroAnimationSet = AnimationLoader::loadSet("assets/animations/rockman-32.json");
        auto heroSpriteSheet = imageCache->get(heroAnimationSet->getImageFileName());

        hero = std::make_shared<Hero>(heroId, nullptr);
        hero->setActive(true);
        hero->setAnimationSet(heroAnimationSet);
        hero->setSpriteTexture(heroSpriteSheet);
        hero->setBoundingBox(BoundingBoxF(0, 0, 16, 24).setOrigin(8, 20));
        hero->changeAnimation("idle");
        hero->setPosition(100.0f, 100.0f);
        hero->setActionController(std::make_shared<PlayerInputHeroActionController>(userInput));

        world.setPlayer(hero);

        subState.reset(new ReadySubState(*this));
        subState->enter();
    }

    void GamePlayState::handleEvent(sf::Event &event) {
        if((event.type == sf::Event::KeyPressed) && event.key.code == sf::Keyboard::Return) {
            isViewingMenu = !isViewingMenu;
        }

        if((event.type == sf::Event::KeyPressed) && event.key.code == sf::Keyboard::BackSpace) {
            if(subState) {
                changeSubState(std::unique_ptr<SubState>(new ReadySubState(*this)));
            }
        }
    }

    void GamePlayState::render(sf::RenderTarget &target) {
        target.clear(sf::Color(255, 0, 255, 255));

        if(subState) {
            subState->render(target);
        }

        if(drawInfamousBlackBar) {
            target.draw(leftBar);
        }
    }

    bool GamePlayState::update(const float &dt) {
        userInput->update();

        if(subState) {
            subState->update(dt);
        }

        return false;
    }

    void GamePlayState::onEnter() {
        startStage();

        Movable::setCollisionResolver(collisionResolver);
        Movable::setGravity(0.25f);

        // Determine which stage we're on and set that to the current level...
        currentMap = maps.at("map-test2.json");

        changeCurrentRoom(currentMap->getRoom(0));
    }

    void GamePlayState::onExit() { }

    const std::string& GamePlayState::getName() const {
        return name;
    }

    void GamePlayState::changeSubState(std::unique_ptr<SubState> && newSubState) {
        if(newSubState) {
            if(subState) {
                subState->exit();
            }
            subState = std::move(newSubState);
            subState->enter();
        }
    }

    void GamePlayState::changeCurrentRoom(const std::shared_ptr<Room>& newCurrentRoom) {
        currentRoom = newCurrentRoom;

        if(currentRoom) {
            // Make camera aware of new room boundaries
            camera.setBoundary(currentRoom->getCameraBounds());
            camera.lockVertical(true);
            camera.lockHorizontal(true);

            // Make renderer cull from the new room
            mapRenderer->setRoom(currentRoom);
            mapRenderer->setTileData(currentMap->getTileset());
            mapRenderer->setCullRegion(camera.getBoundary());

            // Make sure we detect collisions in this room
            collisionResolver->setRoom(currentRoom);

            // Get links to all spawners from new room
            linkSpawners(currentRoom);
            checkSpawners();

            // Let Rockman know where he is too
            hero->setRoom(currentRoom);
        }
    }

    void GamePlayState::linkSpawners(const std::shared_ptr<Room> & room) {
        if(room) {
            activeSpawners.clear();
            inactiveSpawners.clear();

            const auto & spawners = room->getSpawners();

            for(auto spawner = std::begin(spawners), end = std::end(spawners);
                    spawner != end; spawner++) {
                inactiveSpawners.emplace_back(std::weak_ptr<Spawner>(*spawner));
            }
        }

        // Sort spawners by X coordinate, ascending
        std::sort(std::begin(inactiveSpawners), std::end(inactiveSpawners),
            [](const std::weak_ptr<Spawner> &a, const std::weak_ptr<Spawner> &b) -> bool {
                const auto & spawnerA = a.lock();
                const auto & spawnerB = b.lock();
                const auto aX = spawnerA->getPosition().getX();
                const auto bX = spawnerB->getPosition().getX();

                return aX < bX;
            }
        );

        std::for_each(
            std::begin(inactiveSpawners),
            std::end(inactiveSpawners),
            [](std::weak_ptr<Spawner> & s) {
                if(auto ptr = s.lock()) {
                    ptr->setActive(false);
                }
            }
        );
    }

    void GamePlayState::checkSpawners() {
        const auto & cameraView = camera.getView();

        std::for_each(
            std::begin(inactiveSpawners),
            std::end(inactiveSpawners),
            [&cameraView](std::weak_ptr<Spawner> & s) {
                if(auto spawner = s.lock()) {
                    const auto & spawnerPosition = spawner->getPosition();

                    // If it's already awake see if it needs to deactivate
                    // It will need to be deactivated if:
                    //   1) It's no longer on screen
                    //   2) It's spawn has died
                    if(spawner->isActive()) {
                        if(!cameraView.contains(spawnerPosition.getX(), spawnerPosition.getY())) {
                            spawner->setActive(false);
                            HIKARI_LOG(debug3) << "Just put spawner #" << spawner->getId() << " to bed";
                        }
                        // TODO: Check for if spawn is dead (messaging?)
                        //
                    }
                    // If it's asleep, see if we need to wake it up
                    else {
                        if(cameraView.contains(spawnerPosition.getX(), spawnerPosition.getY())) {
                            spawner->setActive(true);
                            HIKARI_LOG(debug3) << "Just woke up spawner #" << spawner->getId();
                        }
                    }
                }
            }
        );
    }

    void GamePlayState::loadAllMaps(const std::weak_ptr<MapLoader> &mapLoader, const Json::Value &params) {
        if(auto mapLoaderPtr = mapLoader.lock()) {
            try {
                std::string stagesDirectory = params["assets"]["stages"].asString();

                bool directoryExists = PhysFS::exists(stagesDirectory);
                bool directoryIsDirectory = PhysFS::isDirectory(stagesDirectory);

                if(directoryExists && directoryIsDirectory) {
                    // Get file listing and load all .json files as maps
                    auto fileListing = PhysFS::getFileListing(stagesDirectory);

                    HIKARI_LOG(debug) << "Found " << fileListing.size() << " file(s) in map directory.";

                    for(auto index = std::begin(fileListing), end = std::end(fileListing); index != end; index++) {
                        const auto & fileName = (*index);
                        const auto & filePath = stagesDirectory + "/" + fileName; // TODO: Handle file paths for real

                        if(StringUtils::endsWith(filePath, ".json")) {
                            try {
                                HIKARI_LOG(debug) << "Loading map from \"" << fileName << "\"...";

                                auto mapJsonObject = JsonUtils::loadJson(filePath);
                                auto map = mapLoaderPtr->loadFromJson(mapJsonObject);

                                if(map) {
                                    maps[fileName] = map;
                                    HIKARI_LOG(debug) << "Successfully loaded map from \"" << fileName << "\".";
                                } else {
                                    HIKARI_LOG(error) << "Failed to load map from \"" << filePath << "\".";
                                }
                            } catch(std::exception &ex) {
                                HIKARI_LOG(error) << "Failed to load map from \"" << filePath << "\". Error: " << ex.what();
                            }
                        }
                    }
                } else {
                    HIKARI_LOG(error) << "Failed to load maps! Specified path doesn't exist or isn't a directory.";
                }
            } catch(std::exception& ex) {
                HIKARI_LOG(error) << "Failed to load maps! Reason: " << ex.what();
            }
        }
    }

    void GamePlayState::startStage() {
        playerBirth();
    }

    void GamePlayState::restartStage() {

    }

    void GamePlayState::endStage() {

    }

    void GamePlayState::playerBirth() {
        // Teleport Rock to starting point in current room
        // Play "teleported" sample
    }

    void GamePlayState::playerDeath() {
        // Hide hero
        // Spawn explosion energy balls in all directions
        // Stop music
        // Play death sample
    }

    void GamePlayState::checkCollisionWithTransition() { }

    void GamePlayState::renderMap(sf::RenderTarget &target) const {
        const auto& oldView = target.getDefaultView();
        auto newView = camera.getPixelAlignedView();
        target.setView(newView);

        mapRenderer->setRoom(currentRoom);
        mapRenderer->render(target);

        target.setView(oldView);
    }

    void GamePlayState::renderEntities(sf::RenderTarget &target) const {
        const auto& oldView = target.getDefaultView();
        auto newView = camera.getPixelAlignedView();
        target.setView(newView);

        // Render the entities here...
        hero->render(target);

        target.setView(oldView);
    }

    void GamePlayState::renderHud(sf::RenderTarget &target) const {
        // guiFont->renderText(target, "HUD", 72, 32);

        if(drawBossEnergyMeter) {
            hudBossEnergyMeter->render(target);
        }

        if(drawHeroEnergyMeter) {
             hudHeroEnergyMeter->render(target);
        }

        if(drawWeaponEnergyMeter) {
            hudCurrentWeaponMeter->render(target);
        }

        if(isViewingMenu) {
            //guiFont->renderText(target, "MENU", 72, 40);
        }
    }


    // ************************************************************************
    // Definition of sub-states
    // ************************************************************************

    GamePlayState::SubState::SubState(GamePlayState & gamePlayState)
        : gamePlayState(gamePlayState)
    {
    }

    //
    // ReadySubState
    //
    GamePlayState::ReadySubState::ReadySubState(GamePlayState & gamePlayState)
        : SubState(gamePlayState)
        , renderReadyText(false)
        , renderFadeOverlay(true)
        , timer(0.0f)
        , fadeOverlay()
    {
        std::cout << "ReadySubState()" << std::endl;

        fadeOverlay.setSize(
            sf::Vector2f(gamePlayState.camera.getView().getWidth(), gamePlayState.camera.getView().getHeight()));

        fadeOverlay.setPosition(0.0f, 0.0f);
        fadeOverlay.setFillColor(sf::Color::Black);
    }

    GamePlayState::ReadySubState::~ReadySubState() {
        std::cout << "~ReadySubState()" << std::endl;
    }

    void GamePlayState::ReadySubState::enter() {
        std::cout << "ReadySubState::enter()" << std::endl;
        timer = 0.0f;

        renderFadeOverlay = true;
        renderReadyText = false;

        sf::Color overlayColor = sf::Color(fadeOverlay.getFillColor());
        overlayColor.a = 0;

        fadeOverlay.setFillColor(overlayColor);
    }

    void GamePlayState::ReadySubState::exit() {
        std::cout << "ReadySubState::exit()" << std::endl;
    }

    void GamePlayState::ReadySubState::update(const float & dt) {
        timer += dt;

        if(timer >= (0.0f * (1.0f/60.0f))) {
            sf::Color overlayColor = sf::Color(fadeOverlay.getFillColor());
            overlayColor.a = 255 - 255 / 4;

            fadeOverlay.setFillColor(overlayColor);
        }

        if(timer >= (5.0f * (1.0f/60.0f))) {
            sf::Color overlayColor = sf::Color(fadeOverlay.getFillColor());
            overlayColor.a = 255 - 255 / 2;

            fadeOverlay.setFillColor(overlayColor);
        }

        if(timer >= (9.0f * (1.0f/60.0f))) {
            sf::Color overlayColor = sf::Color(fadeOverlay.getFillColor());
            overlayColor.a = 255 - ((255 / 4) + (255 / 2));

            fadeOverlay.setFillColor(overlayColor);
        }

        if(timer >= (13.0f * (1.0f/60.0f))) {
            renderFadeOverlay = false;
        }

        if(timer >= (24.0f * (1.0f / 60.0f))) {
            renderReadyText = true;
        }

        if(timer >= ((24.0f + 15.0f) * (1.0f / 60.0f))) {
            renderReadyText = false;
        }

        if(timer >= ((24.0f + 15.0f + 15.0f) * (1.0f / 60.0f))) {
            renderReadyText = true;
        }

        if(timer >= ((24.0f + 15.0f + 15.0f + 15.0f) * (1.0f / 60.0f))) {
            renderReadyText = false;
        }

        if(timer >= ((24.0f + 15.0f + 15.0f + 15.0f + 3.0f) * (1.0f / 60.0f))) {
            renderReadyText = false;
        }

        // The "READY" sequence is 76 frames long, ~1.2666 seconds.
        if(timer >= (76.0f * (1.0f/60.0f))) {
            gamePlayState.changeSubState(std::unique_ptr<SubState>(new TeleportSubState(gamePlayState)));
        }
    }

    void GamePlayState::ReadySubState::render(sf::RenderTarget &target) {
        gamePlayState.renderMap(target);

        if(renderFadeOverlay) {
            target.draw(fadeOverlay);
        }

        if(renderReadyText) {
            gamePlayState.guiFont->renderText(target, "READY", 108, 121);
        }
    }

    //
    // TeleportSubState
    //
    GamePlayState::TeleportSubState::TeleportSubState(GamePlayState & gamePlayState)
        : SubState(gamePlayState)
    {
        std::cout << "TeleportSubState()" << std::endl;
    }

    GamePlayState::TeleportSubState::~TeleportSubState() {
        std::cout << "~TeleportSubState()" << std::endl;
    }

    void GamePlayState::TeleportSubState::enter() {
        std::cout << "TeleportSubState::enter()" << std::endl;

        auto& hero = gamePlayState.hero;
        auto& currentRoom = gamePlayState.currentRoom;

        if(currentRoom) {
            auto roomPositionX = currentRoom->getX() * currentRoom->getGridSize();
            auto roomPositionY = currentRoom->getY() * currentRoom->getGridSize();

            // TODO: Get the room's playerSpawn location and use that, noob!

            hero->setPosition(roomPositionX + 128, roomPositionY);
        }
    }

    void GamePlayState::TeleportSubState::exit() {
        std::cout << "TeleportSubState::exit()" << std::endl;
    }

    void GamePlayState::TeleportSubState::update(const float & dt) {
        auto& camera = gamePlayState.camera;
        auto& hero = gamePlayState.hero;
        auto& heroPosition = hero->getPosition();

        camera.lookAt(heroPosition.getX(), heroPosition.getY());

        gamePlayState.changeSubState(std::unique_ptr<SubState>(new PlayingSubState(gamePlayState)));
    }

    void GamePlayState::TeleportSubState::render(sf::RenderTarget &target) {
        gamePlayState.renderMap(target);
    }

    //
    // PlayingSubState
    //
    GamePlayState::PlayingSubState::PlayingSubState(GamePlayState & gamePlayState)
        : SubState(gamePlayState)
    {
        std::cout << "PlayingSubState()" << std::endl;
    }

    GamePlayState::PlayingSubState::~PlayingSubState() {
        std::cout << "~PlayingSubState()" << std::endl;
    }

    void GamePlayState::PlayingSubState::enter() {
        std::cout << "PlayingSubState::enter()" << std::endl;

        gamePlayState.drawHeroEnergyMeter = true;
    }

    void GamePlayState::PlayingSubState::exit() {
        std::cout << "PlayingSubState::exit()" << std::endl;
    }

    void GamePlayState::PlayingSubState::update(const float & dt) {
        gamePlayState.world.update(dt);

        if(gamePlayState.hero) {
            gamePlayState.hero->update(dt);
        }

        auto& camera = gamePlayState.camera;
        auto& hero = gamePlayState.hero;
        const auto& heroPosition = hero->getPosition();
        auto& renderer = gamePlayState.mapRenderer;

        camera.lookAt(heroPosition.getX(), heroPosition.getY());

        const auto& cameraView = camera.getView();
        const auto cameraX  = static_cast<int>(cameraView.getX());
        const auto cameraWidth = static_cast<int>(cameraView.getWidth());
        const auto cameraY   = static_cast<int>(cameraView.getY());
        const auto cameraHeight = static_cast<int>(cameraView.getHeight());

        renderer->setCullRegion(Rectangle2D<int>(cameraX, cameraY, cameraWidth, cameraHeight));

        gamePlayState.checkSpawners();
    }

    void GamePlayState::PlayingSubState::render(sf::RenderTarget &target) {
        gamePlayState.renderMap(target);
        gamePlayState.renderEntities(target);
        gamePlayState.renderHud(target);
    }

    //
    // TransitionSubState
    //
    GamePlayState::TransitionSubState::TransitionSubState(GamePlayState & gamePlayState)
        : SubState(gamePlayState)
    {

    }

    GamePlayState::TransitionSubState::~TransitionSubState() {

    }

    void GamePlayState::TransitionSubState::enter() {

    }

    void GamePlayState::TransitionSubState::exit() {

    }

    void GamePlayState::TransitionSubState::update(const float & dt) {

    }

    void GamePlayState::TransitionSubState::render(sf::RenderTarget &target) {

    }

} // hikari
