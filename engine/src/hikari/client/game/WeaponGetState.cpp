#include "hikari/client/game/WeaponGetState.hpp"
#include "hikari/client/game/InputService.hpp"
#include "hikari/client/audio/AudioService.hpp"
#include "hikari/client/gui/Panel.hpp"
#include "hikari/client/gui/GuiService.hpp"
#include "hikari/client/Services.hpp"
#include "hikari/core/game/GameController.hpp"
#include "hikari/core/util/ServiceLocator.hpp"
#include "hikari/core/util/Log.hpp"

#include <guichan/gui.hpp>
#include <guichan/widgets/label.hpp>

namespace hikari {

    WeaponGetState::WeaponGetState(const std::string &name)
        : name(name)
    {

    }

    WeaponGetState::~WeaponGetState() {

    }

    void WeaponGetState::handleEvent(sf::Event &event) {

    }

    void WeaponGetState::render(sf::RenderTarget &target) {
        // if(auto gui = guiService.lock()) {
        //     gui->renderAsTop(guiWrapper.get(), target);
        // }
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
        // if(auto gui = guiService.lock()) {
        //     auto & topContainer = gui->getRootContainer();
        //     topContainer.add(guiWrapper.get(), 0, 0);
        //     guiWrapper->setEnabled(true);
        // }

        // if(auto audio = audioService.lock()) {
        //     audio->playMusic("Password (MM3)");
        // }

        // goToNextState = false;
    }

    void WeaponGetState::onExit() {
        // if(auto gui = guiService.lock()) {
        //     auto & topContainer = gui->getRootContainer();
        //     topContainer.remove(guiWrapper.get());
        //     guiWrapper->setEnabled(false);
        // }

        // if(auto audio = audioService.lock()) {
        //     audio->stopMusic();
        // }
    }

    const std::string & WeaponGetState::getName() const {
        return name;
    }

} // hikari
