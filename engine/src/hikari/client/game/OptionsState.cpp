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

    const std::string OptionsState::AUDIO_LABEL_TEXT = "VOLUME";
    const std::string OptionsState::MUSIC_LABEL_TEXT = "MUSIC";
    const std::string OptionsState::SAMPLE_LABEL_TEXT = "SAMPLE";

    OptionsState::OptionsState(const std::string &name, const Json::Value &params, GameController & controller, ServiceLocator &services)
        : GameState()
        , name(name)
        , controller(controller)
        , guiService(services.locateService<GuiService>(Services::GUISERVICE))
        , input(services.locateService<InputService>(Services::INPUT))
        , guiContainer(new gcn::Container())
        , guiLabel(new gcn::Label())
        , audioLabel(new gcn::Label())
        , musicLabel(new gcn::Label())
        , musicVolumeLabel(new gcn::Label())
        , sampleLabel(new gcn::Label())
        , sampleVolumeLabel(new gcn::Label())
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
                    controller.requestStateChange("title");
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

        guiLabel->setCaption("OPTIONS SCREEN");
        guiLabel->adjustSize();

        guiMenu->setWidth(guiContainer->getWidth() - 16);
        guiMenu->setHeight(guiContainer->getHeight() - 16);
        guiMenu->setBackgroundColor(gcn::Color(45, 45, 45));
        guiMenu->addActionListener(guiActionListener.get());
        guiMenu->addSelectionListener(guiSelectionListener.get());
        guiMenu->enableWrapping();

        audioLabel->setCaption(AUDIO_LABEL_TEXT);
        audioLabel->adjustSize();
        musicLabel->setCaption(MUSIC_LABEL_TEXT);
        musicLabel->adjustSize();
        musicVolumeLabel->setCaption("");
        sampleLabel->setCaption(SAMPLE_LABEL_TEXT);
        sampleLabel->adjustSize();
        sampleVolumeLabel->setCaption("");

        std::shared_ptr<gui::MenuItem> backMenuItem(new gui::MenuItem("BACK"));
        backMenuItem->setForegroundColor(gcn::Color(0, 0, 0, 0));
        backMenuItem->setSelectionColor(gcn::Color(250, 128, 128));
        backMenuItem->setY(guiMenu->getHeight() - backMenuItem->getHeight());

        std::shared_ptr<gui::MenuItem> musicVolumeMenuItem(new gui::MenuItem(MUSIC_LABEL_TEXT));
        musicVolumeMenuItem->setForegroundColor(gcn::Color(0, 0, 0, 0));
        musicVolumeMenuItem->setSelectionColor(gcn::Color(250, 128, 128));
        musicVolumeMenuItem->setX(8);
        musicVolumeMenuItem->setY(24);
        
        std::shared_ptr<gui::MenuItem> sampleVolumeMenuItem(new gui::MenuItem(SAMPLE_LABEL_TEXT));
        sampleVolumeMenuItem->setForegroundColor(gcn::Color(0, 0, 0, 0));
        sampleVolumeMenuItem->setSelectionColor(gcn::Color(250, 128, 128));
        sampleVolumeMenuItem->setX(8);
        sampleVolumeMenuItem->setY(32);

        guiMenu->addItem(backMenuItem);
        guiMenu->addItem(musicVolumeMenuItem);
        guiMenu->addItem(sampleVolumeMenuItem);
        guiMenu->setSelectedIndex(0);

        guiContainer->add(guiMenu.get(), 8, 8);
        guiContainer->add(guiLabel.get(), (guiContainer->getWidth() / 2) - (guiLabel->getWidth() / 2), 8);
        guiContainer->add(audioLabel.get(), 8, 16);
        // guiContainer->add(musicLabel.get(), 24, 80 + 16);
        // guiContainer->add(sampleLabel.get(), 24, 80 + 32);
        guiContainer->add(musicVolumeLabel.get(), 100, 80 + 16);
        guiContainer->add(sampleVolumeLabel.get(), 100, 80 + 32);
    }

    void OptionsState::handleEvent(sf::Event &event) {

    }

    void OptionsState::render(sf::RenderTarget &target) {
        if(auto gui = guiService.lock()) {
            gui->renderAsTop(guiContainer.get(), target);
        }
    }

    bool OptionsState::update(float dt) {
        guiMenu->logic();
        return goToNextState;
    }

    void OptionsState::onEnter() {
        // Attach our GUI
        if(auto gui = guiService.lock()) {
            auto & topContainer = gui->getRootContainer();

            topContainer.add(guiContainer.get(), 0, 0);
            guiContainer->setEnabled(true);
            guiContainer->requestFocus();
            guiMenu->setEnabled(true);
            guiMenu->requestFocus();
        }
        
        goToNextState = false;
    }

    void OptionsState::onExit() {
        // Remove our GUI
        if(auto gui = guiService.lock()) {
            auto & topContainer = gui->getRootContainer();

            topContainer.remove(guiContainer.get());
            guiMenu->setEnabled(false);
            guiContainer->setEnabled(false);
        }
    }

    const std::string & OptionsState::getName() const {
        return name;
    }

}// hikari