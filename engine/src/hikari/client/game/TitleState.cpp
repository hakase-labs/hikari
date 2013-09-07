#include "hikari/client/game/TitleState.hpp"
#include "hikari/client/game/InputService.hpp"
#include "hikari/client/game/EventBusService.hpp"
#include "hikari/client/game/events/GameQuitEventData.hpp"
#include "hikari/client/audio/AudioService.hpp"
#include "hikari/client/gui/GuiService.hpp"
#include "hikari/client/gui/Menu.hpp"
#include "hikari/client/gui/MenuItem.hpp"
#include "hikari/client/gui/Icon.hpp"
#include "hikari/client/Services.hpp"

#include "hikari/core/game/GameController.hpp"
#include "hikari/core/util/ServiceLocator.hpp"
#include "hikari/core/util/Log.hpp"

#include <guichan/gui.hpp>
#include <guichan/widgets/container.hpp>
#include <guichan/widgets/label.hpp>

#include <guichan/hakase/functoractionlistener.hpp>
#include <guichan/hakase/functorselectionlistener.hpp>

#include <json/value.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

namespace hikari {

    const std::string TitleState::ITEM_GAME_START = "GAME START";
    const std::string TitleState::ITEM_PASS_WORD = "PASS WORD";
    const std::string TitleState::ITEM_OPTIONS = "OPTIONS";
    const std::string TitleState::ITEM_QUIT = "QUIT";

    TitleState::TitleState(const std::string &name, const Json::Value &params, GameController & controller, ServiceLocator &services)
        : GameState()
        , name(name)
        , controller(controller)
        , guiService(services.locateService<GuiService>(Services::GUISERVICE))
        , audioService(services.locateService<AudioService>(Services::AUDIO))
        , globalEventBus(services.locateService<EventBusService>(Services::EVENTBUS))
        , keyboardInput(services.locateService<InputService>(Services::INPUT))
        , guiContainer(new gcn::Container())
        , guiLabel(new gcn::Label())
        , guiMenu(new gui::Menu())
        , guiIcon(new gui::Icon("assets/images/gui-title-logo.png"))
        , guiCursorIcon(new gui::Icon("assets/images/sp-gui-atlas.png"))
        , guiActionListener(nullptr)
        , guiSelectionListener(nullptr)
        , goToNextState(false)
    {
        buildGui();
    }

    TitleState::~TitleState() {

    }

    void TitleState::buildGui() {
        guiCursorIcon->setSubrectangle(gcn::Rectangle(0, 0, 4, 8));
        guiCursorIcon->setWidth(4);
        guiCursorIcon->setHeight(8);

        guiActionListener.reset(new gcn::FunctorActionListener([&](const gcn::ActionEvent& event) {
            auto item = guiMenu->getMenuItemAt(guiMenu->getSelectedIndex());

            if(item) {
                std::cout << "Actioned on #" << guiMenu->getSelectedIndex() << ", " << item->getName() << std::endl;

                const std::string & menuItemName = item->getName();

                if(menuItemName == ITEM_GAME_START) {
                    controller.requestStateChange("stageselect");
                    goToNextState = true;
                } else if(menuItemName == ITEM_PASS_WORD) {
                    controller.requestStateChange("password");
                    goToNextState = true;
                } else if(menuItemName == ITEM_OPTIONS) {
                    //controller.requestStateChange("options");
                    controller.requestStateChange("options");
                    goToNextState = true;
                } else if(menuItemName == ITEM_QUIT) {
                    if(auto events = globalEventBus.lock()) {
                        events->triggerEvent(EventDataPtr(new GameQuitEventData(GameQuitEventData::QUIT_NOW)));
                    }
                }
            } else {
                std::cout << "Actioned on #" << guiMenu->getSelectedIndex() << std::endl;
            }
        }));

        guiSelectionListener.reset(new gcn::FunctorSelectionListener([&](const gcn::SelectionEvent & event) {
            std::cout << "Selection changed! " << guiMenu->getSelectedIndex() << std::endl;

            positionCursorOnItem();

            if(auto audio = audioService.lock()) {
                audio->playSample(27);
            }
        }));

        guiCursorIcon->setX(8);
        guiCursorIcon->setY(8);

        guiContainer->setBaseColor(gcn::Color(0, 0, 0));
        guiContainer->setOpaque(true);
        guiContainer->setWidth(256);
        guiContainer->setHeight(240);

        guiIcon->setWidth(182);
        guiIcon->setHeight(81);
        guiIcon->setX((guiContainer->getWidth() / 2) - (guiIcon->getWidth() / 2));
        guiIcon->setY(40);

        guiContainer->add(guiIcon.get());

        guiLabel->setCaption("UNRELEASED v0.0.1");
        guiLabel->adjustSize();
        guiLabel->setX(8);
        guiLabel->setY(224);

        guiMenu->setWidth(guiContainer->getWidth() - 32);
        guiMenu->setHeight((guiContainer->getHeight() / 2) - 32);
        guiMenu->setBackgroundColor(gcn::Color(0, 0, 0, 0));
        guiMenu->addActionListener(guiActionListener.get());
        guiMenu->enableWrapping();

        std::shared_ptr<gui::MenuItem> gameStartMenuItem(new gui::MenuItem(ITEM_GAME_START));
        gameStartMenuItem->setForegroundColor(gcn::Color(0, 0, 0, 0));
        gameStartMenuItem->setSelectionColor(gcn::Color(0, 0, 0, 0));
        guiMenu->addItem(gameStartMenuItem);

        std::shared_ptr<gui::MenuItem> passWordMenuItem(new gui::MenuItem(ITEM_PASS_WORD));
        passWordMenuItem->setY(16);
        passWordMenuItem->setForegroundColor(gcn::Color(0, 0, 0, 0));
        passWordMenuItem->setSelectionColor(gcn::Color(0, 0, 0, 0));
        guiMenu->addItem(passWordMenuItem);

        std::shared_ptr<gui::MenuItem> optionsMenuItem(new gui::MenuItem(ITEM_OPTIONS));
        optionsMenuItem->setY(32);
        optionsMenuItem->setForegroundColor(gcn::Color(0, 0, 0, 0));
        optionsMenuItem->setSelectionColor(gcn::Color(0, 0, 0, 0));
        guiMenu->addItem(optionsMenuItem);

        std::shared_ptr<gui::MenuItem> quitMenuItem(new gui::MenuItem(ITEM_QUIT));
        quitMenuItem->setY(48);
        quitMenuItem->setForegroundColor(gcn::Color(0, 0, 0, 0));
        quitMenuItem->setSelectionColor(gcn::Color(0, 0, 0, 0));
        guiMenu->addItem(quitMenuItem);

        guiMenu->setSelectedIndex(0);

        guiContainer->add(guiMenu.get(), 48, guiIcon->getHeight() + guiIcon->getY() + 16);
        guiContainer->add(guiLabel.get());
        guiContainer->add(guiCursorIcon.get());
        
        // Set the initial position of the cursor
        positionCursorOnItem();
    }

    void TitleState::positionCursorOnItem() {
        int itemIndex = guiMenu->getSelectedIndex();
        std::shared_ptr<gui::MenuItem> menuItem = guiMenu->getMenuItemAt(itemIndex);

        int spacing = 2;
        int absX = 0;
        int absY = 0;

        if(menuItem) {
            menuItem->getAbsolutePosition(absX, absY);
            guiCursorIcon->setX(absX - guiCursorIcon->getWidth() - spacing);
            guiCursorIcon->setY(absY);
        }
    }

    void TitleState::handleEvent(sf::Event &event) {

    }

    void TitleState::render(sf::RenderTarget &target) {
        if(auto gui = guiService.lock()) {
            gui->renderRootContainer(target);
        }
    }

    bool TitleState::update(const float &dt) {
        guiMenu->logic();
        return goToNextState;
    }

    void TitleState::onEnter() {
        // Attach our GUI
        if(auto gui = guiService.lock()) {
            auto & topContainer = gui->getRootContainer();

            topContainer.add(guiContainer.get(), 0, 0);
            guiMenu->setEnabled(true);
            guiMenu->requestFocus();
            guiMenu->setSelectedIndex(0);
        }

        positionCursorOnItem();

        if(auto audio = audioService.lock()) {
            audio->playMusic(0);
        }

        goToNextState = false;
        guiMenu->addSelectionListener(guiSelectionListener.get());
    }

    void TitleState::onExit() {
        // Remove our GUI
        if(auto gui = guiService.lock()) {
            auto & topContainer = gui->getRootContainer();

            topContainer.remove(guiContainer.get());
            guiMenu->setEnabled(false);
        }

        if(auto audio = audioService.lock()) {
            audio->stopMusic();
        }

        guiMenu->removeSelectionListener(guiSelectionListener.get());
    }

    const std::string & TitleState::getName() const {
        return name;
    }

}// hikari