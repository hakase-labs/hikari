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
#include "hikari/client/game/objects/Projectile.hpp"
#include "hikari/client/game/objects/ProjectileFactory.hpp"
#include "hikari/client/game/objects/Particle.hpp"
#include "hikari/client/game/objects/ParticleFactory.hpp"
#include "hikari/client/game/objects/PalettedAnimatedSprite.hpp"
#include "hikari/client/game/Effect.hpp"
#include "hikari/client/game/Weapon.hpp"
#include "hikari/client/game/Shot.hpp"
#include "hikari/client/game/WeaponTable.hpp"
#include "hikari/client/game/DamageKey.hpp"
#include "hikari/client/game/DamageTable.hpp"
#include "hikari/client/gui/EnergyGauge.hpp"
#include "hikari/client/gui/Panel.hpp"
#include "hikari/client/gui/Orientation.hpp"
#include "hikari/client/Services.hpp"
#include "hikari/client/audio/AudioService.hpp"
#include "hikari/client/game/KeyboardInput.hpp"
#include "hikari/client/game/Task.hpp"
#include "hikari/client/game/FunctionTask.hpp"
#include "hikari/client/game/events/EventBusImpl.hpp"
#include "hikari/client/game/events/EventListenerDelegate.hpp"
#include "hikari/client/game/events/EntityDamageEventData.hpp"
#include "hikari/client/game/events/DoorEventData.hpp"
#include "hikari/client/game/events/EntityDeathEventData.hpp"
#include "hikari/client/game/events/EntityStateChangeEventData.hpp"
#include "hikari/client/game/events/EventData.hpp"
#include "hikari/client/game/events/WeaponFireEventData.hpp"
#include "hikari/client/game/events/ObjectRemovedEventData.hpp"
#include "hikari/client/gui/GuiService.hpp"
#include "hikari/client/gui/Menu.hpp"
#include "hikari/client/gui/WeaponMenuItem.hpp"
#include "hikari/client/gui/Icon.hpp"
#include "hikari/core/game/GameController.hpp"
#include "hikari/core/game/AnimationSet.hpp"
#include "hikari/core/game/AnimationLoader.hpp"
#include "hikari/core/game/TileMapCollisionResolver.hpp"
#include "hikari/core/game/map/MapLoader.hpp"
#include "hikari/core/game/map/MapRenderer.hpp"
#include "hikari/core/game/map/Door.hpp"
#include "hikari/core/game/map/Room.hpp"
#include "hikari/core/game/map/RoomTransition.hpp"
#include "hikari/core/geom/GeometryUtils.hpp"
#include "hikari/core/util/ImageCache.hpp"
#include "hikari/core/util/AnimationSetCache.hpp"
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
#include <guichan/widgets/label.hpp>

#include <guichan/hakase/functoractionlistener.hpp>
#include <guichan/hakase/functorselectionlistener.hpp>

#include <sqrat.h>

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <string>

namespace hikari {

    void letMeKnowItsGone(EventDataPtr data) {
        auto eventData = std::static_pointer_cast<ObjectRemovedEventData>(data);
        HIKARI_LOG(debug1) << "Removed! id =" << eventData->getObjectId();
    }

    GamePlayState::GamePlayState(const std::string &name, GameController & controller, const Json::Value &params, ServiceLocator &services)
        : name(name)
        , controller(controller)
        , audioService(services.locateService<AudioService>(Services::AUDIO))
        , guiService(services.locateService<GuiService>(Services::GUISERVICE))
        , eventBus(new EventBusImpl("GamePlayEvents", false))
        , weaponTable(services.locateService<WeaponTable>(Services::WEAPONTABLE))
        , damageTable(services.locateService<DamageTable>(Services::DAMAGETABLE))
        , gameProgress(services.locateService<GameProgress>(Services::GAMEPROGRESS))
        , imageCache(services.locateService<ImageCache>(Services::IMAGECACHE))
        , userInput(new RealTimeInput())
        , scriptEnv(services.locateService<SquirrelService>(Services::SCRIPTING))
        , collisionResolver(new TileMapCollisionResolver())
        , currentMap(nullptr)
        , currentTileset(nullptr)
        , currentRoom(nullptr)
        , hero(nullptr)
        , mapRenderer(new MapRenderer(nullptr, nullptr))
        , subState(nullptr)
        , nextSubState(nullptr)
        , guiContainer(new gcn::Container())
        , guiBossEnergyGauge(new gui::EnergyGauge())
        , guiHeroEnergyGauge(new gui::EnergyGauge())
        , guiWeaponEnergyGauge(new gui::EnergyGauge())
        , guiMenuPanel(new gui::Panel())
        , guiWeaponMenuBackground(new gui::Icon("assets/images/bg-weapon-menu.png"))
        , guiMenuLifeEnergyGauge(new gui::EnergyGauge())
        , guiLivesLabel(new gcn::Label())
        , guiETanksLabel(new gcn::Label())
        , guiReadyLabel(new gcn::Label())
        , guiWeaponMenu(new gui::Menu())
        , guiWeaponMenuActionListener(nullptr)
        , guiWeaponMenuSelectionListener(nullptr)
        , keyboardInput(new KeyboardInput())
        , maps()
        , itemSpawners()
        , deactivatedItemSpawners()
        , eventHandlerDelegates()
        , bonusChancesTable()
        , taskQueue()
        , world()
        , camera(Rectangle2D<float>(0.0f, 0.0f, 256.0f, 240.0f))
        , view()
        , spawnerMarker()
        , transitionMarker()
        , leftBar(sf::Vector2f(8.0f, 240.0f))
        , drawInfamousBlackBar(false)
        , canViewMenu(false)
        , isViewingMenu(false)
        , hasReachedMidpoint(false)
        , hasReachedBossCorridor(false)
        , isHeroAlive(false)
        , gotoNextState(false)
        , isRestoringEnergy(false)
    {
        loadAllMaps(services.locateService<MapLoader>(hikari::Services::MAPLOADER), params);

        bindEventHandlers();

        //
        // Create/configure GUI
        //
        buildGui();

        populateBonusChancesTable();

        // leftBar.setFillColor(sf::Color::Black);

        auto animationCacheWeak = services.locateService<AnimationSetCache>(Services::ANIMATIONSETCACHE);

        if(auto animationCache = animationCacheWeak.lock()) {

            //
            // Create/configure Rockman
            //
            auto heroId = GameObject::generateObjectId();
            auto heroAnimationSet = animationCache->get("assets/animations/rockman-32.json");

            hero = std::make_shared<Hero>(heroId, nullptr);
            hero->setActive(true);
            hero->setAnimationSet(heroAnimationSet);
            hero->setBoundingBox(BoundingBoxF(0, 0, 16, 24).setOrigin(8, 20));
            hero->changeAnimation("idle");
            hero->setPosition(100.0f, 100.0f);
            hero->setActionSpot(Vector2<float>(6.0, -8.0));
            hero->setActionController(std::make_shared<PlayerInputHeroActionController>(userInput));
            hero->setEventBus(std::weak_ptr<EventBus>(eventBus));
        }

        world.setPlayer(hero);
        world.setEventBus(std::weak_ptr<EventBus>(eventBus));

        const auto itemFactoryWeak  = services.locateService<ItemFactory>(Services::ITEMFACTORY);
        const auto enemyFactoryWeak = services.locateService<EnemyFactory>(Services::ENEMYFACTORY);
        const auto particleFactoryWeak = services.locateService<ParticleFactory>(Services::PARTICLEFACTORY);
        const auto projectileFactoryWeak = services.locateService<ProjectileFactory>(Services::PROJECTILEFACTORY);
        world.setItemFactory(itemFactoryWeak);
        world.setEnemyFactory(enemyFactoryWeak);
        world.setParticleFactory(particleFactoryWeak);
        world.setProjectileFactory(projectileFactoryWeak);
    }

    GamePlayState::~GamePlayState() {
        HIKARI_LOG(debug) << "~GamePlayState()";

        std::for_each(
            std::begin(eventHandlerDelegates),
            std::end(eventHandlerDelegates),
            [&](const std::pair<EventListenerDelegate, EventType> & del) {
                if(eventBus) {
                    bool removed = eventBus->removeListener(del.first, del.second);
                    HIKARI_LOG(debug) << "Removing event listener, type = " << del.second << ", succes = " << removed;
                }
            }
        );
    }

    void GamePlayState::buildGui() {
        if(auto guiSvc = guiService.lock()) {
            guiContainer->setWidth(256);
            guiContainer->setHeight(240);
            guiContainer->setOpaque(false);
            guiContainer->setBackgroundColor(gcn::Color(0, 0, 0, 0));
            //guiContainer->add(guiBossEnergyGauge.get(), 32, 16);
            guiContainer->add(guiHeroEnergyGauge.get(), 16, 16);
            //guiContainer->add(guiWeaponEnergyGauge.get(), 8, 16);
            guiContainer->add(guiMenuPanel.get(), 0, 0);

            // The reddish energy gauge for bosses
            guiBossEnergyGauge->setMaximumValue(3.0f);
            guiBossEnergyGauge->setValue(3.0f);
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

            // "Life" energy gague on the weapon menu
            guiMenuLifeEnergyGauge->setMaximumValue(56.0f);
            guiMenuLifeEnergyGauge->setValue(56.0f);
            guiMenuLifeEnergyGauge->setVisible(true);
            guiMenuLifeEnergyGauge->setOrientation(gui::Orientation::HORIZONTAL);
            guiMenuLifeEnergyGauge->setWidth(56);
            guiMenuLifeEnergyGauge->setHeight(8);

            guiMenuPanel->setX(0);
            guiMenuPanel->setY(0);
            guiMenuPanel->setWidth(256);
            guiMenuPanel->setHeight(240);
            guiMenuPanel->setBaseColor(gcn::Color(0, 0, 0, 192));
            guiMenuPanel->setVisible(false);
            guiMenuPanel->add(guiWeaponMenuBackground.get(), 0, 0);
            guiMenuPanel->add(guiMenuLifeEnergyGauge.get(), 104, 32);
            guiMenuPanel->add(guiLivesLabel.get(), 208 + 1, 192 + 8);
            guiMenuPanel->add(guiETanksLabel.get(), 56 + 1, 192 + 8);

            guiLivesLabel->setVisible(true);
            guiETanksLabel->setVisible(true);

            guiReadyLabel->setX(108);
            guiReadyLabel->setY(121);
            guiReadyLabel->setCaption("READY");
            guiReadyLabel->setAlignment(gcn::Graphics::Left);
            guiReadyLabel->adjustSize();
            guiReadyLabel->setVisible(false);

            guiContainer->add(guiReadyLabel.get());

            if(auto weaponItemFont = guiSvc->getFontByName("weapon-menu")) {
                guiLivesLabel->setFont(weaponItemFont.get());
                guiETanksLabel->setFont(weaponItemFont.get());

                std::shared_ptr<gui::WeaponMenuItem> weapon1MenuItem(new gui::WeaponMenuItem("M.BUSTER"));
                weapon1MenuItem->setForegroundColor(gcn::Color(0, 0, 0, 0));
                weapon1MenuItem->setSelectionColor(gcn::Color(250, 128, 128));
                weapon1MenuItem->setX(0);
                weapon1MenuItem->setY(0);
                weapon1MenuItem->setFont(weaponItemFont.get());
                guiWeaponMenu->addItem(weapon1MenuItem);

                std::shared_ptr<gui::WeaponMenuItem> weapon2MenuItem(new gui::WeaponMenuItem("PEARL"));
                weapon2MenuItem->setForegroundColor(gcn::Color(0, 0, 0, 0));
                weapon2MenuItem->setSelectionColor(gcn::Color(250, 128, 128));
                weapon2MenuItem->setX(0);
                weapon2MenuItem->setY(16);
                weapon2MenuItem->setFont(weaponItemFont.get());
                guiWeaponMenu->addItem(weapon2MenuItem);

                std::shared_ptr<gui::WeaponMenuItem> weapon3MenuItem(new gui::WeaponMenuItem("Weapon 3"));
                weapon3MenuItem->setForegroundColor(gcn::Color(0, 0, 0, 0));
                weapon3MenuItem->setSelectionColor(gcn::Color(250, 128, 128));
                weapon3MenuItem->setX(0);
                weapon3MenuItem->setY(32);
                weapon3MenuItem->setFont(weaponItemFont.get());
                guiWeaponMenu->addItem(weapon3MenuItem);

                std::shared_ptr<gui::WeaponMenuItem> weapon4MenuItem(new gui::WeaponMenuItem("Weapon 4"));
                weapon4MenuItem->setForegroundColor(gcn::Color(0, 0, 0, 0));
                weapon4MenuItem->setSelectionColor(gcn::Color(250, 128, 128));
                weapon4MenuItem->setX(0);
                weapon4MenuItem->setY(48);
                weapon4MenuItem->setFont(weaponItemFont.get());
                guiWeaponMenu->addItem(weapon4MenuItem);

                std::shared_ptr<gui::WeaponMenuItem> weapon5MenuItem(new gui::WeaponMenuItem("Weapon 5"));
                weapon5MenuItem->setForegroundColor(gcn::Color(0, 0, 0, 0));
                weapon5MenuItem->setSelectionColor(gcn::Color(250, 128, 128));
                weapon5MenuItem->setX(0);
                weapon5MenuItem->setY(64);
                weapon5MenuItem->setFont(weaponItemFont.get());
                guiWeaponMenu->addItem(weapon5MenuItem);

                std::shared_ptr<gui::WeaponMenuItem> weapon6MenuItem(new gui::WeaponMenuItem("Weapon 6"));
                weapon6MenuItem->setForegroundColor(gcn::Color(0, 0, 0, 0));
                weapon6MenuItem->setSelectionColor(gcn::Color(250, 128, 128));
                weapon6MenuItem->setX(112);
                weapon6MenuItem->setY(0);
                weapon6MenuItem->setFont(weaponItemFont.get());
                guiWeaponMenu->addItem(weapon6MenuItem);

                std::shared_ptr<gui::WeaponMenuItem> weapon7MenuItem(new gui::WeaponMenuItem("Weapon 7"));
                weapon7MenuItem->setForegroundColor(gcn::Color(0, 0, 0, 0));
                weapon7MenuItem->setSelectionColor(gcn::Color(250, 128, 128));
                weapon7MenuItem->setX(112);
                weapon7MenuItem->setY(16);
                weapon7MenuItem->setFont(weaponItemFont.get());
                guiWeaponMenu->addItem(weapon7MenuItem);

                std::shared_ptr<gui::WeaponMenuItem> weapon8MenuItem(new gui::WeaponMenuItem("Weapon 8"));
                weapon8MenuItem->setForegroundColor(gcn::Color(0, 0, 0, 0));
                weapon8MenuItem->setSelectionColor(gcn::Color(250, 128, 128));
                weapon8MenuItem->setX(112);
                weapon8MenuItem->setY(32);
                weapon8MenuItem->setFont(weaponItemFont.get());
                guiWeaponMenu->addItem(weapon8MenuItem);

                std::shared_ptr<gui::WeaponMenuItem> weapon9MenuItem(new gui::WeaponMenuItem("Weapon 9"));
                weapon9MenuItem->setForegroundColor(gcn::Color(0, 0, 0, 0));
                weapon9MenuItem->setSelectionColor(gcn::Color(250, 128, 128));
                weapon9MenuItem->setX(112);
                weapon9MenuItem->setY(48);
                weapon9MenuItem->setFont(weaponItemFont.get());
                guiWeaponMenu->addItem(weapon9MenuItem);

                std::shared_ptr<gui::WeaponMenuItem> weapon10MenuItem(new gui::WeaponMenuItem("Weapon 10"));
                weapon10MenuItem->setForegroundColor(gcn::Color(0, 0, 0, 0));
                weapon10MenuItem->setSelectionColor(gcn::Color(250, 128, 128));
                weapon10MenuItem->setX(112);
                weapon10MenuItem->setY(64);
                weapon10MenuItem->setFont(weaponItemFont.get());
                guiWeaponMenu->addItem(weapon10MenuItem);
            }

            guiWeaponMenu->setWidth(guiContainer->getWidth() - 32);
            guiWeaponMenu->setHeight((guiContainer->getHeight() / 2) - 32);
            guiWeaponMenu->setBackgroundColor(gcn::Color(0, 0, 0, 0));
            guiWeaponMenu->enableWrapping();
            guiWeaponMenu->setVisible(true);

            guiMenuPanel->add(guiWeaponMenu.get(), 32, 48);

            guiWeaponMenuActionListener.reset(new gcn::FunctorActionListener([&](const gcn::ActionEvent& event) {
                auto item = guiWeaponMenu->getMenuItemAt(guiWeaponMenu->getSelectedIndex());
                // std::cout << "Actioned on #" << guiWeaponMenu->getSelectedIndex() << std::endl;
            }));

            guiWeaponMenuSelectionListener.reset(new gcn::FunctorSelectionListener([&](const gcn::SelectionEvent & event) {
                std::cout << "Selection changed! " << guiWeaponMenu->getSelectedIndex() << std::endl;

                // TODO: For now just use the index of the item in the menu.
                // auto item = guiWeaponMenu->getMenuItemAt(guiWeaponMenu->getSelectedIndex());
                auto selectedWeaponIndex = guiWeaponMenu->getSelectedIndex();

                if(auto weapons = weaponTable.lock()) {
                    std::cout << "Weapon table is good." << std::endl;
                    auto weaponWeak = weapons->getWeaponById(selectedWeaponIndex);

                    if(auto weapon = weaponWeak.lock()) {
                        int paletteId = weapon->getPaletteId();

                        std::cout << "Weapon pointer is good, paletteId = " << paletteId << std::endl;

                        if(paletteId == -1) {
                            paletteId = 3; // Fix the index so it defaults to the blue palette
                        }

                        PalettedAnimatedSprite::setSharedPaletteIndex(paletteId);
                    }
                }
                // if(auto audio = audioService.lock()) {
                //     audio->playSample(27);
                // }
            }));

            guiWeaponMenu->setEnabled(true);
            guiWeaponMenu->addActionListener(guiWeaponMenuActionListener.get());
            guiWeaponMenu->addSelectionListener(guiWeaponMenuSelectionListener.get());
            guiWeaponMenu->setSelectedIndex(0);

            guiContainer->setEnabled(true);
            guiMenuPanel->setEnabled(true);
        }
    }

    void GamePlayState::handleEvent(sf::Event &event) {
        if((event.type == sf::Event::KeyPressed) && event.key.code == sf::Keyboard::Return) {
            if(canViewMenu) {
                isViewingMenu = !isViewingMenu;
            }
            guiMenuPanel->setVisible(isViewingMenu);
            guiWeaponMenu->requestFocus();
            hero->setWeaponId(guiWeaponMenu->getSelectedIndex());
        }

        if((event.type == sf::Event::KeyPressed) && event.key.code == sf::Keyboard::BackSpace) {
            if(event.key.shift) {
                startStage();
            } else {
                startRound();
            }
        }

        if((event.type == sf::Event::KeyPressed) && event.key.code == sf::Keyboard::R) {
            hero->performStun();
        }

        if((event.type == sf::Event::KeyPressed) && event.key.code == sf::Keyboard::T) {
            if(auto gp = gameProgress.lock()) {
                gp->setPlayerEnergy(0);
            }
        }

        if((event.type == sf::Event::KeyPressed) && event.key.code == sf::Keyboard::Q) {
            hero->setWeaponId(hero->getWeaponId() - 1);
        }

        if((event.type == sf::Event::KeyPressed) && event.key.code == sf::Keyboard::W) {
            hero->setWeaponId(hero->getWeaponId() + 1);
        }
    }

    void GamePlayState::render(sf::RenderTarget &target) {
        if(subState) {
            subState->render(target);
        }

        if(auto gui = guiService.lock()) {
            gui->renderAsTop(guiContainer.get(), target);
        }

        if(drawInfamousBlackBar) {
            target.draw(leftBar);
        }
    }

    bool GamePlayState::update(const float &dt) {
        gotoNextState = false;

        //if(guiWeaponMenu) {
            guiWeaponMenu->logic();
        //}

        userInput->update(dt);

        if(eventBus) {
            eventBus->processEvents();
        }

        if(!taskQueue.empty()) {
            auto & task = taskQueue.front();
            task->update(dt);

            if(task->isComplete()) {
                taskQueue.pop();
            }
        } else {
            if(subState) {
                if(isRestoringEnergy) {
                    // Do the energy sequence
                } else {
                    if(isViewingMenu) {
                        if(auto gp = gameProgress.lock()) {
                            std::string livesCaption = (gp->getLives() < 10 ? "0" : "") + StringUtils::toString(static_cast<int>(gp->getLives()));
                            guiLivesLabel->setVisible(true);
                            guiLivesLabel->setCaption(livesCaption);
                            guiLivesLabel->adjustSize();

                            std::string etanksCaption = (gp->getETanks() < 10 ? "0" : "") + StringUtils::toString(static_cast<int>(gp->getETanks()));
                            guiETanksLabel->setCaption(etanksCaption);
                            guiETanksLabel->adjustSize();
                        }
                    } else {
                        // "Pause" the substate if the menu is being shown
                        // Handle state change request actions...
                        SubState::StateChangeAction action = subState->update(dt);

                        if(SubState::NEXT == action) {
                            if(nextSubState) {
                                changeSubState(std::move(nextSubState));
                            }
                        }
                    }
                }
            }

            if(currentTileset) {
                currentTileset->update(dt);
            }
        }

        return gotoNextState;
    }

    void GamePlayState::onEnter() {
        if(auto gui = guiService.lock()) {
            auto & topContainer = gui->getRootContainer();
            topContainer.add(guiContainer.get(), 0, 0);
            guiContainer->setEnabled(true);
            guiWeaponMenu->setEnabled(true);
            guiWeaponMenu->requestFocus();
        }

        Movable::setCollisionResolver(collisionResolver);
        Movable::setGravity(0.25f);

        std::vector<std::string> mapList;
        mapList.push_back("map-pearlman.json");
        mapList.push_back("map-test6.json");
        mapList.push_back("map-pearl.json");
        mapList.push_back("map-test4.json");
        mapList.push_back("map-test3.json");
        mapList.push_back("map-test2.json");

        if(auto gp = gameProgress.lock()) {
            // Determine which stage we're on and set that to the current level...
            if((currentMap = maps.at(mapList.at(gp->getCurrentBoss() % mapList.size())))) {
                currentTileset = currentMap->getTileset();
            }
        }

        startStage();
    }

    void GamePlayState::onExit() {
        if(auto gui = guiService.lock()) {
            auto & topContainer = gui->getRootContainer();
            topContainer.remove(guiContainer.get());
            guiContainer->setEnabled(false);
            guiWeaponMenu->setEnabled(false);
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

    void GamePlayState::requestSubStateChange(std::unique_ptr<SubState> && newSubState) {
        if(newSubState) {
            nextSubState = std::move(newSubState);
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

            // Clean up and then change the world's "room"
            world.removeAllObjects();
            world.setCurrentRoom(currentRoom);

            // Get links to all spawners from new room
            linkSpawners(currentRoom);

            // This was causing a bug. Any spawners that are visible before the level start were "waking" twice.
            //checkSpawners();

            // Let Rockman know where he is too
            hero->setRoom(currentRoom);
        }
    }

    void GamePlayState::linkSpawners(const std::shared_ptr<Room> & room) {
        if(room) {
            itemSpawners = room->getSpawnerList();
        }

        std::for_each(
            std::begin(itemSpawners),
            std::end(itemSpawners),
            [&](std::weak_ptr<Spawner> & s) {
                if(auto ptr = s.lock()) {
                    ptr->detachEventListeners(*eventBus.get());
                    ptr->attachEventListeners(*eventBus.get());
                    ptr->setAwake(false);
                }
            }
        );
    }

    void GamePlayState::checkSpawners(float dt) {
        const auto & cameraView = camera.getView();

        //
        // This will wake any spawners that are active when they come on screen.
        // It will also put to sleep any awake spawners when they go off screen.
        //
        std::for_each(
            std::begin(itemSpawners),
            std::end(itemSpawners),
            [this, &cameraView, &dt](std::weak_ptr<Spawner> & s) {
                if(auto spawner = s.lock()) {
                    spawner->update(dt);
                    const auto & spawnerPosition = spawner->getPosition();

                    if(spawner->isActive()) {
                        // If it's on screen
                        if(cameraView.contains(spawnerPosition.getX(), spawnerPosition.getY())) {
                            if(spawner->isAwake()) {
                                if(spawner->canSpawn()) {
                                    spawner->performAction(world);
                                }
                            } else {
                                spawner->setAwake(true);
                            }
                        } else {
                            if(spawner->canSleep()) {
                                spawner->setAwake(false);
                            }
                        }

                        // if(spawner->isAwake()) {
                        //     if(!cameraView.contains(spawnerPosition.getX(), spawnerPosition.getY())) {
                        //         if(spawner->canSleep()) {
                        //             spawner->setAwake(false);
                        //         }
                        //     }
                        // } else {
                        //     if(cameraView.contains(spawnerPosition.getX(), spawnerPosition.getY())) {
                        //         spawner->setAwake(true);

                        //         if(spawner->canSpawn()) {
                        //             spawner->performAction(world);
                        //         }
                        //     }
                        // }
                    }
                }
            }
        );
    }

    void GamePlayState::populateBonusChancesTable() {
        srand(time(nullptr));
        bonusChancesTable.push_back(std::make_pair(1,  "Extra Life"));
        bonusChancesTable.push_back(std::make_pair(2,  "Large Health Energy"));
        bonusChancesTable.push_back(std::make_pair(2,  "Large Weapon Energy"));
        bonusChancesTable.push_back(std::make_pair(15, "Small Health Energy"));
        bonusChancesTable.push_back(std::make_pair(15, "Small Weapon Energy"));
    }

    std::shared_ptr<CollectableItem> GamePlayState::spawnBonusItem(int bonusTableIndex) {
        std::shared_ptr<CollectableItem> bonus;

        int roll = rand() % 100;

        if(bonusChancesTable.size() > 0) {
            int lowerBound = 0;

            for(auto it = std::begin(bonusChancesTable), end = std::end(bonusChancesTable); it != end; it++) {
                const auto & chance = *it;

                int upperBound = lowerBound + chance.first;

                if(roll >= lowerBound && roll < upperBound) {
                    bonus = world.spawnCollectableItem(chance.second);
                    break;
                }

                // Advance the lower bound
                lowerBound = upperBound;
            }
        }

        return bonus;
    }

    void GamePlayState::spawnDeathExplosion(EntityDeathType::Type type, const Vector2<float> & position) {
        if(type == EntityDeathType::Hero) {
            // This type of explosion shoots in 8 directions. Thwo explosions per
            // direction; one fast and one slow. It's the death thath appens to Rock
            // as well as Robot Masters.
            std::list<Vector2<float>> velocities;
            velocities.emplace_back(Vector2<float>(-2.125f,  -2.125f )); // Fast up left
            velocities.emplace_back(Vector2<float>(0.0f,     -3.0f   )); // Fast up
            velocities.emplace_back(Vector2<float>(2.125f,   -2.125f )); // Fast up right
            velocities.emplace_back(Vector2<float>(-3.0f,     0.0f   )); // Fast left
            velocities.emplace_back(Vector2<float>(-2.125f,   2.125f )); // Fast down left
            velocities.emplace_back(Vector2<float>(0.0f,      3.0f   )); // Fast down
            velocities.emplace_back(Vector2<float>(2.125f,    2.125f )); // Fast down right
            velocities.emplace_back(Vector2<float>(3.0f,      0.0f   )); // Fast right
            velocities.emplace_back(Vector2<float>(-1.0625f, -1.0625f)); // Slow up left
            velocities.emplace_back(Vector2<float>(0.0f,     -1.5f   )); // Slow up
            velocities.emplace_back(Vector2<float>(1.0625f,  -1.0625f)); // Slow up right
            velocities.emplace_back(Vector2<float>(-1.5f,     0.0f   )); // Slow left
            velocities.emplace_back(Vector2<float>(-1.0625f,  1.0625f)); // Slow down left
            velocities.emplace_back(Vector2<float>(0.0f,      1.5f   )); // Slow down
            velocities.emplace_back(Vector2<float>(1.0625f,   1.0625f)); // Slow down right
            velocities.emplace_back(Vector2<float>(1.5f,      0.0f   )); // Slow right

            while(!velocities.empty()) {
                auto vel = velocities.front();
                velocities.pop_front();

                if(std::shared_ptr<Particle> clone = world.spawnParticle("Medium Explosion (Loop)")) {
                    clone->setPosition(position);
                    clone->setVelocity(vel);
                    clone->setActive(true);
                    world.queueObjectAddition(clone);
                }
            }
        } else if(type == EntityDeathType::Small) {
            if(std::shared_ptr<Particle> clone = world.spawnParticle("Medium Explosion")) {
                clone->setPosition(position);
                clone->setActive(true);
                world.queueObjectAddition(clone);
            }
        }
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

                    HIKARI_LOG(debug3) << "Found " << fileListing.size() << " file(s) in map directory.";

                    for(auto index = std::begin(fileListing), end = std::end(fileListing); index != end; index++) {
                        const std::string & fileName = (*index);
                        const std::string & filePath = stagesDirectory + "/" + fileName; // TODO: Handle file paths for real

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
                            spawner->reset();
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
        }

        startRound();
    }

    void GamePlayState::startRound() {
        if(auto gp = gameProgress.lock()) {
            gp->resetPlayerEnergyToDefault();
            gp->setCurrentWeapon(0);
            PalettedAnimatedSprite::setSharedPaletteIndex(3);
        }

        // Reset direction to face right
        hero->setDirection(Directions::Right);
        hero->setWeaponId(0);

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

    void GamePlayState::updateDoors(float dt) {
        if(currentRoom) {
            auto & exitDoor = currentRoom->getExitDoor();
            auto & entranceDoor = currentRoom->getEntranceDoor();

            if(exitDoor) {
                exitDoor->update(dt);
            }

            if(entranceDoor) {
                entranceDoor->update(dt);
            }
        }
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

        const auto & activeParticles = world.getActiveParticles();

        std::for_each(
            std::begin(activeParticles),
            std::end(activeParticles),
            std::bind(&Particle::render, std::placeholders::_1, ReferenceWrapper<sf::RenderTarget>(target)));

        const auto & activeProjectiles = world.getActiveProjectiles();

        std::for_each(
            std::begin(activeProjectiles),
            std::end(activeProjectiles),
            std::bind(&Projectile::render, std::placeholders::_1, ReferenceWrapper<sf::RenderTarget>(target)));

        // Restore UI view
        target.setView(oldView);
    }

    void GamePlayState::renderHud(sf::RenderTarget &target) const {

    }

    void GamePlayState::bindEventHandlers() {
        if(eventBus) {
            auto weaponFireDelegate = EventListenerDelegate(std::bind(&GamePlayState::handleWeaponFireEvent, this, std::placeholders::_1));
            eventBus->addListener(weaponFireDelegate, WeaponFireEventData::Type);
            eventHandlerDelegates.push_back(std::make_pair(weaponFireDelegate, WeaponFireEventData::Type));

            auto entityDamageDelegate = EventListenerDelegate(std::bind(&GamePlayState::handleEntityDamageEvent, this, std::placeholders::_1));
            eventBus->addListener(entityDamageDelegate, EntityDamageEventData::Type);
            eventHandlerDelegates.push_back(std::make_pair(entityDamageDelegate, EntityDamageEventData::Type));

            auto entityDeathDelegate = EventListenerDelegate(std::bind(&GamePlayState::handleEntityDeathEvent, this, std::placeholders::_1));
            eventBus->addListener(entityDeathDelegate, EntityDeathEventData::Type);
            eventHandlerDelegates.push_back(std::make_pair(entityDeathDelegate, EntityDeathEventData::Type));

            auto entityStateChangeDelegate = EventListenerDelegate(std::bind(&GamePlayState::handleEntityStateChangeEvent, this, std::placeholders::_1));
            eventBus->addListener(entityStateChangeDelegate, EntityStateChangeEventData::Type);
            eventHandlerDelegates.push_back(std::make_pair(entityStateChangeDelegate, EntityStateChangeEventData::Type));

            auto objectRemovedDelegate = EventListenerDelegate(&letMeKnowItsGone);
            eventBus->addListener(objectRemovedDelegate, ObjectRemovedEventData::Type);
            eventHandlerDelegates.push_back(std::make_pair(objectRemovedDelegate, ObjectRemovedEventData::Type));

            auto doorEventDelegate = EventListenerDelegate(std::bind(&GamePlayState::handleDoorEvent, this, std::placeholders::_1));
            eventBus->addListener(doorEventDelegate, DoorEventData::Type);
            eventHandlerDelegates.push_back(std::make_pair(doorEventDelegate, DoorEventData::Type));
        }
    }

    void GamePlayState::handleEntityDamageEvent(EventDataPtr evt) {
        auto eventData = std::static_pointer_cast<EntityDamageEventData>(evt);

        if(eventData->getEntityId() == hero->getId()) {
            if(auto sound = audioService.lock()) {
                sound->playSample(22); // SAMPLE_HERO_DAMAGE
            }
        }
    }

    void GamePlayState::handleEntityDeathEvent(EventDataPtr evt) {
        auto eventData = std::static_pointer_cast<EntityDeathEventData>(evt);

        if(eventData->getEntityId() == hero->getId()) {
            if(isHeroAlive) {
                isHeroAlive = false;

                //hero->setActive(false);
                //hero->setPosition(0.0f, 0.0f);

                if(auto progress = gameProgress.lock()) {
                    progress->setPlayerEnergy(0);

                    // Decrement lives
                    progress->setLives(progress->getLives() - 1);

                    // All the way dead
                    if(progress->getLives() < 0) {
                        HIKARI_LOG(debug2) << "Hero has died all of his lives, go to password screen.";
                        // TODO: Reset number of lives here to the default.
                        progress->resetLivesToDefault();
                        controller.requestStateChange("stageselect");
                        gotoNextState = true;
                    }
                }

                spawnDeathExplosion(hero->getDeathType(), hero->getPosition());

                HIKARI_LOG(debug) << "Hero died. Starting over.";

                if(auto sound = audioService.lock()) {
                    sound->stopMusic();
                    sound->stopAllSamples();
                    sound->playSample(23); // SAMPLE_HERO_DEATH
                }
            }
        } else if(eventData->getEntityType() == EntityDeathEventData::Enemy) {
            HIKARI_LOG(debug2) << "An enemy died! id = " << eventData->getEntityId();

            auto enemyPtr = std::dynamic_pointer_cast<Enemy>(world.getObjectById(eventData->getEntityId()).lock());

            if(enemyPtr) {
                world.queueObjectRemoval(enemyPtr);

                spawnDeathExplosion(enemyPtr->getDeathType(), enemyPtr->getPosition());

                // Calculate bonus drop
                if(auto bonus = spawnBonusItem()) {
                    bonus->setPosition(enemyPtr->getPosition());
                    bonus->setVelocityY(-3.0f); // TODO: Determine the actual upward velocity.
                    bonus->setActive(true);
                    bonus->setAgeless(false);
                    bonus->setMaximumAge(3.0f);
                    world.queueObjectAddition(bonus);
                }
            }
        } else if(eventData->getEntityType() == EntityDeathEventData::Item) {
            HIKARI_LOG(debug2) << "An item died! id = " << eventData->getEntityId();

            auto itemPtr = std::dynamic_pointer_cast<CollectableItem>(world.getObjectById(eventData->getEntityId()).lock());

            if(itemPtr) {
                world.queueObjectRemoval(itemPtr);
            }
        } else if(eventData->getEntityType() == EntityDeathEventData::Projectile) {
            HIKARI_LOG(debug2) << "A projectile died! id = " << eventData->getEntityId();

            auto projectilePtr = std::dynamic_pointer_cast<Projectile>(world.getObjectById(eventData->getEntityId()).lock());

            if(projectilePtr) {
                world.queueObjectRemoval(projectilePtr);
            }
        }
    }

    void GamePlayState::handleWeaponFireEvent(EventDataPtr evt) {
        auto eventData = std::static_pointer_cast<WeaponFireEventData>(evt);
        HIKARI_LOG(debug) << "Member Weapon Fired! wid=" <<
                          eventData->getWeaponId() << ", sid=" << eventData->getShooterId() <<
                          ", faction=" << eventData->getFaction() <<
                          ", direction=" << eventData->getDirection();

        if(auto weapons = weaponTable.lock()) {
            auto weaponWeak = weapons->getWeaponById(eventData->getWeaponId());
            if(auto weapon = weaponWeak.lock()) {
                Shot shot = weapon->fire(world, *eventData.get());

                // This is dirty and I don't like it, so I should design a better
                // way to accomplish this part.
                if(eventData->getShooterId() == hero->getId()) {
                    hero->observeShot(shot);

                    HIKARI_LOG(debug4) << "Hero's shot count: " << hero->getActiveShotCount();
                } else {
                    // It could be an enemy...
                    // So we need to somehow get the enemy by ID and make it
                    // observe the shot. It would be nice to do this without
                    // casting.
                    // TODO: CLEAN ME / CASTING
                    std::weak_ptr<GameObject> possibleEnemyPtr = world.getObjectById(eventData->getShooterId());

                    if(auto enemyGoPtr = possibleEnemyPtr.lock()) {
                        if(std::shared_ptr<Enemy> enemy = std::static_pointer_cast<Enemy>(enemyGoPtr)) {
                            enemy->observeShot(shot);
                        }
                    }
                }


                if(auto sound = audioService.lock()) {
                    sound->playSample(21);
                }

            } else {
                HIKARI_LOG(debug4) << "Tried to fire weapon with bad ID (" << eventData->getWeaponId() << ")";
            }
        }
    }

    void GamePlayState::handleEntityStateChangeEvent(EventDataPtr evt) {
        auto eventData = std::static_pointer_cast<EntityStateChangeEventData>(evt);

        if(eventData->getEntityId() == hero->getId()) {
            if(eventData->getStateName() == "water") {
                // if(auto sound = audioService.lock()) {
                //     sound->playSample(19);
                // }
                if(std::shared_ptr<Particle> clone = world.spawnParticle("Medium Splash")) {
                    clone->setPosition(Vector2<float>(
                        hero->getPosition().getX(),
                        static_cast<float>(static_cast<int>(std::floor(hero->getPosition().getY())) / 16) * 16)
                    );
                    clone->setActive(true);
                    world.queueObjectAddition(clone);
                }

                if(auto sound = audioService.lock()) {
                    sound->playSample(31);
                }
            }

            if(eventData->getStateName() == "landed") {
                if(auto sound = audioService.lock()) {
                    sound->playSample(19);
                }
            } else if(eventData->getStateName() == "teleporting") {
                if(auto sound = audioService.lock()) {
                    sound->playSample(52);
                }
            } else if(eventData->getStateName() == "sliding") {
                if(std::shared_ptr<Particle> clone = world.spawnParticle("Sliding Dust")) {
                    clone->setPosition(hero->getPosition());
                    clone->setActive(true);
                    world.queueObjectAddition(clone);
                }
            }
        }
    }

    void GamePlayState::handleDoorEvent(EventDataPtr evt) {
        // auto eventData = std::static_pointer_cast<DoorEventData>(evt);
        if(auto sound = audioService.lock()) {
            sound->playSample(29);
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

        gamePlayState.guiReadyLabel->setVisible(false);
    }

    GamePlayState::ReadySubState::~ReadySubState() {

    }

    void GamePlayState::ReadySubState::enter() {
        gamePlayState.canViewMenu = false;

        timer = 0.0f;

        renderFadeOverlay = true;
        renderReadyText = false;
        gamePlayState.guiHeroEnergyGauge->setVisible(false);

        if(auto gp = gamePlayState.gameProgress.lock()) {
            gamePlayState.guiHeroEnergyGauge->setValue(
                static_cast<float>(gp->getPlayerEnergy())
            );
        }

        sf::Color overlayColor = sf::Color(fadeOverlay.getFillColor());
        overlayColor.a = 255;

        fadeOverlay.setFillColor(overlayColor);

        if(auto sound = gamePlayState.audioService.lock()) {
            HIKARI_LOG(debug) << "Playing music for the level!";
            sound->playMusic(gamePlayState.currentMap->getMusicId());
        }

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

    GamePlayState::SubState::StateChangeAction GamePlayState::ReadySubState::update(const float & dt) {
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

        gamePlayState.guiReadyLabel->setVisible(renderReadyText);

        // The "READY" sequence is 76 frames long, ~1.2666 seconds.
        if(timer >= (76.0f * frameMs)) {
            gamePlayState.requestSubStateChange(std::unique_ptr<SubState>(new TeleportSubState(gamePlayState)));
            return SubState::NEXT;
        }

        gamePlayState.updateDoors(dt);

        return SubState::CONTINUE;
    }

    void GamePlayState::ReadySubState::render(sf::RenderTarget &target) {
        gamePlayState.renderMap(target);

        if(renderFadeOverlay) {
            target.draw(fadeOverlay);
        }

        if(renderReadyText) {
            // gamePlayState.guiFont->renderText(target, "READY", 108, 121);
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
        gamePlayState.canViewMenu = false;

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

    GamePlayState::SubState::StateChangeAction GamePlayState::TeleportSubState::update(const float & dt) {
        auto& hero = gamePlayState.hero;
        auto& heroPosition = hero->getPosition();

        if(heroPosition.getY() < targetPoint.getY()) {
            const float verticalTeleportSpeedPerFrame = 16.0f;

            // Make sure we don't teleport too far.
            float deltaY = std::min(verticalTeleportSpeedPerFrame,
                                std::abs(targetPoint.getY() - heroPosition.getY()));

            hero->setPosition(heroPosition.getX(), heroPosition.getY() + deltaY);
        } else {
            hero->performMorph();
            gamePlayState.requestSubStateChange(std::unique_ptr<SubState>(new PlayingSubState(gamePlayState)));
            return SubState::NEXT;
        }

        gamePlayState.updateDoors(dt);

        return SubState::CONTINUE;
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
        gamePlayState.canViewMenu = true;
        gamePlayState.isHeroAlive = true;
        postDeathTimer = 0.0f;

        // Check if we just entered the room where the boss battle will take place
        if(gamePlayState.currentRoom == gamePlayState.currentMap->getBossChamberRoom()) {
            // We're going to start fighting the boss
            HIKARI_LOG(debug3) << "We just entered the boss chamber. Time to start the battle!";
        }

        // Remove any enemies that may have been there from before
        auto & staleEnemies = gamePlayState.world.getActiveEnemies();

        std::for_each(std::begin(staleEnemies), std::end(staleEnemies), [&](const std::shared_ptr<Enemy> & enemy) {
            HIKARI_LOG(debug2) << "Removing stale enemy, id = " << enemy->getId();
            gamePlayState.world.queueObjectRemoval(enemy);
        });
    }

    void GamePlayState::PlayingSubState::exit() {

    }

    GamePlayState::SubState::StateChangeAction GamePlayState::PlayingSubState::update(const float & dt) {
        auto& camera = gamePlayState.camera;

        auto playerPosition = gamePlayState.world.getPlayerPosition();

        Sqrat::RootTable()
            .SetValue("heroX", playerPosition.getX())
            .SetValue("heroY", playerPosition.getY());

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

                    // Don't call onDeath since this is non a "natural" death
                    gamePlayState.world.queueObjectRemoval(item);
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
                    item->onDeath();
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

                const auto & cameraView = camera.getView();

                if(!geom::intersects(enemy->getBoundingBox(), cameraView)) {
                    HIKARI_LOG(debug3) << "Cleaning up off-screen enemy #" << enemy->getId();
                    enemy->setActive(false);
                    gamePlayState.world.queueObjectRemoval(enemy);
                }

                //
                // This part "damages" the hero if he touches an enemy.
                //
                const auto & hero = gamePlayState.hero;

                if(enemy->getBoundingBox().intersects(hero->getBoundingBox())) {
                    DamageKey damageKey;
                    damageKey.damagerType = enemy->getDamageId();
                    damageKey.damageeType = hero->getDamageId();

                    // TODO: Perform damage lookup and apply it to hero.
                    // START DAMAGE RESOLVER LOGIC
                    float damageAmount = 0.0f;

                    if(auto dt = gamePlayState.damageTable.lock()) {
                        damageAmount = dt->getDamageFor(damageKey.damagerType);
                    }
                    // END DAMAGE RESOLVER LOGIC

                    HIKARI_LOG(debug3) << "Hero should take " << damageAmount << " damage!";

                    if(hero->isVulnerable()) {
                        if(auto gp = gamePlayState.gameProgress.lock()) {
                            gp->setPlayerEnergy(
                                gp->getPlayerEnergy() - 5
                            );

                            HIKARI_LOG(debug4) << "My energy is " << gp->getPlayerEnergy();

                            // Only stun if you're not dead
                            if(gp->getPlayerEnergy() > 0) {
                                hero->performStun();
                            }
                        }
                    }
                }

        });

        //
        // Update particles
        //
        const auto & activeParticles = gamePlayState.world.getActiveParticles();

        std::for_each(
            std::begin(activeParticles),
            std::end(activeParticles),
            [this, &camera, &dt](const std::shared_ptr<Particle> & particle) {
                particle->update(dt);

                const auto & cameraView = camera.getView();

                if(!geom::intersects(particle->getBoundingBox(), cameraView)) {
                    HIKARI_LOG(debug3) << "Cleaning up off-screen particle #" << particle->getId();
                    particle->setActive(false);
                }

                if(!particle->isActive()) {
                    gamePlayState.world.queueObjectRemoval(particle);
                }
        });

        //
        // Update projectiles
        //
        const auto & activeProjectiles = gamePlayState.world.getActiveProjectiles();

        std::for_each(
            std::begin(activeProjectiles),
            std::end(activeProjectiles),
            [this, &activeEnemies, &camera, &dt](const std::shared_ptr<Projectile> & projectile) {
                projectile->update(dt);

                const auto & cameraView = camera.getView();

                if(!geom::intersects(projectile->getBoundingBox(), cameraView)) {
                    HIKARI_LOG(debug3) << "Cleaning up off-screen projectile #" << projectile->getId();
                    projectile->setActive(false);
                    gamePlayState.world.queueObjectRemoval(projectile);
                }

                // Check Hero -> Enemy projectiles
                if(projectile->getFaction() == Faction::Hero) {
                    auto & context = *this; // TODO: This makes intellisense errors go away but it seems kludgy.
                                            // In VS2010, this is the Intellisense error:
                                            //     "Error : a nonstatic member reference must be relative to a specific object"
                                            // However, despite the error the code compiles fine.

                    // Check for collision with enemies
                    std::for_each(
                        std::begin(activeEnemies),
                        std::end(activeEnemies),
                        [&](const std::shared_ptr<Enemy> & enemy) {
                            if(!projectile->isInert()) {
                                if(projectile->getBoundingBox().intersects(enemy->getBoundingBox())) {
                                    if(enemy->isShielded()) {
                                         // Deflect projectile
                                        projectile->deflect();

                                        if(auto sound = context.gamePlayState.audioService.lock()) {
                                            HIKARI_LOG(debug4) << "PLAYING SAMPLE weapon DEFLECTED";
                                            sound->playSample(25); // SAMPLE_HERO_DEATH
                                        }
                                    } else {
                                        HIKARI_LOG(debug3) << "Hero bullet " << projectile->getId() << " hit an enemy " << enemy->getId();
                                        projectile->setActive(false);
                                        context.gamePlayState.world.queueObjectRemoval(projectile);

                                        DamageKey damageKey;
                                        damageKey.damagerType = projectile->getDamageId();
                                        damageKey.damageeType = enemy->getDamageId();

                                        HIKARI_LOG(debug3) << "Hero bullet damage id = " << projectile->getDamageId();

                                        // TODO: Perform damage lookup and apply it to hero.
                                        // Trigger enemy damage
                                        float damageAmount = 0.0f;

                                        if(auto dt = context.gamePlayState.damageTable.lock()) {
                                            damageAmount = dt->getDamageFor(damageKey.damagerType);
                                        }

                                        HIKARI_LOG(debug3) << "Enemy took " << damageAmount;

                                        enemy->takeDamage(damageAmount);

                                        if(auto sound = context.gamePlayState.audioService.lock()) {
                                            sound->playSample(24); // SAMPLE_HERO_DEATH
                                        }
                                    }
                                }
                            }
                        }
                    );
                } else if(projectile->getFaction() == Faction::Enemy) {

                    const auto & hero = gamePlayState.hero;

                    // Check for collision with hero
                    if(projectile->getBoundingBox().intersects(hero->getBoundingBox())) {
                        HIKARI_LOG(debug3) << "Enemy bullet " << projectile->getId() << " hit the hero!";

                        // DamageKey damageKey;
                        // damageKey.damagerType = projectile->getDamageId();
                        // damageKey.damageeType = hero->getDamageId();

                        // TODO: Perform damage lookup and apply it to hero.

                        if(hero->isVulnerable()) {
                            hero->performStun();
                            projectile->setActive(false);
                            gamePlayState.world.queueObjectRemoval(projectile);
                        }
                    }
                }
        });

        gamePlayState.updateDoors(dt);

        // Hero died so we need to restart
        if(!gamePlayState.isHeroAlive) {
            postDeathTimer += dt;

            // Wait 1 second after you died and then restart
            if(postDeathTimer >= 2.5f) {
                gamePlayState.startRound();
                return SubState::NEXT;
            }
        } else {
            // TODO: Note to self -- this seems pretty convoluted... probably change this soon please.
            //
            // Update hero
            //
            if(gamePlayState.hero) {
                // Check to see if there are any ladders to grab on to.
                auto index = std::begin(gamePlayState.currentRoom->getLadders());
                const auto & end = std::end(gamePlayState.currentRoom->getLadders());

                while(index != end) {
                    const BoundingBox<float> & ladder = *index;

                    if(gamePlayState.hero->getBoundingBox().intersects(ladder)) {
                       gamePlayState.hero->requestClimbingAttachment(ladder);
                    }

                    index++;
                }

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

        gamePlayState.checkSpawners(dt);

        //
        // Check if hero has touched any transitions
        //
        auto & currentRoom = gamePlayState.currentRoom;
        auto & currentRoomTransitions = currentRoom->getTransitions();

        for(auto transitionIt = currentRoomTransitions.begin(), end = currentRoomTransitions.end(); transitionIt != end; transitionIt++) {
            const RoomTransition& transition = *transitionIt;

            int regionLeft   = ((currentRoom->getX() + transition.getX()) * currentRoom->getGridSize());
            int regionTop    = ((currentRoom->getY() + transition.getY()) * currentRoom->getGridSize());
            int regionWidth  = transition.getWidth() * currentRoom->getGridSize();
            int regionHeight = transition.getHeight() * currentRoom->getGridSize();

            BoundingBox<float> transitionBounds(
                static_cast<float>(regionLeft),
                static_cast<float>(regionTop),
                static_cast<float>(regionWidth),
                static_cast<float>(regionHeight)
            );

            // Boss entrances can be triggered by merely touching, but regular
            // transitions require Rock to be fully contained before triggering.
            if(transition.isDoor()) {
                if(transitionBounds.intersects(hero->getBoundingBox())) {
                    HIKARI_LOG(debug) << "Transitioning from room " << currentRoom->getId() << " through a door " << transition.getToRegion();
                    gamePlayState.requestSubStateChange(std::unique_ptr<SubState>(new TransitionSubState(gamePlayState, transition)));
                    return SubState::NEXT;
                }
            } else if(transitionBounds.contains(hero->getBoundingBox())) {
                HIKARI_LOG(debug) << "Transitioning from room " << currentRoom->getId() << " to room " << transition.getToRegion();
                gamePlayState.requestSubStateChange(std::unique_ptr<SubState>(new TransitionSubState(gamePlayState, transition)));
                return SubState::NEXT;
            }
        }

        if(auto gp = gamePlayState.gameProgress.lock()) {
            int playerEnergy = gp->getPlayerEnergy();

            if(playerEnergy <= 0) {
                hero->kill();
            }

            gamePlayState.guiHeroEnergyGauge->setValue(
                static_cast<float>(gp->getPlayerEnergy())
            );
        }

        return SubState::CONTINUE;
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
    const float GamePlayState::TransitionSubState::doorDelay = 4.0f * (1.0f / 60.0f) * 4.0; // 4 frames each section, 4 sections

    GamePlayState::TransitionSubState::TransitionSubState(GamePlayState & gamePlayState, RoomTransition transition)
        : SubState(gamePlayState)
        , transitionEndX(0.0f)
        , transitionEndY(0.0f)
        , transitionFrames(0)
        , transitionFinished(false)
        , doorDelayIn(0.0f)
        , doorDelayOut(0.0f)
        , transition(transition)
        , nextRoomCullRegion()
        , nextRoom(nullptr)
        , entranceDoor(nullptr)
        , exitDoor(nullptr)
    {
        auto & camera = gamePlayState.camera;

        if(transition.isDoor()) {
            exitDoor = gamePlayState.currentRoom->getExitDoor();
        }

        nextRoom = findNextRoom();

        if(nextRoom) {
            if(transition.isDoor()) {
                entranceDoor = nextRoom->getEntranceDoor();
            }

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
        gamePlayState.canViewMenu = false;

        HIKARI_LOG(debug) << "TransitionSubState::enter()";

        if(transition.isDoor()) {
            // Open the door sequence
            doorDelayOut = doorDelayIn = doorDelay;

            // Attempt to open current room's exit door (since you're leaving
            // that room for another)
            if(exitDoor) {
                HIKARI_LOG(debug4) << "Opening exit door in current room";
                exitDoor->open();

                if(gamePlayState.eventBus) {
                    gamePlayState.eventBus->triggerEvent(EventDataPtr(new DoorEventData(exitDoor)));
                }
            } else {
                HIKARI_LOG(debug4) << "Current room has no exit door";
            }

            if(entranceDoor) {
                HIKARI_LOG(debug4) << "Opening entrance door in next room";
                entranceDoor->setOpen(); // Make it fully open automatically
            }
        }

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

    GamePlayState::SubState::StateChangeAction GamePlayState::TransitionSubState::update(const float & dt) {
        auto & camera = gamePlayState.camera;
        auto & hero = gamePlayState.hero;

        float camY = camera.getY();
        float camX = camera.getX();

        if(exitDoor) {
            exitDoor->update(dt);
        }

        if(entranceDoor) {
            entranceDoor->update(dt);
        }

        // For boss doors there is a delay before the camera transition
        if(doorDelayIn > 0.0f) {
            doorDelayIn -= dt;
        } else {
            switch(transition.getDirection()) {
                case RoomTransition::DirectionUp:
                    if(camY > transitionEndY) {
                        camera.move(0.0f, -transitionSpeedY * dt);
                        transitionFrames++;

                        float heroTranslateY = -heroTranslationSpeedY * dt;
                        hero->setPosition(hero->getPosition().getX(), hero->getPosition().getY() + heroTranslateY);
                        hero->playAnimation(dt);
                    } else {
                        transitionFinished = true;
                    }
                    break;
                case RoomTransition::DirectionForward:
                    camera.lockHorizontal(false);
                    if(camX < transitionEndX) {
                        camera.move(transitionSpeedX * dt, 0.0f);
                        transitionFrames++;

                        float heroTranslateX = heroTranslationSpeedX * dt;
                        hero->setPosition(hero->getPosition().getX() + heroTranslateX, hero->getPosition().getY());
                        hero->playAnimation(dt);
                    } else {
                        transitionFinished = true;
                    }
                    break;
                case RoomTransition::DirectionDown:
                    camera.lockVertical(false);
                    if(camY < transitionEndY) {
                        camera.move(0.0f, transitionSpeedY * dt);
                        transitionFrames++;

                        float heroTranslateY = heroTranslationSpeedY * dt;
                        hero->setPosition(hero->getPosition().getX(), hero->getPosition().getY() + heroTranslateY);
                        hero->playAnimation(dt);
                    } else {
                        transitionFinished = true;
                    }
                    break;
                case RoomTransition::DirectionBackward:
                    camera.lockHorizontal(false);
                    if(camX > transitionEndX) {
                        camera.move(-transitionSpeedX * dt, 0.0f);
                        transitionFrames++;

                        float heroTranslateX = -heroTranslationSpeedX * dt;
                        hero->setPosition(hero->getPosition().getX() + heroTranslateX, hero->getPosition().getY());
                        hero->playAnimation(dt);
                    } else {
                        transitionFinished = true;
                    }
                    break;
                default:
                    transitionFinished = true;
                    break;
            }
        }

        if(transitionFinished) {
            // For boss doors there is a delay after the camera transition.
            if(doorDelayOut > 0.0f) {

                if(doorDelayOut == doorDelay) {
                    // Attempt to open current room's exit door (since you're leaving
                    // that room for another)
                    if(entranceDoor) {
                        HIKARI_LOG(debug4) << "Closing entrance door in next room";
                        entranceDoor->close();

                        if(gamePlayState.eventBus) {
                            gamePlayState.eventBus->triggerEvent(EventDataPtr(new DoorEventData(exitDoor)));
                        }
                    } else {
                        HIKARI_LOG(debug4) << "Next room has no entrance door";
                    }

                    if(exitDoor) {
                        HIKARI_LOG(debug4) << "Closing exit door in previous room";
                        exitDoor->setClosed();
                    }
                }

                doorDelayOut -= dt;
            } else {
                gamePlayState.requestSubStateChange(std::unique_ptr<SubState>(new PlayingSubState(gamePlayState)));
                return SubState::NEXT;
            }
        }

        return SubState::CONTINUE;
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
