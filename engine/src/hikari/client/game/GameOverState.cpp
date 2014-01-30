#include "hikari/client/game/GameOverState.hpp"
#include "hikari/client/game/InputService.hpp"
#include "hikari/client/audio/AudioService.hpp"
#include "hikari/client/gui/Panel.hpp"
#include "hikari/client/gui/Menu.hpp"
#include "hikari/client/gui/MenuItem.hpp"
#include "hikari/client/gui/Icon.hpp"
#include "hikari/client/gui/GuiService.hpp"
#include "hikari/client/Services.hpp"
#include "hikari/core/game/GameController.hpp"
#include "hikari/core/util/ServiceLocator.hpp"

#include <guichan/gui.hpp>
#include <guichan/widgets/label.hpp>
#include <json/reader.h>

#include <memory>

namespace hikari {

    const std::string GameOverState::ITEM_CONTINUE = "CONTINUE";
    const std::string GameOverState::ITEM_PASS_WORD = "PASS WORD";
    const std::string GameOverState::ITEM_STAGE_SELECT = "STAGE SELECT";
    const std::string GameOverState::ITEM_TITLE_SCREEN = "TITLE SCREEN";

    GameOverState::GameOverState(const std::string &name, const Json::Value &params, GameController & controller, ServiceLocator &services)
        : name(name)
        , controller(controller)
        , audioService(services.locateService<AudioService>(Services::AUDIO))
        , keyboardInput(services.locateService<InputService>(Services::INPUT))
        , mainPanel(new gui::Panel())
        , guiWrapper(new gcn::Container())
        , guiMenu(new gui::Menu())
        , guiCursorIcon(new gui::Icon("assets/images/sp-gui-atlas.png"))
        , gameOverLabel(new gcn::Label())
        , guiService(services.locateService<GuiService>(Services::GUISERVICE))
        , goToNextState(false)
    {
        guiCursorIcon->setSubrectangle(gcn::Rectangle(0, 0, 4, 8));
        guiCursorIcon->setWidth(4);
        guiCursorIcon->setHeight(8);

        guiWrapper->setWidth(256);
        guiWrapper->setHeight(240);
        guiWrapper->setOpaque(true);
        guiWrapper->setBaseColor(gcn::Color(0, 112, 236));

        gameOverLabel->setCaption("Game Over");
        gameOverLabel->adjustSize();

        std::shared_ptr<gui::MenuItem> continueMenuItem(new gui::MenuItem(ITEM_CONTINUE));
        continueMenuItem->setForegroundColor(gcn::Color(0, 0, 0, 0));
        continueMenuItem->setSelectionColor(gcn::Color(0, 0, 0, 0));
        guiMenu->addItem(continueMenuItem);

        std::shared_ptr<gui::MenuItem> passWordMenuItem(new gui::MenuItem(ITEM_PASS_WORD));
        passWordMenuItem->setY(16);
        passWordMenuItem->setForegroundColor(gcn::Color(0, 0, 0, 0));
        passWordMenuItem->setSelectionColor(gcn::Color(0, 0, 0, 0));
        guiMenu->addItem(passWordMenuItem);

        std::shared_ptr<gui::MenuItem> stageSelectMenuItem(new gui::MenuItem(ITEM_STAGE_SELECT));
        stageSelectMenuItem->setY(32);
        stageSelectMenuItem->setForegroundColor(gcn::Color(0, 0, 0, 0));
        stageSelectMenuItem->setSelectionColor(gcn::Color(0, 0, 0, 0));
        guiMenu->addItem(stageSelectMenuItem);

        std::shared_ptr<gui::MenuItem> titleScreenMenuItem(new gui::MenuItem(ITEM_TITLE_SCREEN));
        titleScreenMenuItem->setY(48);
        titleScreenMenuItem->setForegroundColor(gcn::Color(0, 0, 0, 0));
        titleScreenMenuItem->setSelectionColor(gcn::Color(0, 0, 0, 0));
        guiMenu->addItem(titleScreenMenuItem);

        guiMenu->setSelectedIndex(0);

        mainPanel->setWidth(guiWrapper->getWidth() - 32);
        mainPanel->setHeight(guiWrapper->getHeight() - 32);
        mainPanel->setBaseColor(gcn::Color(0, 0, 0));

        guiMenu->setWidth(mainPanel->getWidth() - 24);
        guiMenu->setHeight(mainPanel->getHeight() - 16);
        guiMenu->setBackgroundColor(gcn::Color(0, 0, 0, 0));
        // guiMenu->addActionListener(guiActionListener.get());
        guiMenu->enableWrapping();

        guiWrapper->add(gameOverLabel.get(), 100, 4);
        guiWrapper->add(mainPanel.get(), 16, 16);
        mainPanel->add(guiMenu.get(), 24, 16);
    }

    GameOverState::~GameOverState() {

    }

    void GameOverState::handleEvent(sf::Event &event) {

    }

    void GameOverState::render(sf::RenderTarget &target) {
        if(auto gui = guiService.lock()) {
            gui->renderAsTop(guiWrapper.get(), target);
        }
    }

    bool GameOverState::update(float dt) {
        if(keyboardInput->wasPressed(Input::BUTTON_CANCEL)) {
            controller.requestStateChange("stageselect");
            goToNextState = true;
        }

        return goToNextState;
    }

    void GameOverState::onEnter() {
        if(auto gui = guiService.lock()) {
            auto & topContainer = gui->getRootContainer();
            topContainer.add(guiWrapper.get(), 0, 0);
            guiWrapper->setEnabled(true);
        }

        if(auto audio = audioService.lock()) {
            audio->playMusic("Password (MM3)");
        }

        goToNextState = false;
    }

    void GameOverState::onExit() {
        if(auto gui = guiService.lock()) {
            auto & topContainer = gui->getRootContainer();
            topContainer.remove(guiWrapper.get());
            guiWrapper->setEnabled(false);
        }
        
        if(auto audio = audioService.lock()) {
            audio->stopMusic();
        }
    }

    const std::string & GameOverState::getName() const {
        return name;
    }

} // hikari