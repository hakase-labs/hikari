#include "hikari/client/game/PasswordState.hpp"
#include "hikari/client/gui/Panel.hpp"
#include "hikari/client/gui/GuiService.hpp"
#include "hikari/client/Services.hpp"
#include "hikari/core/util/ServiceLocator.hpp"

#include <guichan/gui.hpp>
#include <guichan/widgets/label.hpp>
#include <json/reader.h>

namespace hikari {

    PasswordState::PasswordState(const std::string &name, const Json::Value &params, GameController & controller, ServiceLocator &services)
        : name(name)
        , controller(controller)
        , passwordGrid(new gui::Panel())
        , guiWrapper(new gcn::Container())
        , testLabel(new gcn::Label())
        , guiService(services.locateService<GuiService>(Services::GUISERVICE))
        , goToNextState(false)
    {
        guiWrapper->setWidth(256);
        guiWrapper->setHeight(240);
        guiWrapper->setOpaque(true);
        guiWrapper->setBaseColor(gcn::Color(12, 56, 130));

        testLabel->setCaption("Password!");
        testLabel->adjustSize();

        passwordGrid->setWidth(100);
        passwordGrid->setHeight(100);

        guiWrapper->add(testLabel.get(), 100, 4);
        guiWrapper->add(passwordGrid.get(), 10, 20);
    }

    PasswordState::~PasswordState() {

    }

    void PasswordState::handleEvent(sf::Event &event) {

    }

    void PasswordState::render(sf::RenderTarget &target) {

    }

    bool PasswordState::update(const float &dt) {
        return goToNextState;
    }

    void PasswordState::onEnter() {
        if(auto gui = guiService.lock()) {
            auto & topContainer = gui->getRootContainer();

            topContainer.add(guiWrapper.get(), 0, 0);
        }

        goToNextState = false;
    }

    void PasswordState::onExit() {
        if(auto gui = guiService.lock()) {
            auto & topContainer = gui->getRootContainer();

            topContainer.remove(guiWrapper.get());
        }
    }

    const std::string & PasswordState::getName() const {
        return name;
    }

} // hikari