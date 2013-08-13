#include "hikari/client/game/TitleState.hpp"
#include "hikari/client/audio/AudioService.hpp"
#include "hikari/client/gui/GuiService.hpp"
#include "hikari/client/gui/Menu.hpp"
#include "hikari/client/gui/MenuItem.hpp"
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
        , guiMenu(new gui::Menu())
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

        guiMenu->setWidth(guiContainer->getWidth() - 32);
        guiMenu->setHeight((guiContainer->getHeight() / 2) - 32);
        guiMenu->setBackgroundColor(gcn::Color(45, 45, 45));

        std::shared_ptr<gui::MenuItem> firstItem(new gui::MenuItem("FIRST ITEM"));
        firstItem->setWidth(60);
        firstItem->setHeight(8);
        firstItem->setForegroundColor(gcn::Color(250, 250, 250));
        firstItem->setSelectionColor(gcn::Color(250, 128, 128));
        guiMenu->addItem(firstItem);

        std::shared_ptr<gui::MenuItem> secondItem(new gui::MenuItem("SECOND ITEM"));
        secondItem->setWidth(60);
        secondItem->setHeight(8);
        secondItem->setY(8);
        secondItem->setForegroundColor(gcn::Color(250, 250, 250));
        secondItem->setSelectionColor(gcn::Color(250, 128, 128));
        guiMenu->addItem(secondItem);

        std::shared_ptr<gui::MenuItem> thirdItem(new gui::MenuItem("THIRD ITEM"));
        thirdItem->setWidth(60);
        thirdItem->setHeight(8);
        thirdItem->setY(16);
        thirdItem->setForegroundColor(gcn::Color(250, 250, 250));
        thirdItem->setSelectionColor(gcn::Color(250, 128, 128));
        guiMenu->addItem(thirdItem);

        guiContainer->add(guiMenu.get(), 16, 128);

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
            guiMenu->requestFocus();
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