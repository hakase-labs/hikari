#include "hikari/client/game/TitleState.hpp"
#include "hikari/client/audio/AudioService.hpp"
#include "hikari/client/gui/GuiService.hpp"
#include "hikari/client/gui/Menu.hpp"
#include "hikari/client/gui/MenuItem.hpp"
#include "hikari/client/gui/Icon.hpp"
#include "hikari/client/Services.hpp"

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

    TitleState::TitleState(const std::string &name, const Json::Value &params, ServiceLocator &services)
        : GameState()
        , name(name)
        , guiService(services.locateService<GuiService>(Services::GUISERVICE))
        , audioService(services.locateService<AudioService>(Services::AUDIO))
        , guiContainer(new gcn::Container())
        , guiLabel(new gcn::Label())
        , guiMenu(new gui::Menu())
        , guiIcon(new gui::Icon("assets/images/cursor-stage-select.png"))
        , guiActionListener(nullptr)
        , guiSelectionListener(nullptr)
    {
        buildGui();
    }

    TitleState::~TitleState() {

    }

    void TitleState::buildGui() {
        guiActionListener.reset(new gcn::FunctorActionListener([&](const gcn::ActionEvent& event) {
            auto item = guiMenu->getMenuItemAt(guiMenu->getSelectedIndex());

            if(item) {
                std::cout << "Actioned on #" << guiMenu->getSelectedIndex() << ", " << item->getName() << std::endl;
            } else {
                std::cout << "Actioned on #" << guiMenu->getSelectedIndex() << std::endl;
            }
        }));

        guiSelectionListener.reset(new gcn::FunctorSelectionListener([&](const gcn::SelectionEvent & event) {
            std::cout << "Selection changed! " << guiMenu->getSelectedIndex() << std::endl;

            if(auto audio = audioService.lock()) {
                audio->playSample(27);
            }
        }));

        guiContainer->setBaseColor(gcn::Color(0, 0, 0));
        guiContainer->setOpaque(true);
        guiContainer->setWidth(256);
        guiContainer->setHeight(240);

        guiIcon->setWidth(30);
        guiIcon->setHeight(30);
        guiIcon->setX(16);
        guiIcon->setY(16);

        guiContainer->add(guiIcon.get());

        guiLabel->setCaption("Title Screen");
        guiLabel->adjustSize();

        guiMenu->setWidth(guiContainer->getWidth() - 32);
        guiMenu->setHeight((guiContainer->getHeight() / 2) - 32);
        guiMenu->setBackgroundColor(gcn::Color(45, 45, 45));
        guiMenu->addActionListener(guiActionListener.get());
        guiMenu->enableWrapping();

        std::shared_ptr<gui::MenuItem> gameStartMenuItem(new gui::MenuItem("GAME START"));
        gameStartMenuItem->setForegroundColor(gcn::Color(0, 0, 0, 0));
        gameStartMenuItem->setSelectionColor(gcn::Color(250, 128, 128));
        guiMenu->addItem(gameStartMenuItem);

        std::shared_ptr<gui::MenuItem> passWordMenuItem(new gui::MenuItem("PASS WORD"));
        passWordMenuItem->setY(16);
        passWordMenuItem->setForegroundColor(gcn::Color(0, 0, 0, 0));
        passWordMenuItem->setSelectionColor(gcn::Color(0, 128, 128));
        guiMenu->addItem(passWordMenuItem);

        std::shared_ptr<gui::MenuItem> quitMenuItem(new gui::MenuItem("QUIT"));
        quitMenuItem->setY(32);
        quitMenuItem->setForegroundColor(gcn::Color(0, 0, 0, 0));
        quitMenuItem->setSelectionColor(gcn::Color(250, 128, 128));
        guiMenu->addItem(quitMenuItem);

        guiMenu->setSelectedIndex(0);

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

        guiMenu->addSelectionListener(guiSelectionListener.get());
    }

    void TitleState::onExit() {
        // Remove our GUI
        if(auto gui = guiService.lock()) {
            auto & topContainer = gui->getRootContainer();

            topContainer.remove(guiContainer.get());
        }

        guiMenu->removeSelectionListener(guiSelectionListener.get());
    }

    const std::string & TitleState::getName() const {
        return name;
    }

}// hikari