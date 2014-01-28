#include "hikari/client/game/GameOverState.hpp"
#include "hikari/client/game/InputService.hpp"
#include "hikari/client/audio/AudioService.hpp"
#include "hikari/client/gui/Panel.hpp"
#include "hikari/client/gui/GuiService.hpp"
#include "hikari/client/Services.hpp"
#include "hikari/core/game/GameController.hpp"
#include "hikari/core/util/ServiceLocator.hpp"

#include <guichan/gui.hpp>
#include <guichan/widgets/label.hpp>
#include <json/reader.h>

namespace hikari {

    GameOverState::GameOverState(const std::string &name, const Json::Value &params, GameController & controller, ServiceLocator &services)
        : name(name)
        , controller(controller)
        , audioService(services.locateService<AudioService>(Services::AUDIO))
        , keyboardInput(services.locateService<InputService>(Services::INPUT))
        , mainPanel(new gui::Panel())
        , guiWrapper(new gcn::Container())
        , testLabel(new gcn::Label())
        , guiService(services.locateService<GuiService>(Services::GUISERVICE))
        , goToNextState(false)
    {
        guiWrapper->setWidth(256);
        guiWrapper->setHeight(240);
        guiWrapper->setOpaque(true);
        guiWrapper->setBaseColor(gcn::Color(12, 56, 130));

        testLabel->setCaption("Game Over");
        testLabel->adjustSize();

        mainPanel->setWidth(100);
        mainPanel->setHeight(100);

        guiWrapper->add(testLabel.get(), 100, 4);
        guiWrapper->add(mainPanel.get(), 10, 20);
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