#include "hikari/client/game/WeaponGetState.hpp"
#include "hikari/client/game/InputService.hpp"
#include "hikari/client/audio/AudioService.hpp"
#include "hikari/client/game/GameProgress.hpp"
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

namespace hikari {

    WeaponGetState::WeaponGetState(const std::string & name, GameController & controller, ServiceLocator &services)
        : name(name)
        , controller(controller)
        , guiService(services.locateService<GuiService>(Services::GUISERVICE))
        , audioService(services.locateService<AudioService>(Services::AUDIO))
        , gameProgress(services.locateService<GameProgress>(Services::GAMEPROGRESS))
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

        guiWeaponGetText.reset(new gcn::LabelEx("WEAPON GET"));
        guiWeaponGetText->adjustSize();
        guiContainer->add(guiWeaponGetText.get(), 100, 100);
    }

    void WeaponGetState::handleEvent(sf::Event &event) {

    }

    void WeaponGetState::render(sf::RenderTarget &target) {
        if(auto gui = guiService.lock()) {
            gui->renderAsTop(guiContainer.get(), target);
        }
    }

    bool WeaponGetState::update(float dt) {
        bool goToNextState = false;

        // if(keyboardInput->wasPressed(Input::BUTTON_CANCEL)) {
        //     controller.requestStateChange(controller.getPreviousStateName());
        //     goToNextState = true;
        // }

        return goToNextState;
    }

    void WeaponGetState::onEnter() {
        if(auto gui = guiService.lock()) {
            auto & topContainer = gui->getRootContainer();
            topContainer.add(guiContainer.get(), 0, 0);
            guiContainer->setEnabled(true);
        }

        if(auto audio = audioService.lock()) {
            audio->playMusic("Weapon Get (MM3)");
        }

        // goToNextState = false;
    }

    void WeaponGetState::onExit() {
        if(auto gui = guiService.lock()) {
            auto & topContainer = gui->getRootContainer();
            topContainer.remove(guiContainer.get());
            guiContainer->setEnabled(false);
        }

        // if(auto audio = audioService.lock()) {
        //     audio->stopMusic();
        // }
    }

    const std::string & WeaponGetState::getName() const {
        return name;
    }

} // hikari
