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
    }

    class AudioService;
    class GuiService;
    class ServiceLocator;

    class TitleState : public GameState {
    private:
        std::string name;
        std::weak_ptr<GuiService> guiService;
        std::unique_ptr<gcn::Container> guiContainer;
        std::unique_ptr<gcn::Label> guiLabel;
        std::unique_ptr<gui::Menu> guiMenu;
        std::unique_ptr<gcn::ActionListener> guiActionListener;
        std::unique_ptr<gcn::SelectionListener> guiSelectionListener;

        void buildGui();

    public:
        TitleState(const std::string &name, const Json::Value &params, ServiceLocator &services);
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