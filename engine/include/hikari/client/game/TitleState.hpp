#ifndef HIKARI_CLIENT_GAME_TITLESTATE
#define HIKARI_CLIENT_GAME_TITLESTATE

#include "hikari/core/game/GameState.hpp"

#include <memory>
#include <string>

namespace Json {
    class Value;
}

namespace gcn {
    class Container;
    class Label;
    class ActionListener;
    class SelectionListener;
}

namespace hikari {

    namespace gui {
        class Menu;
        class Icon;
    }

    class AudioService;
    class GuiService;
    class ServiceLocator;
    class GameController;
    class KeyboardInput;

    class TitleState : public GameState {
    private:
        std::string name;
        GameController & controller;
        std::weak_ptr<GuiService> guiService;
        std::weak_ptr<AudioService> audioService;
        std::shared_ptr<KeyboardInput> keyboardInput;
        std::unique_ptr<gcn::Container> guiContainer;
        std::unique_ptr<gcn::Label> guiLabel;
        std::unique_ptr<gui::Menu> guiMenu;
        std::unique_ptr<gui::Icon> guiIcon;
        std::unique_ptr<gcn::ActionListener> guiActionListener;
        std::unique_ptr<gcn::SelectionListener> guiSelectionListener;

        bool goToNextState;

        void buildGui();

    public:
        TitleState(const std::string &name, const Json::Value &params, GameController & controller, ServiceLocator &services);
        virtual ~TitleState();

        virtual void handleEvent(sf::Event &event);
        virtual void render(sf::RenderTarget &target);
        virtual bool update(const float &dt);
        virtual void onEnter();
        virtual void onExit();
        virtual const std::string &getName() const;
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_TITLESTATE