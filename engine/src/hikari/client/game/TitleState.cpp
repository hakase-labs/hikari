#include "hikari/client/game/TitleState.hpp"
#include "hikari/client/audio/AudioService.hpp"
#include "hikari/client/gui/GuiService.hpp"
#include "hikari/client/Services.hpp"

#include "hikari/core/util/ServiceLocator.hpp"
#include "hikari/core/util/Log.hpp"

#include <guichan/widgets/container.hpp>
#include <guichan/widgets/label.hpp>

#include <json/value.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

namespace hikari {

    TitleState::TitleState(const std::string &name, const Json::Value &params, ServiceLocator &services)
        : GameState()
        , name(name)
        , guiService(services.locateService<GuiService>(Services::GUISERVICE))
        , guiContainer(new gcn::Container())
        , guiLabel(new gcn::Label())
    {
        buildGui();
    }

    TitleState::~TitleState() {

    }

    void TitleState::buildGui() {
        guiContainer->setBaseColor(gcn::Color(0, 0, 0));
        guiContainer->setOpaque(true);
        guiContainer->setWidth(256);
        guiContainer->setHeight(240);

        guiLabel->setCaption("Title Screen");
        guiLabel->adjustSize();

        guiContainer->add(guiLabel.get(), 30, 30);
    }

    void TitleState::handleEvent(sf::Event &event) {

    }

    void TitleState::render(sf::RenderTarget &target) {

    }

    bool TitleState::update(const float &dt) {
        return false;
    }

    void TitleState::onEnter() {
        // Attach our GUI
        if(auto gui = guiService.lock()) {
            auto & topContainer = gui->getRootContainer();

            topContainer.add(guiContainer.get(), 0, 0);
        }
    }

    void TitleState::onExit() {
        // Remove our GUI
        if(auto gui = guiService.lock()) {
            auto & topContainer = gui->getRootContainer();

            topContainer.remove(guiContainer.get());
        }
    }

    const std::string & TitleState::getName() const {
        return name;
    }

}// hikari