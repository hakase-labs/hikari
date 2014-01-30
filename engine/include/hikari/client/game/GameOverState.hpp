#ifndef HIKARI_CLIENT_GAME_GAMEOVERSTATE
#define HIKARI_CLIENT_GAME_GAMEOVERSTATE

#include "hikari/core/game/GameState.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>
#include <memory>
#include <string>

namespace gcn {
    class Container;
    class Label;
    class ActionListener;
    class SelectionListener;}

namespace Json {
    class Value;
}

namespace hikari {

    class GuiService;
    class Input;
    class AudioService;
    
    namespace gui {
        class Panel;
        class Menu;
        class Icon;
    }

    class GameController;
    class ServiceLocator;

    class GameOverState : public GameState {
    private:
        const static std::string ITEM_CONTINUE;
        const static std::string ITEM_PASS_WORD;
        const static std::string ITEM_STAGE_SELECT;
        const static std::string ITEM_TITLE_SCREEN;

        std::string name;
        GameController & controller;
        std::weak_ptr<AudioService> audioService;
        std::shared_ptr<Input> keyboardInput;
        std::unique_ptr<gui::Panel> mainPanel;
        std::unique_ptr<gcn::Container> guiWrapper;
        std::unique_ptr<gui::Menu> guiMenu;
        std::unique_ptr<gui::Icon> guiCursorIcon;
        std::unique_ptr<gcn::Label> gameOverLabel;
        std::weak_ptr<GuiService> guiService;

        bool goToNextState;

    public:
        GameOverState(const std::string &name, const Json::Value &params, GameController & controller, ServiceLocator &services);
        virtual ~GameOverState();

        virtual void handleEvent(sf::Event &event);
        virtual void render(sf::RenderTarget &target);
        virtual bool update(float dt);
        virtual void onEnter();
        virtual void onExit();
        virtual const std::string &getName() const;
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_GAMEOVERSTATE