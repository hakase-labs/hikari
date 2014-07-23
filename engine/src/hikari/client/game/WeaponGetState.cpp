#include "hikari/client/game/WeaponGetState.hpp"
#include "hikari/client/game/InputService.hpp"
#include "hikari/client/audio/AudioService.hpp"
#include "hikari/client/game/GameProgress.hpp"
#include "hikari/client/game/Task.hpp"
#include "hikari/client/game/FunctionTask.hpp"
#include "hikari/client/game/WaitTask.hpp"
#include "hikari/client/game/GameConfig.hpp"
#include "hikari/client/gui/Panel.hpp"
#include "hikari/client/gui/GuiService.hpp"
#include "hikari/client/gui/Icon.hpp"
#include "hikari/client/gui/IconAnimator.hpp"
#include "hikari/client/Services.hpp"

#include "hikari/core/game/GameController.hpp"
#include "hikari/core/util/ServiceLocator.hpp"
#include "hikari/core/util/Log.hpp"

#include <guichan/gui.hpp>
#include <guichan/widgets/label.hpp>
#include <guichan/widgets/container.hpp>
#include <guichan/widgets/label.hpp>
#include <guichan/widgets/icon.hpp>
#include <guichan/hakase/labelex.hpp>

#include <SFML/Window/Event.hpp>

namespace hikari {

    WeaponGetState::WeaponGetState(const std::string & name, GameController & controller, const std::weak_ptr<GameConfig> & gameConfig, ServiceLocator &services)
        : name(name)
        , controller(controller)
        , gameConfig(gameConfig)
        , guiService(services.locateService<GuiService>(Services::GUISERVICE))
        , audioService(services.locateService<AudioService>(Services::AUDIO))
        , gameProgress(services.locateService<GameProgress>(Services::GAMEPROGRESS))
        , keyboardInput(services.locateService<InputService>(Services::INPUT))
        , goToNextState(false)
    {
        buildGui(services);
    }

    WeaponGetState::~WeaponGetState() {

    }

    void WeaponGetState::buildGui(ServiceLocator & services) {
        guiContainer.reset(new gcn::Container());
        guiContainer->setSize(256, 240);
        guiContainer->setBaseColor(0x000000);
        guiContainer->setOpaque(true);
        guiContainer->setVisible(true);

        guiBackground.reset(new gui::Icon("assets/images/bg-weapon-get.png"));
        guiBackground->setOpaque(false);
        guiBackground->setSize(guiContainer->getWidth(), guiContainer->getHeight());
        guiBackground->setSubrectangle(gcn::Rectangle(0, 0, guiContainer->getWidth(), guiContainer->getHeight()));
        guiContainer->add(guiBackground.get(), 0, 0);

        guiYouGotText.reset(new gcn::LabelEx("YOU GOT"));
        guiYouGotText->setAlignment(gcn::Graphics::Center);
        guiYouGotText->setSize(182, 8); // We don't shrink-wrap here since we want to center-align.
        guiContainer->add(guiYouGotText.get(), 0, 96);

        guiWeaponGetText.reset(new gcn::LabelEx("[WEAPON NAME]"));
        guiWeaponGetText->setAlignment(gcn::Graphics::Center);
        guiWeaponGetText->setSize(182, 8); // We don't shrink-wrap here since we want to center-align.
        guiContainer->add(guiWeaponGetText.get(), 0, 120);

        guiRockman.reset(new gui::Icon("assets/images/bg-weapon-get.png"));
        guiRockman->setSize(16, 80);
        guiRockman->setSubrectangle(gcn::Rectangle(256, 0, guiRockman->getWidth(), guiRockman->getHeight()));
        guiRockman->setVisible(false);
        guiContainer->add(guiRockman.get(), 120, 188);
    }

    void WeaponGetState::handleEvent(sf::Event &event) {
        if(event.type == sf::Event::KeyPressed) {
            if(event.key.code == sf::Keyboard::Return) {
                controller.requestStateChange("stageselect");
                goToNextState = true;
            }
        }
    }

    void WeaponGetState::render(sf::RenderTarget &target) {
        if(auto gui = guiService.lock()) {
            gui->renderAsTop(guiContainer.get(), target);
        }
    }

    bool WeaponGetState::update(float dt) {
        if(keyboardInput->wasPressed(Input::BUTTON_CANCEL)) {
            controller.requestStateChange("password");
            // TODO: Check if enough time has elapsed to show the sequence before
            // skipping to stage select.
            goToNextState = true;
        }

        if(!taskQueue.empty()) {
            auto & task = taskQueue.front();
            task->update(dt);

            if(task->isComplete()) {
                taskQueue.pop();
            }
        } else {

        }

        return goToNextState;
    }

    void WeaponGetState::onEnter() {
        goToNextState = false;

        // Determine the weapon name to display.
        if(auto config = gameConfig.lock()) {
            if(auto gp = gameProgress.lock()) {
                const unsigned int currentBossIndex = gp->getCurrentBoss();
                const auto & weaponNames = config->getHeroWeaponNames();
                guiWeaponGetText->setCaption(weaponNames.at(currentBossIndex + 1)); // Weapons[0] is the Mega Buster
            }
        }

        // Push this state's GUI into the GUI container.
        if(auto gui = guiService.lock()) {
            auto & topContainer = gui->getRootContainer();
            topContainer.add(guiContainer.get(), 0, 0);
            guiContainer->setEnabled(true);
            guiContainer->requestFocus();
        }

        if(auto audio = audioService.lock()) {
            audio->playMusic("Weapon Get (MM3)");
        }

        // Hide the text.
        guiYouGotText->setVisible(false);
        guiWeaponGetText->setVisible(false);

        // Position the teleport sprite in the right spot.
        guiRockman->setSize(16, 80);
        guiRockman->setPosition(120, 188);
        guiRockman->setSubrectangle(gcn::Rectangle(256, 0, guiRockman->getWidth(), guiRockman->getHeight()));
        guiRockman->setVisible(false);

        // Set up GUI tasks to move the sprites around.
        taskQueue.push(std::make_shared<WaitTask>((1.0f / 60.0f) * 4.0f));

        // Position the teleport sprite at the bottom and make it visible.
        taskQueue.push(std::make_shared<FunctionTask>(0, [&](float dt) {
            guiRockman->setSize(16, 80);
            guiRockman->setSubrectangle(gcn::Rectangle(256, 0, guiRockman->getWidth(), guiRockman->getHeight()));
            guiRockman->setVisible(true);

            return true;
        }));

        // Continue moving the sprite up to it's destination.
        taskQueue.push(std::make_shared<FunctionTask>(0, [&](float dt) {
            if(guiRockman->getY() > 76) {
                guiRockman->setPosition(guiRockman->getX(), guiRockman->getY() - 4); // 4px per frame for 28 frames
            } else {
                return true;
            }

            return false;
        }));

        // Change to sideways sprite.
        taskQueue.push(std::make_shared<FunctionTask>(0, [&](float dt) {
            guiRockman->setSize(64, 32);
            guiRockman->setSubrectangle(gcn::Rectangle(256, 80, guiRockman->getWidth(), guiRockman->getHeight()));
            guiRockman->setPosition(96, 100);

            return true;
        }));

        // Wait 2 more frames (3 frames of this sprite total)
        taskQueue.push(std::make_shared<WaitTask>((1.0f / 60.0f) * 2.0f));

        // Change to blob sprite. Takes 1 frame.
        taskQueue.push(std::make_shared<FunctionTask>(0, [&](float dt) {
            guiRockman->setSize(48, 32);
            guiRockman->setSubrectangle(gcn::Rectangle(272, 80, guiRockman->getWidth(), guiRockman->getHeight()));
            guiRockman->setPosition(96, 100);

            return true;
        }));

        // Change to Rockman sprite. Takes 1 frame.
        taskQueue.push(std::make_shared<FunctionTask>(0, [&](float dt) {
            guiRockman->setSize(56, 96);
            guiRockman->setSubrectangle(gcn::Rectangle(256, 112, guiRockman->getWidth(), guiRockman->getHeight()));
            guiRockman->setPosition(104, 72);

            return true;
        }));

        // Wait 1 whole second.
        taskQueue.push(std::make_shared<WaitTask>(1.0f));

        // Move Rockman sprite to the right until it gets into the right spot.
        taskQueue.push(std::make_shared<FunctionTask>(0, [&](float dt) {
            if(guiRockman->getX() < 168) {
                guiRockman->setPosition(guiRockman->getX() + 2, guiRockman->getY()); // 2px per frame
            } else {
                return true;
            }

            return false;
        }));

        // Wait 64 frames.
        taskQueue.push(std::make_shared<WaitTask>((1.0f / 60.0f) * 64.0f));

        // Start fading in the text.
        taskQueue.push(std::make_shared<FunctionTask>(0, [&](float dt) {
            guiYouGotText->setVisible(true);

            return true;
        }));

        taskQueue.push(std::make_shared<WaitTask>((1.0f / 60.0f) * 38.0f));

        taskQueue.push(std::make_shared<FunctionTask>(0, [&](float dt) {
            guiWeaponGetText->setVisible(true);

            return true;
        }));

        // goToNextState = false;
    }

    void WeaponGetState::onExit() {
        if(auto gui = guiService.lock()) {
            auto & topContainer = gui->getRootContainer();
            topContainer.remove(guiContainer.get());
            guiContainer->setEnabled(false);
        }

        // Clear any tasks that could be left in the queue.
        while(!taskQueue.empty()) {
            taskQueue.pop();
        }

        // Hide the text.
        guiYouGotText->setVisible(false);
        guiWeaponGetText->setVisible(false);

        // if(auto audio = audioService.lock()) {
        //     audio->stopMusic();
        // }
    }

    const std::string & WeaponGetState::getName() const {
        return name;
    }

} // hikari
