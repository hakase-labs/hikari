#include "hikari/client/game/OptionsState.hpp"
#include "hikari/client/game/InputService.hpp"
#include "hikari/client/game/Input.hpp"
#include "hikari/client/audio/AudioService.hpp"
#include "hikari/client/gui/GuiService.hpp"
#include "hikari/client/gui/Menu.hpp"
#include "hikari/client/gui/MenuItem.hpp"
#include "hikari/client/Services.hpp"

#include "hikari/core/game/GameController.hpp"
#include "hikari/core/util/ServiceLocator.hpp"
#include "hikari/core/util/Log.hpp"

#include <guichan/widgets/container.hpp>
#include <guichan/widgets/label.hpp>

#include <guichan/hakase/functoractionlistener.hpp>
#include <guichan/hakase/functorselectionlistener.hpp>

#include <json/value.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

namespace hikari {

    OptionsState::OptionsState(const std::string &name, const Json::Value &params, GameController & controller, ServiceLocator &services)
        : GameState()
        , name(name)
        , controller(controller)
        , guiService(services.locateService<GuiService>(Services::GUISERVICE))
        , input(services.locateService<InputService>("INPUT"))
        , guiContainer(new gcn::Container())
        , guiLabel(new gcn::Label())
        , guiMenu(new gui::Menu())
        , guiActionListener(nullptr)
        , guiSelectionListener(nullptr)
        , goToNextState(false)
    {
        buildGui();
    }

    OptionsState::~OptionsState() {

    }

    void OptionsState::buildGui() {
        guiActionListener.reset(new gcn::FunctorActionListener([&](const gcn::ActionEvent& event) {
            auto item = guiMenu->getMenuItemAt(guiMenu->getSelectedIndex());

            if(item) {
                std::cout << "Actioned on #" << guiMenu->getSelectedIndex() << ", " << item->getName() << std::endl;

                if(item->getName() == "BACK") {
                    controller.setNextState("title");
                    goToNextState = true;
                }
            } else {
                std::cout << "Actioned on #" << guiMenu->getSelectedIndex() << std::endl;
            }
        }));

        guiSelectionListener.reset(new gcn::FunctorSelectionListener([&](const gcn::SelectionEvent & event) {
            std::cout << "Selection changed! " << guiMenu->getSelectedIndex() << std::endl;
        }));

        guiContainer->setBaseColor(gcn::Color(0, 25, 25));
        guiContainer->setOpaque(true);
        guiContainer->setWidth(256);
        guiContainer->setHeight(240);

        guiLabel->setCaption("Options Screen");
        guiLabel->adjustSize();

        guiMenu->setWidth(guiContainer->getWidth() - 32);
        guiMenu->setHeight((guiContainer->getHeight() / 2) - 32);
        guiMenu->setBackgroundColor(gcn::Color(45, 45, 45));
        guiMenu->addActionListener(guiActionListener.get());
        guiMenu->addSelectionListener(guiSelectionListener.get());
        guiMenu->enableWrapping();
        guiMenu->setInput(input);

        std::shared_ptr<gui::MenuItem> gameStartMenuItem(new gui::MenuItem("BACK"));
        gameStartMenuItem->setForegroundColor(gcn::Color(0, 0, 0, 0));
        gameStartMenuItem->setSelectionColor(gcn::Color(250, 128, 128));
        guiMenu->addItem(gameStartMenuItem);

        guiMenu->setSelectedIndex(0);

        guiContainer->add(guiMenu.get(), 16, 128);
        guiContainer->add(guiLabel.get(), 30, 30);
    }

    void OptionsState::handleEvent(sf::Event &event) {

    }

    void OptionsState::render(sf::RenderTarget &target) {

    }

    bool OptionsState::update(const float &dt) {
        guiMenu->logic();
        return goToNextState;
    }

    void OptionsState::onEnter() {
        // Attach our GUI
        if(auto gui = guiService.lock()) {
            auto & topContainer = gui->getRootContainer();

            topContainer.add(guiContainer.get(), 0, 0);
            guiMenu->requestFocus();
        }

        goToNextState = false;
    }

    void OptionsState::onExit() {
        // Remove our GUI
        if(auto gui = guiService.lock()) {
            auto & topContainer = gui->getRootContainer();

            topContainer.remove(guiContainer.get());
        }
    }

    const std::string & OptionsState::getName() const {
        return name;
    }

}// hikari