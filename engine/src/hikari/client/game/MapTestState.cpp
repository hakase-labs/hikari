#include "hikari/client/game/MapTestState.hpp"
#include "hikari/client/game/RealTimeInput.hpp"
#include "hikari/client/Services.hpp"
#include "hikari/client/scripting/SquirrelService.hpp"
#include "hikari/core/util/PhysFS.hpp"
#include "hikari/core/util/PhysFSUtils.hpp"
#include "hikari/core/util/ServiceLocator.hpp"
#include "hikari/core/util/StringUtils.hpp"
#include <SFML/Window/Event.hpp>
#include <json/reader.h>
#include <json/value.h>
#include "hikari/core/util/Log.hpp"
#include <cmath>
#include <string>
#include <iostream>
#include <memory>
#include <cstdarg>

#include "hikari/client/game/objects/Entity.hpp"
#include "hikari/client/game/objects/Spawner.hpp"
#include "hikari/client/game/objects/Hero.hpp"
#include "hikari/client/game/objects/Enemy.hpp"
#include "hikari/client/game/objects/EnemyBrain.hpp"
#include "hikari/client/game/objects/brains/ScriptedEnemyBrain.hpp"
#include "hikari/client/game/objects/CollectableItem.hpp"
#include "hikari/client/game/objects/effects/NothingEffect.hpp"
#include "hikari/client/game/objects/effects/ScriptedEffect.hpp"
#include "hikari/client/game/objects/EnemyFactory.hpp"
#include "hikari/core/game/Movable.hpp"
#include "hikari/core/game/TileMapCollisionResolver.hpp"
#include "hikari/core/util/ReferenceWrapper.hpp"

#include "hikari/client/game/objects/controllers/PlayerInputHeroActionController.hpp"

#include "hikari/core/util/AnimationSetCache.hpp"
#include "hikari/core/util/ImageCache.hpp"
#include "hikari/client/game/objects/ItemFactory.hpp"

namespace hikari {

    class ImageCache;

    const float MapTestState::transitionSpeedX = 4.0f / (1.0f / 60.0f);
    const float MapTestState::transitionSpeedY = 3.0f / (1.0f / 60.0f);
    const float MapTestState::heroTranslationSpeedX = (51.0f / 64.0f) / (1.0f / 60.0f); // TODO: This number might be off
    const float MapTestState::heroTranslationSpeedY = (21.0f / 80.0f) / (1.0f / 60.0f); // This one too...

    MapTestState::MapTestState(
            const std::string &name,
            const std::shared_ptr<MapLoader> &mapLoader,
            const std::string &mapFile,
            const std::string &tileFile,
            const std::shared_ptr<ImageCache> &imageCache,
            const std::shared_ptr<ImageFont> &font,
            const std::weak_ptr<ItemFactory> &itemFactory,
            const std::weak_ptr<EnemyFactory> &enemyFactory,
            ServiceLocator &services)
        : name(name)
        , mapLoader(mapLoader)
        , renderer(currentRoom, tiles)
        , font(font)
        , view(sf::FloatRect(0.0f, 0.0f, 256.0f, 240.0f))
        , camera(Rectangle2D<float>(0.0f, 0.0f, 256.0f, 240.0f))
        , cameraViewportOutline(sf::Vector2f(256.0f, 240.0f))
        , imageCache(imageCache)
        , itemFactory(itemFactory)
        , grounded(false)
        , input(new RealTimeInput())
        , velocity(0.0f, 0.0f)
        , logicalCursor()
        , renderedCursor()
        , animationPlayer(sprite)
        , collisionResolver(new TileMapCollisionResolver())
        , movable(new Movable())
        , hero(new Hero(GameObject::generateObjectId(), nullptr))
        , enemy(new Enemy(GameObject::generateObjectId(), nullptr))
        , playFieldZoom(1.0f)
        , renderWindow(nullptr)
    {
        squirrel = std::shared_ptr<SquirrelService>(services.locateService<SquirrelService>(Services::SCRIPTING));
        animationSetCache = std::shared_ptr<AnimationSetCache>(services.locateService<AnimationSetCache>("AnimationSetCache"));

        // itemFactory = std::make_shared<ItemFactory>(animationSetCache, imageCache, squirrel);
        animations = animationSetCache->get("assets/animations/heroes.json");
        idleAnimation = animations->get("idle");
        runningAnimation = animations->get("running");
        redRunningAnimation = animations->get("idle");

            loadMap(mapFile);

            renderer.setTileData(tiles); // TODO: This should be in a different method
            renderer.setRoom(currentRoom);  // TODO: This should be in a different method

            logicalCursor.setPosition(35.0f, 35.0f);

            collidingWithTransition = false;
            cameraFollowingPlayer = true;
            transitioning = false;

            initializeCamera();

            transitionFrames = 0;
            jumpIterations = 0;

            retroGravity = NESNumber(0, 0x40);
            retroPositionY = NESNumber(0, 0);
            retroVelocityY = NESNumber(0, 0);
            retroVelocityX = NESNumber(0, 0);
            retroJumpVelocity = NESNumber(0x04, 0xA5);

            //Entity ent = Entity(1, currentRoom);

            auto enemyAnimations = animationSetCache->get("assets/animations/enemies.json");
            auto enemySprite = sf::Texture();

            PhysFSUtils::loadImage(animationSetCache->get("assets/animations/items.json")->getImageFileName(), enemySprite);
            enemySprite.setSmooth(false);

            PhysFSUtils::loadImage(animations->getImageFileName(), spriteImage);
            spriteImage.setSmooth(false);
            sprite.setTexture(spriteImage);
            sprite.setPosition(100.0f, 100.0f);
            animationPlayer.setAnimation(animations->get("idle"));

            setupHero();
            // setupEnemy();

            world.setPlayer(hero);

            spawnerMarker.setFillColor(sf::Color(255, 0, 255, 128));
            spawnerMarker.setOutlineColor(sf::Color::White);
            spawnerMarker.setOutlineThickness(1.0f);
            spawnerMarker.setSize(sf::Vector2f(16.0f, 16.0f));
            spawnerMarker.setOrigin(8.0f, 8.0f);

            transitionMarker.setFillColor(sf::Color(64, 64, 255, 128));
            transitionMarker.setOutlineColor(sf::Color::White);
            transitionMarker.setOutlineThickness(1.0f);
            transitionMarker.setSize(sf::Vector2f(16.0f, 16.0f));
            transitionMarker.setOrigin(0.0f, 0.0f);

            sf::Color cameraOutlineColor(sf::Color::Green);
            cameraOutlineColor.a = 128;
            cameraViewportOutline.setOutlineColor(cameraOutlineColor);
            cameraViewportOutline.setOutlineThickness(3.0f);
            cameraViewportOutline.setFillColor(sf::Color::Transparent);

            std::vector<std::string> scriptFiles;
            //scriptFiles.push_back("assets/scripts/EffectBase.nut");
            //scriptFiles.push_back("assets/scripts/behaviors/EnemyBehavior.nut");
            //scriptFiles.push_back("assets/scripts/behaviors/TellyBehavior.nut");
            //scriptFiles.push_back("assets/scripts/behaviors/OctopusBatteryEnemyBehavior.nut");

            //std::for_each(std::begin(scriptFiles), std::end(scriptFiles), [this](const std::string & scriptFileName) {
            //    squirrel->runScriptFile(scriptFileName);
            //});

            item = itemFactory.lock()->createItem("Large Health Energy");

            //item.reset(new CollectableItem(7, currentRoom, std::make_shared<ScriptedEffect>(*squirrel, "EffectBase")));
            //item->setAnimationSet(AnimationLoader::loadSet("assets/animations/items.json"));
            //item->setSpriteTexture(enemySprite);
            //item->changeAnimation("e-tank");
            item->setRoom(currentRoom);
            item->setPosition(256.0f + 65.0f, 645.0f);
            item->setAgeless(true);
            item->setActive(true);
            item->setGravitated(true);

            //BoundingBoxF itemBounds = BoundingBoxF(256.0f + 65.0f, 645.0f, 16.0f, 16.0f);

            //item->setBoundingBox(itemBounds);

            //auto clone = item->clone();
            if(auto enemyFactoryPtr = enemyFactory.lock()) {
                // auto en6 = spawnEnemy("scripted-octopusbattery");
                // en6->setDirection(Directions::Up);
                // en6->setPosition(184, 120);
                // enemies.push_back(en6);

                //auto en7 = spawnEnemy("scripted-telly");
                if(auto en7 = enemyFactoryPtr->create("Telly")) {
                    en7->setDirection(Directions::Up);
                    en7->setPosition(16*12, 16*66);
                    en7->setRoom(currentRoom);
                    enemies.push_back(std::shared_ptr<Enemy>(std::move(en7)));
                }

                //auto en8 = en7->clone();
                if(auto en8 = enemyFactoryPtr->create("Telly")) {
                    en8->setDirection(Directions::Up);
                    en8->setPosition(16*10, 16*70);
                    en8->setRoom(currentRoom);
                    enemies.push_back(std::shared_ptr<Enemy>(std::move(en8)));
                }

                if(auto en9 = enemyFactoryPtr->create("Octopus Battery")) {
                    en9->setDirection(Directions::Left);
                    en9->setPosition(16*10 + 500, 16*70 - 10);
                    en9->setRoom(currentRoom);
                    enemies.push_back(std::shared_ptr<Enemy>(std::move(en9)));
                }
            }

            //scriptFiles.clear();
            //scriptFiles.push_back("TellyBehaviorOverrides.nut");
            //std::for_each(std::begin(scriptFiles), std::end(scriptFiles), [this](const std::string & scriptFileName) {
            //    squirrel->runScriptFile(scriptFileName);
            //});
    }

    MapTestState::~MapTestState() {
        enemies.clear();
    }

    void MapTestState::loadMap(const std::string &mapFile) {
        Json::Value json = PhysFSUtils::loadJson(mapFile);

        map = mapLoader->loadFromJson(json);
        tiles = map->getTileset();

        currentRoom = map->getRoom(0);
    }

    void MapTestState::initializeCamera() {
        camera.setBoundary(currentRoom->getCameraBounds());
        camera.lockHorizontal(true);
        camera.lockVertical(true);
    }

    void MapTestState::handleEvent(sf::Event &event) {
        if((event.type == sf::Event::KeyPressed) && event.key.code == sf::Keyboard::Return) {
            std::cout << "Collecting garbage..." << std::endl;
            squirrel->collectGarbage();
        }

        if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            auto coords = screenCoordToWorldCoord(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
            hero->setPosition(coords);
        }

        if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
            auto coords = screenCoordToWorldCoord(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
            //auto item = itemFactory->createItem("extraLife");
            auto enemy = spawnEnemy("scripted-telly");

            enemy->setPosition(coords);
            enemy->setDirection(Directions::Up);
            enemy->setRoom(currentRoom);

            enemies.push_back(enemy);


        }

        if(event.type == sf::Event::KeyPressed) {
            if(event.key.code == sf::Keyboard::Num1) {
                camera.lockHorizontal(camera.canMoveHorizontal());
            }

            if(event.key.code == sf::Keyboard::Num2) {
                camera.lockVertical(camera.canMoveVertical());
            }

            if(event.key.code == sf::Keyboard::Num3) {
                cameraFollowingPlayer = !cameraFollowingPlayer;
            }

            if(event.key.code == sf::Keyboard::Num4) {
                item->setActive(true);
            }

            if(event.key.code == sf::Keyboard::Num5) {
                Entity::enableDebug(true);
            }

            if(event.key.code == sf::Keyboard::Num6) {
                Entity::enableDebug(false);
            }

            if(event.key.code == sf::Keyboard::Equal) {
                playFieldZoom += 0.25;
            }

            if(event.key.code == sf::Keyboard::Subtract) {
                playFieldZoom -= 0.25;
            }
        }
    }

    void MapTestState::render(sf::RenderTarget &target) {
        // Save old view
        const sf::View &oldView = target.getDefaultView();
        // also sf::View oldView = target.GetView(); might be better but we'll see

        auto cameraView = camera.getPixelAlignedView();

        cameraViewportOutline.setPosition(std::floor(camera.getView().getLeft()), std::floor(camera.getView().getTop()));

        view.setCenter(cameraView.getCenter());

        cameraView.zoom(playFieldZoom);
        //cameraView.setRotation(45.0f);

        target.setView(cameraView);

        // Render the map
        renderer.setRoom(currentRoom);
        renderer.render(target);

        target.draw(cameraViewportOutline);

        if(transitioning) {
            renderer.setRoom(nextRoom);
            renderer.render(target);
        }

        // Draw player
        if(collidingWithTransition) {
            renderedCursor.setFillColor(sf::Color::Red);
        } else {
            renderedCursor.setFillColor(sf::Color::Green);
        }

        hero->render(target);
        enemy->render(target);


        std::for_each(
            enemies.begin(),
            enemies.end(),
            std::bind(
                &Enemy::render,
                std::placeholders::_1,
                ReferenceWrapper<sf::RenderTarget>(target)
            )
        );



        std::for_each(
            currentRoom->getSpawners().begin(),
            currentRoom->getSpawners().end(),
            [&](const std::shared_ptr<Spawner>& spawner) {
                if(spawner) {
                    spawnerMarker.setPosition(spawner->getPosition().getX(), spawner->getPosition().getY());
                    spawnerMarker.setOrigin(sf::Vector2f(16.0f, 16.0f));
                    spawnerMarker.setSize(sf::Vector2f(16.0f, 16.0f));
                    target.draw(spawnerMarker);
                }
            }
        );

        std::for_each(
            currentRoom->getTransitions().begin(),
            currentRoom->getTransitions().end(),
            [&](const RoomTransition& transition) {
                auto x = static_cast<float>((transition.getX() + currentRoom->getX()) * map->getGridSize());
                auto y = static_cast<float>((transition.getY() + currentRoom->getY()) * map->getGridSize());
                auto width = static_cast<float>(transition.getWidth() * map->getGridSize());
                auto height = static_cast<float>(transition.getHeight() * map->getGridSize());

                transitionMarker.setPosition(x, y);
                transitionMarker.setSize(sf::Vector2f(width, height));
                target.draw(transitionMarker);
            }
        );


        std::for_each(
            enemies.begin(),
            enemies.end(),
            [&target](std::shared_ptr<Enemy>& en){
                en->render(target);
            }
        );


        if(item->isActive()) {
          item->render(target);
        }

        // Put old view back
        target.setView(oldView);

        font->renderText(target, StringUtils::toString<float>(cameraView.getCenter().x - (cameraView.getSize().x / 2)), 8, 24);


        /*if(cameraFollowingPlayer) {
            font->renderText(target, "FOLLOWING", 8, 64, sf::Color::White);
        } else if(transitioning) {
            font->renderText(target, "TRANSITIONING", 8, 64, sf::Color::White);
        }

        font->renderText(target, StringUtils::toString<float>(hero->getBoundingBox().getBottom()), 120, 64, sf::Color::White);
        font->renderText(target, StringUtils::toString<float>(hero->getVelocityY()), 120, 74, sf::Color::White);

        if(input->isDown(Input::BUTTON_RIGHT)) {
            font->renderText(target, "RIGHT", 180, 64, sf::Color::White);
        }

        if(input->isDown(Input::BUTTON_LEFT)) {
            font->renderText(target, "LEFT", 180, 64, sf::Color::White);
        }*/
    }

    bool MapTestState::update(const float &dt) {
        previousHeroPosition = currentHeroPosition;

        collisionResolver->setRoom(currentRoom);

        input->update();

        tiles->update(dt);

        // hero->setVelocityX(0);

        if(!transitioning) {
            velocity.x = 0.0f;
            velocity.y += 0.25f;
            velocity.y = std::max(velocity.y, -7.0f);
            velocity.y = std::min(velocity.y, 7.0f);

            retroVelocityX.setHigh(0);
            retroVelocityX.setLow(0);
            retroVelocityY -= retroGravity;
            retroVelocityY = std::max(retroVelocityY, NESNumber(-7, 0));
            retroVelocityY = std::min(retroVelocityY, NESNumber(7, 0));

            if(input->isDown(Input::BUTTON_LEFT)) {
                velocity.x = -1.296875f;
                //retroVelocityX = NESNumber::fromFloat(-1.296875f);
                retroVelocityX = -NESNumber(0x01, 0x4C);
                //sprite.FlipX(true);
                sprite.setScale(-1.0f, sprite.getScale().y);

                // movable->setVelocity(-1.296875f/* * 60.0f */, movable->getVelocity().getY());
                // hero->setVelocityX(-(NESNumber(0x01, 0x4C).toFloat())/* * 60.0f */);
            }

            if(input->isDown(Input::BUTTON_RIGHT)) {
                velocity.x = 1.296875f;
                //retroVelocityX = NESNumber::fromFloat(1.296875f);
                retroVelocityX = NESNumber(0x01, 0x4C);
                //sprite.FlipX(false);
                sprite.setScale(1.0f, sprite.getScale().y);

                // movable->setVelocity(1.296875f/* * 60.0f */, movable->getVelocity().getY());
                // hero->setVelocityX(NESNumber(0x01, 0x4C).toFloat()/* * 60.0f */);
            }

            auto playerPosition = world.getPlayerPosition();
            Sqrat::RootTable()
                .SetValue("heroX", playerPosition.getX())
                .SetValue("heroY", playerPosition.getY());

            item->update(dt);
            hero->update(dt);

            currentHeroPosition = hero->getPosition();

            std::for_each(
                std::begin(enemies),
                std::end(enemies),
                std::bind(
                    &GameObject::update,
                    std::placeholders::_1,
                    std::cref(dt)
                )
            );

            world.update(dt);

            enemy->update(dt);

            logicalCursor.move(velocity.x, (velocity.y));

            //
            // This adjusts the clipping of the tile renderer
            //
            if(cameraFollowingPlayer) {
                camera.lookAt(
                    (hero->getPosition().getX()),
                    (hero->getPosition().getY())
                );

            }
        }

        collidingWithTransition = false;

        //
        // TODO: This block needs to be methodized / refactored.
        //
        if(!transitioning) {
            // Check if the player contacts any of the transition regions.
            for(auto it = currentRoom->getTransitions().begin(),
                end = currentRoom->getTransitions().end();
                it < end;
                it++)
            {
                const RoomTransition& region = *it;

                // A region's position is relative to the room, so that has to be
                // taken in to account when checking intersection.
                int regionLeft = ((currentRoom->getX() + region.getX()) * 16);
                int regionTop = ((currentRoom->getY() + region.getY()) * 16);
                int regionWidth = region.getWidth() * 16;
                int regionHeight = region.getHeight() * 16;

                BoundingBox<float> transitionBounds(
                    static_cast<float>(regionLeft),
                    static_cast<float>(regionTop),
                    static_cast<float>(regionWidth),
                    static_cast<float>(regionHeight));

                //if(x >= regionLeft && x <= regionRight &&
                //    y >= regionTop && y <= regionBottom) {

                if(transitionBounds.contains(hero->getBoundingBox())) {
                    collidingWithTransition = true;

                    nextRoom = map->getRoom(region.getToRegion());

                    transitionDirection = region.getDirection();
                    transitionFrames = 0;

                    HIKARI_LOG(debug) << "Transitioning from room " << currentRoom->getId() << " to room " << region.getToRegion();
                    break;
                }
            }
        }

        // Handle the transition motion
        if(collidingWithTransition && !transitioning) {
            transitionBegin();
        }

        if(transitioning) {
            transitionUpdate(dt);
        }

        const auto cameraView = camera.getView();
        const auto cameraX  = static_cast<int>(cameraView.getX());
        const auto cameraWidth = static_cast<int>(cameraView.getWidth());
        const auto cameraY   = static_cast<int>(cameraView.getY());
        const auto cameraHeight = static_cast<int>(cameraView.getHeight());

        renderer.setCullRegion(Rectangle2D<int>(cameraX, cameraY, cameraWidth, cameraHeight));

        //
        // Set position of running megaman sprite
        //
        sf::Vector2f tempPos = logicalCursor.getPosition();
        tempPos.x = std::floor(tempPos.x + 16.0f);
        tempPos.y = std::floor(tempPos.y + 22.0f + 1.0f); // + 1 puts rock's feet on the first ground pixel

        //sprite.setPosition(tempPos);
        //animationPlayer.update(dt);

        // Check to see if we collide with item and apply its effect if we do.
        if(hero->getBoundingBox().intersects(item->getBoundingBox())) {
            // Have to check if the item is active otherwise the effect could
            // be applied more that one time.
            if(item->isActive()) {
                if(auto itemEffect = item->getEffect()) {
                    itemEffect->apply();
                }

                item->onDeath();
            }
        }

        return false;
    }

    void MapTestState::onEnter() {
        Movable::setGravity(0.25f/* * 60.0f */);
        Movable::setCollisionResolver(collisionResolver);

        BoundingBoxF playerBounds = BoundingBoxF(80.0f, 40.0f, 14.0f, 22.0f);
        playerBounds.setOrigin(7.0f, 5.0f);
        movable->setBoundingBox(playerBounds);

        hero->setRoom(currentRoom);
        hero->setPosition(static_cast<float>((currentRoom->getX() * 16) + 40), static_cast<float>((currentRoom->getY() * 16) + 30));
        //renderedCursor = sf::Shape::Rectangle(0.0f, 0.0f, 14.0f, 22.0f, sf::Color(0, 255, 255, 128));

        currentHeroPosition = hero->getPosition();
        diffPosition.setX(0.0f).setY(0.0f);
    }

    void MapTestState::onExit() {

    }

    const std::string& MapTestState::getName() const {
        return name;
    }

    void MapTestState::transitionBegin() {
        transitioning = true;
        cameraFollowingPlayer = false;

        switch(transitionDirection) {
            case RoomTransition::DirectionUp:
                // Find the bottom of the next room, subtract the camera's height
                camera.lockVertical(false);
                transitionEndY = static_cast<float>(nextRoom->getCameraBounds().getBottom() - camera.getView().getHeight());
                // logicalCursor.move(0.0f, -1.0f);
                //hero->move(0.0f, -1.0f);
                break;
            case RoomTransition::DirectionForward:
                camera.lockHorizontal(false);
                transitionEndX = static_cast<float>(nextRoom->getCameraBounds().getLeft());
                //logicalCursor.move(1.0f, 0.0f);
                break;
            case RoomTransition::DirectionDown:
                camera.lockVertical(false);
                transitionEndY = static_cast<float>(nextRoom->getCameraBounds().getTop());
                //logicalCursor.move(0.0f, 1.0f);
                break;
            case RoomTransition::DirectionBackward:
                // Find the right side of the room, subtract the camera's width
                camera.lockHorizontal(false);
                transitionEndX = static_cast<float>(nextRoom->getCameraBounds().getRight() - camera.getView().getWidth());
                //logicalCursor.move(-1.0f, 0.0f);
                break;
            default:
                break;
        }
    }

    void MapTestState::transitionUpdate(float dt) {
        float camY = camera.getY();
        float camX = camera.getX();

        switch(transitionDirection) {
            case RoomTransition::DirectionUp:
                if(camY > transitionEndY) {
                    camera.move(0.0f, -transitionSpeedY * dt);
                    transitionFrames++;

                    auto heroTranslateY = -heroTranslationSpeedY  * dt;
                    hero->setPosition(hero->getPosition().getX(), hero->getPosition().getY() + heroTranslateY);
                    hero->getAnimationPlayer()->update(dt);
                } else {
                    transitionEnd();
                }
                break;
            case RoomTransition::DirectionForward:
                camera.lockHorizontal(false);
                if(camX < transitionEndX) {
                    camera.move(transitionSpeedX * dt, 0.0f);
                    transitionFrames++;

                    auto heroTranslateX = heroTranslationSpeedX  * dt;
                    hero->setPosition(hero->getPosition().getX() + heroTranslateX, hero->getPosition().getY());
                    hero->getAnimationPlayer()->update(dt);
                } else {
                    transitionEnd();
                }
                break;
            case RoomTransition::DirectionDown:
                camera.lockVertical(false);
                if(camY < transitionEndY) {
                    camera.move(0.0f, transitionSpeedY * dt);
                    transitionFrames++;

                    auto heroTranslateY = heroTranslationSpeedY  * dt;
                    hero->setPosition(hero->getPosition().getX(), hero->getPosition().getY() + heroTranslateY);
                    hero->getAnimationPlayer()->update(dt);
                } else {
                    transitionEnd();
                }
                break;
            case RoomTransition::DirectionBackward:
                camera.lockHorizontal(false);
                if(camX > transitionEndX) {
                    camera.move(-transitionSpeedX * dt, 0.0f);
                    transitionFrames++;

                    auto heroTranslateX = -heroTranslationSpeedX  * dt;
                    hero->setPosition(hero->getPosition().getX() + heroTranslateX, hero->getPosition().getY());
                    hero->getAnimationPlayer()->update(dt);
                } else {
                    transitionEnd();
                }
                break;
            default:
                transitionEnd();
                break;
        }
    }

    void MapTestState::transitionEnd() {
        transitioning = false;
        cameraFollowingPlayer = true;

        camera.lockVertical(true);
        camera.lockHorizontal(true);

        currentRoom.swap(nextRoom);
        //camera.setBoundary(map->getRoomRect(currentRoom->getId()));
        camera.setBoundary(currentRoom->getCameraBounds());
        hero->setRoom(currentRoom);
    }

    Vector2<float> MapTestState::screenCoordToWorldCoord(const float & x, const float & y) {
        if(renderWindow) {
            auto convertedCoords = renderWindow->mapPixelToCoords(sf::Vector2i(static_cast<int>(x), static_cast<int>(y)), camera.getPixelAlignedView());
            return Vector2<float>(convertedCoords.x, convertedCoords.y);
        } else {
            return Vector2<float>(0.0f, 0.0f);
        }
    }

    void MapTestState::setupHero() {
        auto heroAnimationSet = animationSetCache->get("assets/animations/rockman-32.json");
        auto spriteSheet = imageCache->get(heroAnimationSet->getImageFileName());

        spriteSheet->setSmooth(false);

        hero->setActive(true);
        hero->setAnimationSet(heroAnimationSet);
        hero->setSpriteTexture(spriteSheet);
        hero->setBoundingBox(BoundingBoxF(0, 0, 16, 24).setOrigin(8, 20));
        hero->changeAnimation("idle");
        hero->setPosition(100.0f, 100.0f);
        hero->setActionController(std::make_shared<PlayerInputHeroActionController>(input));
    }

    void MapTestState::setupEnemy() {
        /*
        auto enemyAnimationSet = animationSetCache->get("assets/animations/enemies.json");
        auto enemySprites = imageCache->get(enemyAnimationSet->getImageFileName());

        enemySprites->setSmooth(false);

        enemy->setActive(false);
        enemy->setAnimationSet(enemyAnimationSet);
        enemy->setSpriteTexture(enemySprites);
        enemy->setBoundingBox(BoundingBoxF(0, 0, 16, 16).setOrigin(8, 8));
        enemy->setCurrentAnimation(enemyAnimationSet->get("idle"));
        enemy->setPosition(-255.0f, 255.0f);
        enemy->setGravitated(false);

        auto enemyBrain = std::make_shared<EnemyBrain>();
        enemy->setBrain(enemyBrain);
        enemy->setDirection(Directions::Down);
        */
    }

    std::shared_ptr<Enemy> MapTestState::spawnEnemy(const std::string& type) { 
        auto instance = std::make_shared<Enemy>(GameObject::generateObjectId(), nullptr);

        if(type == "scripted-telly") {

            auto enemyAnimationSet = animationSetCache->get("assets/animations/telly.json");
            auto enemySprites = imageCache->get(enemyAnimationSet->getImageFileName());

            enemySprites->setSmooth(false);

            instance->setActive(true);
            instance->setAnimationSet(enemyAnimationSet);
            instance->setSpriteTexture(enemySprites);
            instance->setBoundingBox(BoundingBoxF(0, 0, 14, 14).setOrigin(7, 7));
            instance->setCurrentAnimation(enemyAnimationSet->get("idle"));
            instance->setGravitated(false);
            instance->setPhasing(false);

            auto enemyBrain = std::make_shared<ScriptedEnemyBrain>(*squirrel, "TellyEnemyBehavior");
            instance->setBrain(enemyBrain);
            instance->setDirection(Directions::Down);

        } else if(type == "scripted-octopusbattery") {
            auto enemyAnimationSet = animationSetCache->get("assets/animations/enemies.json");
            auto enemySprites = imageCache->get(enemyAnimationSet->getImageFileName());

            enemySprites->setSmooth(false);

            instance->setActive(true);
            instance->setAnimationSet(enemyAnimationSet);
            instance->setSpriteTexture(enemySprites);
            instance->setBoundingBox(BoundingBoxF(0, 0, 14, 14).setOrigin(7, 7));
            instance->setCurrentAnimation(enemyAnimationSet->get("idle"));
            instance->setGravitated(false);

            auto enemyBrain = std::make_shared<ScriptedEnemyBrain>(*squirrel, "OctopusBatteryEnemyBehavior");
            instance->setBrain(enemyBrain);
            instance->setDirection(Directions::Down);
        }

        return instance;
    }

    void MapTestState::setupItem() {

    }

    void MapTestState::setRenderWindow(sf::RenderWindow * window) {
        renderWindow = window;
    }

} // hikari
