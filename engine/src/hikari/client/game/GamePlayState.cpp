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
#include "hikari/client/game/objects/CollectableItem.hpp"
#include "hikari/client/game/objects/ItemFactory.hpp"
#include "hikari/client/game/objects/Enemy.hpp"
#include "hikari/client/game/objects/EnemyFactory.hpp"
#include "hikari/client/game/Effect.hpp"
#include "hikari/client/gui/EnergyGauge.hpp"
#include "hikari/client/gui/Panel.hpp"
#include "hikari/client/Services.hpp"
#include "hikari/client/audio/AudioService.hpp"
#include "hikari/client/game/events/EventManagerImpl.hpp"
#include "hikari/client/game/events/EventListenerDelegate.hpp"
#include "hikari/client/game/events/EntityDeathEventData.hpp"
#include "hikari/client/game/events/EntityStateChangeEventData.hpp"
#include "hikari/client/game/events/EventData.hpp"
#include "hikari/client/game/events/WeaponFireEventData.hpp"
#include "hikari/client/gui/GuiService.hpp"
#include "hikari/core/game/AnimationSet.hpp"
#include "hikari/core/game/AnimationLoader.hpp"
#include "hikari/core/game/TileMapCollisionResolver.hpp"
#include "hikari/core/game/map/MapLoader.hpp"
#include "hikari/core/game/map/MapRenderer.hpp"
#include "hikari/core/game/map/Room.hpp"
#include "hikari/core/game/map/RoomTransition.hpp"
#include "hikari/core/geom/GeometryUtils.hpp"
#include "hikari/core/gui/ImageFont.hpp"
#include "hikari/core/util/ImageCache.hpp"
#include "hikari/core/util/JsonUtils.hpp"
#include "hikari/core/util/FileSystem.hpp"
#include "hikari/core/util/ReferenceWrapper.hpp"
#include "hikari/core/util/ServiceLocator.hpp"
#include "hikari/core/util/StringUtils.hpp"
#include "hikari/core/util/Log.hpp"

#include <json/value.h>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>

#include <guichan/widgets/container.hpp>

#include <algorithm>
#include <memory>
#include <string>

namespace hikari {

    GamePlayState::GamePlayState(const std::string &name, const Json::Value &params, ServiceLocator &services)
        : name(name)
        , audioService(services.locateService<AudioService>(Services::AUDIO))
        , guiService(services.locateService<GuiService>(Services::GUISERVICE))
        , eventManager(new EventManagerImpl("GamePlayEvents", false))
        , gameProgress(services.locateService<GameProgress>(Services::GAMEPROGRESS))
        , guiFont(services.locateService<ImageFont>(Services::GUIFONT))
        , imageCache(services.locateService<ImageCache>(Services::IMAGECACHE))
        , userInput(new RealTimeInput())
        , scriptEnv(services.locateService<SquirrelService>(Services::SCRIPTING))
        , collisionResolver(new TileMapCollisionResolver())
        , currentMap(nullptr)
        , currentRoom(nullptr)
        , hero(nullptr)
        , mapRenderer(new MapRenderer(nullptr, nullptr))
        , subState(nullptr)
        , guiContainer(new gcn::Container())
        , guiBossEnergyGauge(new gui::EnergyGauge())
        , guiHeroEnergyGauge(new gui::EnergyGauge())
        , guiWeaponEnergyGauge(new gui::EnergyGauge())
        , guiMenuPanel(new gui::Panel())
        , maps()
        , itemSpawners()
        , deactivatedItemSpawners()
        , eventHandlerDelegates()
        , world()
        , camera(Rectangle2D<float>(0.0f, 0.0f, 256.0f, 240.0f))
        , view()
        , spawnerMarker()
        , transitionMarker()
        , leftBar(sf::Vector2f(8.0f, 240.0f))
        , drawInfamousBlackBar(false)
        , isViewingMenu(false)
        , hasReachedMidpoint(false)
        , hasReachedBossCorridor(false)
        , isHeroAlive(false)
    {
        loadAllMaps(services.locateService<MapLoader>(hikari::Services::MAPLOADER), params);

        bindEventHandlers();

        //
        // Create/configure GUI
        //
        buildGui();

        // leftBar.setFillColor(sf::Color::Black);

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
        hero->setEventManager(std::weak_ptr<EventManager>(eventManager));

        world.setPlayer(hero);

        const auto itemFactoryWeak  = services.locateService<ItemFactory>(Services::ITEMFACTORY);
        const auto enemyFactoryWeak = services.locateService<EnemyFactory>(Services::ENEMYFACTORY);
        world.setItemFactory(itemFactoryWeak);
        world.setEnemyFactory(enemyFactoryWeak);
    }

    GamePlayState::~GamePlayState() {
        HIKARI_LOG(debug) << "~GamePlayState()";

        std::for_each(
            std::begin(eventHandlerDelegates),
            std::end(eventHandlerDelegates), 
            [&](const std::pair<EventListenerDelegate, EventType> & del) {
                if(eventManager) {
                    bool removed = eventManager->removeListener(del.first, del.second);
                    HIKARI_LOG(debug) << "Removing event listener, type = " << del.second << ", succes = " << removed;
                }
            }
        );
    }

    void GamePlayState::buildGui() {
        guiContainer->setWidth(256);
        guiContainer->setHeight(240);
        guiContainer->setOpaque(false);
        guiContainer->setBackgroundColor(gcn::Color(0, 0, 0, 0));
        guiContainer->add(guiBossEnergyGauge.get(), 32, 16);
        guiContainer->add(guiHeroEnergyGauge.get(), 16, 16);
        guiContainer->add(guiWeaponEnergyGauge.get(), 8, 16);
        guiContainer->add(guiMenuPanel.get(), 0, 0);

        // The reddish energy gauge for bosses
        guiBossEnergyGauge->setMaximumValue(56.0f);
        guiBossEnergyGauge->setValue(56.0f);
        guiBossEnergyGauge->setVisible(true);
        guiBossEnergyGauge->setBackgroundColor(gcn::Color(0xe40058));
        guiBossEnergyGauge->setForegroundColor(gcn::Color(0xfc9838));

        // Mega man's energy gauge
        guiHeroEnergyGauge->setMaximumValue(56.0f);
        guiHeroEnergyGauge->setValue(56.0f);
        guiHeroEnergyGauge->setVisible(false);

        // Current weapon's energy gauge
        guiWeaponEnergyGauge->setMaximumValue(56.0f);
        guiWeaponEnergyGauge->setValue(56.0f);
        guiWeaponEnergyGauge->setVisible(true);
        guiWeaponEnergyGauge->setBackgroundColor(0x002a88);
        guiWeaponEnergyGauge->setForegroundColor(0xadadad);

        guiMenuPanel->setX(0);
        guiMenuPanel->setY(0);
        guiMenuPanel->setWidth(256);
        guiMenuPanel->setHeight(240);
        guiMenuPanel->setBaseColor(gcn::Color(0, 0, 0, 192));
        guiMenuPanel->setVisible(false);
    }

    void GamePlayState::handleEvent(sf::Event &event) {
        if((event.type == sf::Event::KeyPressed) && event.key.code == sf::Keyboard::Return) {
            isViewingMenu = !isViewingMenu;
            guiMenuPanel->setVisible(isViewingMenu);
        }

        if((event.type == sf::Event::KeyPressed) && event.key.code == sf::Keyboard::BackSpace) {
            //if(subState) {
                restartStage();
            //}
        }
    }

    void GamePlayState::render(sf::RenderTarget &target) {
        if(subState) {
            subState->render(target);
        }

        if(drawInfamousBlackBar) {
            target.draw(leftBar);
        }
    }

    bool GamePlayState::update(const float &dt) {
        userInput->update();

        if(eventManager) {
            eventManager->processEvents();
        }

        if(subState) {
            if(!isViewingMenu) {
                subState->update(dt);
            }
        }

        return false;
    }

    void GamePlayState::onEnter() {
        if(auto gui = guiService.lock()) {
            auto & topContainer = gui->getRootContainer();

            topContainer.add(guiContainer.get(), 0, 0);
        }

        Movable::setCollisionResolver(collisionResolver);
        Movable::setGravity(0.25f);

        // Determine which stage we're on and set that to the current level...
        currentMap = maps.at("map-test2.json");

        startStage();
    }

    void GamePlayState::onExit() {
        if(auto gui = guiService.lock()) {
            auto & topContainer = gui->getRootContainer();

            topContainer.remove(guiContainer.get());
        }
    }

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
            if(currentMap->getMidpointRoom() == currentRoom) {
                hasReachedMidpoint = true;
            }

            if(currentMap->getBossCorridorRoom() == currentRoom) {
                hasReachedBossCorridor = true;
            }

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

            // Change the world's "room"
            world.setCurrentRoom(currentRoom);

            // Get links to all spawners from new room
            linkSpawners(currentRoom);
            checkSpawners();

            // Let Rockman know where he is too
            hero->setRoom(currentRoom);
        }
    }

    void GamePlayState::linkSpawners(const std::shared_ptr<Room> & room) {
        if(room) {
            itemSpawners.clear();

            const auto & spawners = room->getSpawners();

            for(auto spawner = std::begin(spawners), end = std::end(spawners); spawner != end; spawner++) {
                if(*spawner) {
                    itemSpawners.emplace_back(std::weak_ptr<Spawner>(*spawner));
                }
            }
        }

        // Sort spawners by X coordinate, ascending
        std::sort(std::begin(itemSpawners), std::end(itemSpawners),
            [](const std::weak_ptr<Spawner> &a, const std::weak_ptr<Spawner> &b) -> bool {
                const auto & spawnerA = a.lock();
                const auto & spawnerB = b.lock();
                const auto aX = spawnerA->getPosition().getX();
                const auto bX = spawnerB->getPosition().getX();

                return aX < bX;
            }
        );

        std::for_each(
            std::begin(itemSpawners),
            std::end(itemSpawners),
            [](std::weak_ptr<Spawner> & s) {
                if(auto ptr = s.lock()) { 
                    ptr->setAwake(false);
                }
            }
        );
    }

    void GamePlayState::checkSpawners() {
        const auto & cameraView = camera.getView();

        //
        // This will wake any spawners that are active when they come on screen.
        // It will also put to sleep any awake spawners when they go off screen.
        //
        std::for_each(
            std::begin(itemSpawners),
            std::end(itemSpawners),
            [this, &cameraView](std::weak_ptr<Spawner> & s) {
                if(auto spawner = s.lock()) {
                    const auto & spawnerPosition = spawner->getPosition();

                    if(spawner->isActive()) {
                        if(spawner->isAwake()) {
                            if(!cameraView.contains(spawnerPosition.getX(), spawnerPosition.getY())) {
                                spawner->setAwake(false);
                                HIKARI_LOG(debug3) << "Just put spawner #" << spawner->getId() << " to bed";
                            }
                        }
                        else {
                            if(cameraView.contains(spawnerPosition.getX(), spawnerPosition.getY())) {
                                spawner->setAwake(true);
                                spawner->performAction(world);
                                HIKARI_LOG(debug3) << "Just woke up spawner #" << spawner->getId();
                            }
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

                bool directoryExists = FileSystem::exists(stagesDirectory);
                bool directoryIsDirectory = FileSystem::isDirectory(stagesDirectory);

                if(directoryExists && directoryIsDirectory) {
                    // Get file listing and load all .json files as maps
                    auto fileListing = FileSystem::getFileListing(stagesDirectory);

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
        HIKARI_LOG(debug) << "Starting stage.";

        // Hide energy gauge
        // guiHeroEnergyGauge->setVisible(false);

        //
        // Reset all spawners to their original state
        //
        if(currentMap) {
            int numRooms = currentMap->getRoomCount();

            for(int i = 0; i < numRooms; ++i) {
                auto room = currentMap->getRoom(i);

                HIKARI_LOG(debug) << "Resetting spawners for room " << i << ".";

                if(room) {
                    auto spawners = room->getSpawners();

                    std::for_each(std::begin(spawners), std::end(spawners), [](std::shared_ptr<Spawner> & spawner) {
                        if(spawner) {
                            spawner->setActive(true);
                            spawner->setAwake(false);

                            //
                            // TODO: Add logic that disabled already-collected items like magnet-beam, etc.
                            //
                        }
                    });
                }
            }

            hasReachedMidpoint = false;
            hasReachedBossCorridor = false;
            changeCurrentRoom(currentMap->getStartingRoom());
        }

        restartStage();
    }

    void GamePlayState::restartStage() {
        if(currentMap) {
            // Boss corridor has highest priority
            if(hasReachedBossCorridor) {
                changeCurrentRoom(currentMap->getBossCorridorRoom());
            } else if(hasReachedMidpoint) {
                changeCurrentRoom(currentMap->getMidpointRoom());
            } else {
                changeCurrentRoom(currentMap->getStartingRoom());
            }
        }

        changeSubState(std::unique_ptr<SubState>(new ReadySubState(*this)));
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

    void GamePlayState::renderHero(sf::RenderTarget &target) const {
        const auto& oldView = target.getDefaultView();
        auto newView = camera.getPixelAlignedView();
        target.setView(newView);

        // Render hero last so he'll be on "top"
        hero->render(target);

        // Restore UI view
        target.setView(oldView);
    }

    void GamePlayState::renderEntities(sf::RenderTarget &target) const {
        const auto& oldView = target.getDefaultView();
        auto newView = camera.getPixelAlignedView();
        target.setView(newView);

        // Render the entities here...
        const auto & activeItems = world.getActiveItems();

        std::for_each(
            std::begin(activeItems), 
            std::end(activeItems), 
            std::bind(&CollectableItem::render, std::placeholders::_1, ReferenceWrapper<sf::RenderTarget>(target)));

        const auto & activeEnemies = world.getActiveEnemies();

        std::for_each(
            std::begin(activeEnemies), 
            std::end(activeEnemies), 
            std::bind(&Enemy::render, std::placeholders::_1, ReferenceWrapper<sf::RenderTarget>(target)));

        // Restore UI view
        target.setView(oldView);
    }

    void GamePlayState::renderHud(sf::RenderTarget &target) const {

    }

    void GamePlayState::bindEventHandlers() {
        if(eventManager) {
            // TODO: Change this to a member handler
            auto weaponFireDelegate = fastdelegate::MakeDelegate(this, &GamePlayState::handleWeaponFireEvent);
            eventManager->addListener(weaponFireDelegate, WeaponFireEventData::Type);
            eventHandlerDelegates.push_back(std::make_pair(weaponFireDelegate, WeaponFireEventData::Type));

            auto entityDeathDelegate = fastdelegate::MakeDelegate(this, &GamePlayState::handleEntityDeathEvent);
            eventManager->addListener(entityDeathDelegate, EntityDeathEventData::Type);
            eventHandlerDelegates.push_back(std::make_pair(entityDeathDelegate, EntityDeathEventData::Type));

            auto entityStateChangeDelegate = fastdelegate::MakeDelegate(this, &GamePlayState::handleEntityStateChangeEvent);
            eventManager->addListener(entityStateChangeDelegate, EntityStateChangeEventData::Type);
            eventHandlerDelegates.push_back(std::make_pair(entityStateChangeDelegate, EntityStateChangeEventData::Type));
        }
    }

    void GamePlayState::handleEntityDeathEvent(EventDataPtr evt) {
        auto eventData = std::static_pointer_cast<EntityDeathEventData>(evt);

        if(eventData->getEntityId() == hero->getId()) {
            if(isHeroAlive) {
                isHeroAlive = false;

                //hero->setActive(false);
                //hero->setPosition(0.0f, 0.0f);
                
                HIKARI_LOG(debug) << "Hero died. Starting over.";

                if(auto sound = audioService.lock()) {
                    sound->stopMusic();
                    sound->stopAllSamples();
                    sound->playSample(23); // SAMPLE_HERO_DEATH
                }
            }
        }
    }

    void GamePlayState::handleWeaponFireEvent(EventDataPtr evt) {
        auto eventData = std::static_pointer_cast<WeaponFireEventData>(evt);
        HIKARI_LOG(debug) << "Member Weapon Fired! wid=" << eventData->getWeaponId() << ", sid=" << eventData->getShooterId(); 
    }

    void GamePlayState::handleEntityStateChangeEvent(EventDataPtr evt) {
        auto eventData = std::static_pointer_cast<EntityStateChangeEventData>(evt);

        if(eventData->getEntityId() == hero->getId()) {
            if(eventData->getStateName() == "landed") {
                if(auto sound = audioService.lock()) {
                    sound->playSample(19);
                }
            } else if(eventData->getStateName() == "teleporting") {
                if(auto sound = audioService.lock()) {
                    sound->playSample(52);
                }
            }
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
        fadeOverlay.setSize(
            sf::Vector2f(gamePlayState.camera.getView().getWidth(), gamePlayState.camera.getView().getHeight()));

        fadeOverlay.setPosition(0.0f, 0.0f);
        fadeOverlay.setFillColor(sf::Color::Black);
    }

    GamePlayState::ReadySubState::~ReadySubState() {

    }

    void GamePlayState::ReadySubState::enter() {
        timer = 0.0f;

        renderFadeOverlay = true;
        renderReadyText = false;
        gamePlayState.guiHeroEnergyGauge->setVisible(false);

        sf::Color overlayColor = sf::Color(fadeOverlay.getFillColor());
        overlayColor.a = 255;

        fadeOverlay.setFillColor(overlayColor);

        if(auto sound = gamePlayState.audioService.lock()) {
            // TODO: Obtain the correct MusicId for the level and play that.
            HIKARI_LOG(debug) << "Playing music for the level!";
            sound->playMusic(3);
        }

        // gamePlayState.isHeroAlive = true;

        if(gamePlayState.currentRoom) {
            Point2D<int> spawnPosition = gamePlayState.currentRoom->getHeroSpawnPosition();
            spawnPosition.setX(spawnPosition.getX() + gamePlayState.currentRoom->getBounds().getX());
            spawnPosition.setY(spawnPosition.getY() + gamePlayState.currentRoom->getBounds().getY());
            
            gamePlayState.camera.lookAt(
                static_cast<float>(spawnPosition.getX()), 
                static_cast<float>(spawnPosition.getY())
            );

            auto& renderer = gamePlayState.mapRenderer;

            const auto& cameraView  = gamePlayState.camera.getView();
            const auto cameraX      = static_cast<int>(cameraView.getX());
            const auto cameraWidth  = static_cast<int>(cameraView.getWidth());
            const auto cameraY      = static_cast<int>(cameraView.getY());
            const auto cameraHeight = static_cast<int>(cameraView.getHeight());

            renderer->setCullRegion(Rectangle2D<int>(cameraX, cameraY, cameraWidth, cameraHeight));
        }
    }

    void GamePlayState::ReadySubState::exit() {

    }

    void GamePlayState::ReadySubState::update(const float & dt) {
        const float frameMs = (1.0f/60.0f);

        timer += dt;

        if(timer >= (0.0f * frameMs)) {
            sf::Color overlayColor = sf::Color(fadeOverlay.getFillColor());
            overlayColor.a = 255 - 255 / 4;

            fadeOverlay.setFillColor(overlayColor);
        }

        if(timer >= (5.0f * frameMs)) {
            sf::Color overlayColor = sf::Color(fadeOverlay.getFillColor());
            overlayColor.a = 255 - 255 / 2;

            fadeOverlay.setFillColor(overlayColor);
        }

        if(timer >= (9.0f * frameMs)) {
            sf::Color overlayColor = sf::Color(fadeOverlay.getFillColor());
            overlayColor.a = 255 - ((255 / 4) + (255 / 2));

            fadeOverlay.setFillColor(overlayColor);
        }

        if(timer >= (13.0f * frameMs)) {
            renderFadeOverlay = false;
        }

        if(timer >= (24.0f * frameMs)) {
            renderReadyText = true;
        }

        if(timer >= ((24.0f + 15.0f) * frameMs)) {
            renderReadyText = false;
        }

        if(timer >= ((24.0f + 15.0f + 15.0f) * frameMs)) {
            renderReadyText = true;
        }

        if(timer >= ((24.0f + 15.0f + 15.0f + 15.0f) * frameMs)) {
            renderReadyText = false;
        }

        if(timer >= ((24.0f + 15.0f + 15.0f + 15.0f + 3.0f) * frameMs)) {
            renderReadyText = false;
        }

        // The "READY" sequence is 76 frames long, ~1.2666 seconds.
        if(timer >= (76.0f * frameMs)) {
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
        , startingPoint()
        , targetPoint()
    {

    }

    GamePlayState::TeleportSubState::~TeleportSubState() {

    }

    void GamePlayState::TeleportSubState::enter() {
        auto& hero = gamePlayState.hero;
        auto& currentRoom = gamePlayState.currentRoom;

        // gamePlayState.isHeroAlive = true;
        gamePlayState.guiHeroEnergyGauge->setVisible(true);

        if(currentRoom) {
            Point2D<int> spawnPosition = gamePlayState.currentRoom->getHeroSpawnPosition();
            spawnPosition.setX(spawnPosition.getX() + gamePlayState.currentRoom->getBounds().getX());
            spawnPosition.setY(spawnPosition.getY() + gamePlayState.currentRoom->getBounds().getY());

            targetPoint
                .setX(static_cast<float>(spawnPosition.getX()))
                .setY(static_cast<float>(spawnPosition.getY()));

            auto heroPosition = hero->getPosition();

            // Move hero to correct X coordinate
            hero->setPosition(targetPoint.getX(), heroPosition.getY());

            heroPosition = hero->getPosition();

            // Move camera so we can calculate correct Y offset
            Camera & camera = gamePlayState.camera;
            camera.lookAt(targetPoint.getX(), targetPoint.getY());

            float topOfCameraY = camera.getView().getTop();

            // Move hero to correct Y coordinate
            hero->setPosition(heroPosition.getX(), topOfCameraY - hero->getBoundingBox().getHeight());
            hero->performTeleport();
        }
    }

    void GamePlayState::TeleportSubState::exit() {

    }

    void GamePlayState::TeleportSubState::update(const float & dt) {
        auto& hero = gamePlayState.hero;
        auto& heroPosition = hero->getPosition();

        if(heroPosition.getY() < targetPoint.getY()) {
            float deltaY = std::min(16.0f, std::abs(targetPoint.getY() - heroPosition.getY()));
            hero->setPosition(heroPosition.getX(), heroPosition.getY() + deltaY);
        } else {
            hero->performMorph();
            gamePlayState.changeSubState(std::unique_ptr<SubState>(new PlayingSubState(gamePlayState)));
        }
    }

    void GamePlayState::TeleportSubState::render(sf::RenderTarget &target) {
        gamePlayState.renderMap(target);
        gamePlayState.renderHero(target);
    }

    //
    // PlayingSubState
    //
    GamePlayState::PlayingSubState::PlayingSubState(GamePlayState & gamePlayState)
        : SubState(gamePlayState)
        , postDeathTimer(0.0f)
    {

    }

    GamePlayState::PlayingSubState::~PlayingSubState() {

    }

    void GamePlayState::PlayingSubState::enter() {
        gamePlayState.isHeroAlive = true;
        postDeathTimer = 0.0f;

        auto& camera = gamePlayState.camera;
        auto& view = camera.getView();
        auto enemy = gamePlayState.world.spawnEnemy("Telly");

        if(enemy) {
            enemy->setActive(true);
            enemy->setPosition(view.getX() + view.getWidth() / 2, view.getY() + view.getHeight() / 2);
            enemy->setRoom(gamePlayState.currentRoom);
            gamePlayState.world.queueObjectAddition(std::shared_ptr<Enemy>(std::move(enemy)));
        }
    }

    void GamePlayState::PlayingSubState::exit() {

    }

    void GamePlayState::PlayingSubState::update(const float & dt) {
        auto& camera = gamePlayState.camera;

        gamePlayState.world.update(dt);

        //
        // Update collectable items
        //
        const auto & activeItems = gamePlayState.world.getActiveItems();

        std::for_each(
            std::begin(activeItems), 
            std::end(activeItems), 
            [this, &camera, &dt](const std::shared_ptr<CollectableItem> & item) {
                item->update(dt);

                //
                // Check if we've moved off screen and remove if so
                //
                const auto & view = camera.getView();

                if(!geom::intersects(item->getBoundingBox(), view)) {
                    item->setActive(false);
                    gamePlayState.world.queueObjectRemoval(item);
                    // TODO: Handle setting the spawner to deactivated some how...
                    // gamePlayState.deactivatedItemSpawners.push_back(item);
                }

                //
                // Check if we should be consumed...
                //
                const auto & hero = gamePlayState.hero;

                if(hero->getBoundingBox().intersects(item->getBoundingBox())) {
                    const auto & effect = item->getEffect();

                    if(effect) {
                        effect->apply();
                    }

                    item->setActive(false);
                    gamePlayState.world.queueObjectRemoval(item);
                }
        });

        //
        // Update enemies
        //
        const auto & activeEnemies = gamePlayState.world.getActiveEnemies();

        std::for_each(
            std::begin(activeEnemies), 
            std::end(activeEnemies), 
            [this, &camera, &dt](const std::shared_ptr<Enemy> & enemy) {
                enemy->update(dt);
        });

        // Hero died so we need to restart
        if(!gamePlayState.isHeroAlive) {
            postDeathTimer += dt;

            // Wait 1 second after you died and then restart
            if(postDeathTimer >= 2.5f) {
                // gamePlayState.changeSubState(std::unique_ptr<SubState>(new ReadySubState(gamePlayState)));
                gamePlayState.restartStage();
                return;
            }
        } else {
            // TODO: Note to self -- this seems pretty convoluted... probably change this soon please.
            //
            // Update hero
            //
            if(gamePlayState.hero) {
                gamePlayState.hero->update(dt);
            }
        }

        //
        // Move camera to correct place
        //
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

        //
        // Check if hero has touched any transitions
        //
        auto & currentRoom = gamePlayState.currentRoom;
        auto & currentRoomTransitions = currentRoom->getTransitions();

        for(auto transitionIt = currentRoomTransitions.begin(), end = currentRoomTransitions.end(); transitionIt != end; transitionIt++) {
            const RoomTransition& transition = *transitionIt;

            int regionLeft   = ((currentRoom->getX() + transition.getX()) * 16);
            int regionTop    = ((currentRoom->getY() + transition.getY()) * 16);
            int regionWidth  = transition.getWidth() * 16;
            int regionHeight = transition.getHeight() * 16;

            BoundingBox<float> transitionBounds(
                static_cast<float>(regionLeft),
                static_cast<float>(regionTop),
                static_cast<float>(regionWidth),
                static_cast<float>(regionHeight)
            );

            if(transitionBounds.contains(hero->getBoundingBox())) {
                HIKARI_LOG(debug) << "Transitioning from room " << currentRoom->getId() << " to room " << transition.getToRegion();
                gamePlayState.changeSubState(std::unique_ptr<SubState>(new TransitionSubState(gamePlayState, transition)));
                break;
            }
        }
    }

    void GamePlayState::PlayingSubState::render(sf::RenderTarget &target) {
        gamePlayState.renderMap(target);
        gamePlayState.renderEntities(target);

        if(gamePlayState.isHeroAlive) {
            gamePlayState.renderHero(target);
        }
    
        gamePlayState.renderHud(target);
    }

    //
    // TransitionSubState
    //

    const float GamePlayState::TransitionSubState::transitionSpeedX = 4.0f / (1.0f / 60.0f);
    const float GamePlayState::TransitionSubState::transitionSpeedY = 3.0f / (1.0f / 60.0f);
    const float GamePlayState::TransitionSubState::heroTranslationSpeedX = (51.0f / 64.0f) / (1.0f / 60.0f);
    const float GamePlayState::TransitionSubState::heroTranslationSpeedY = (21.0f / 80.0f) / (1.0f / 60.0f);

    GamePlayState::TransitionSubState::TransitionSubState(GamePlayState & gamePlayState, RoomTransition transition)
        : SubState(gamePlayState)
        , transitionEndX(0.0f)
        , transitionEndY(0.0f)
        , transitionFrames(0)
        , transitionFinished(false)
        , transition(transition)
        , nextRoomCullRegion()
        , nextRoom(nullptr)
    {
        auto & camera = gamePlayState.camera;

        nextRoom = findNextRoom();

        if(nextRoom) {
            nextRoomCullRegion.setWidth(static_cast<int>(camera.getView().getWidth()));
            nextRoomCullRegion.setHeight(static_cast<int>(camera.getView().getHeight()));

            switch(transition.getDirection()) {
                case RoomTransition::DirectionUp:
                    camera.lockVertical(false);
                    transitionEndY = static_cast<float>(nextRoom->getCameraBounds().getBottom() - camera.getView().getHeight());
                    break;
                case RoomTransition::DirectionForward:
                    camera.lockHorizontal(false);
                    transitionEndX = static_cast<float>(nextRoom->getCameraBounds().getLeft());
                    break;
                case RoomTransition::DirectionDown:
                    camera.lockVertical(false);
                    transitionEndY = static_cast<float>(nextRoom->getCameraBounds().getTop());
                    break;
                case RoomTransition::DirectionBackward:
                    camera.lockHorizontal(false);
                    transitionEndX = static_cast<float>(nextRoom->getCameraBounds().getRight() - camera.getView().getWidth());
                    break;
                default:
                    break;
            }
        } else {
            // This is an error case; there was no next room to go to.
            HIKARI_LOG(error) << "Tried to transition to non-existent room #" << transition.getToRegion();
            transitionFinished = true;
        }        
    }

    std::shared_ptr<Room> GamePlayState::TransitionSubState::findNextRoom() const {
        return gamePlayState.currentMap->getRoom(transition.getToRegion());
    }

    GamePlayState::TransitionSubState::~TransitionSubState() {

    }

    void GamePlayState::TransitionSubState::enter() {
        HIKARI_LOG(debug) << "TransitionSubState::enter()";

        auto & camera = gamePlayState.camera;

        switch(transition.getDirection()) {
            case RoomTransition::DirectionUp:
                camera.lockVertical(false);
                break;
            case RoomTransition::DirectionForward:
                camera.lockHorizontal(false);
                break;
            case RoomTransition::DirectionDown:
                camera.lockVertical(false);
                break;
            case RoomTransition::DirectionBackward:
                camera.lockHorizontal(false);
                break;
            default:
                break;
        }
    }

    void GamePlayState::TransitionSubState::exit() {
        HIKARI_LOG(debug) << "TransitionSubState::exit()";
        
        auto & camera = gamePlayState.camera;

        camera.lockVertical(true);
        camera.lockHorizontal(true);

        gamePlayState.changeCurrentRoom(nextRoom);
    }

    void GamePlayState::TransitionSubState::update(const float & dt) {
        auto & camera = gamePlayState.camera;
        auto & hero = gamePlayState.hero;

        float camY = camera.getY();
        float camX = camera.getX();

        switch(transition.getDirection()) {
            case RoomTransition::DirectionUp:
                if(camY > transitionEndY) {
                    camera.move(0.0f, -transitionSpeedY * dt);
                    transitionFrames++;

                    auto heroTranslateY = -heroTranslationSpeedY * dt;
                    hero->setPosition(hero->getPosition().getX(), hero->getPosition().getY() + heroTranslateY);
                    hero->getAnimationPlayer()->update(dt);
                } else {
                    transitionFinished = true;
                }
                break;
            case RoomTransition::DirectionForward:
                camera.lockHorizontal(false);
                if(camX < transitionEndX) {
                    camera.move(transitionSpeedX * dt, 0.0f);
                    transitionFrames++;

                    auto heroTranslateX = heroTranslationSpeedX * dt;
                    hero->setPosition(hero->getPosition().getX() + heroTranslateX, hero->getPosition().getY());
                    hero->getAnimationPlayer()->update(dt);
                } else {
                    transitionFinished = true;
                }
                break;
            case RoomTransition::DirectionDown:
                camera.lockVertical(false);
                if(camY < transitionEndY) {
                    camera.move(0.0f, transitionSpeedY * dt);
                    transitionFrames++;

                    auto heroTranslateY = heroTranslationSpeedY * dt;
                    hero->setPosition(hero->getPosition().getX(), hero->getPosition().getY() + heroTranslateY);
                    hero->getAnimationPlayer()->update(dt);
                } else {
                    transitionFinished = true;
                }
                break;
            case RoomTransition::DirectionBackward:
                camera.lockHorizontal(false);
                if(camX > transitionEndX) {
                    camera.move(-transitionSpeedX * dt, 0.0f);
                    transitionFrames++;

                    auto heroTranslateX = -heroTranslationSpeedX * dt;
                    hero->setPosition(hero->getPosition().getX() + heroTranslateX, hero->getPosition().getY());
                    hero->getAnimationPlayer()->update(dt);
                } else {
                    transitionFinished = true;
                }
                break;
            default:
                transitionFinished = true;
                break;
        }

        if(transitionFinished) {
            gamePlayState.changeSubState(std::unique_ptr<SubState>(new PlayingSubState(gamePlayState)));
        }
    }

    void GamePlayState::TransitionSubState::render(sf::RenderTarget &target) {
        gamePlayState.renderMap(target);

        if(nextRoom) {
            const auto & oldView = target.getDefaultView();
            const auto & cameraView = gamePlayState.camera.getView();
            const auto & newView = gamePlayState.camera.getPixelAlignedView();

            nextRoomCullRegion.setX(static_cast<int>(cameraView.getX()));
            nextRoomCullRegion.setY(static_cast<int>(cameraView.getY()));

            target.setView(newView);
            gamePlayState.mapRenderer->setRoom(nextRoom);
            gamePlayState.mapRenderer->setCullRegion(nextRoomCullRegion);
            gamePlayState.mapRenderer->render(target);
            target.setView(oldView);

            gamePlayState.mapRenderer->setRoom(gamePlayState.currentRoom);
        }

        gamePlayState.renderHero(target);
        gamePlayState.renderHud(target);
    }

} // hikari
