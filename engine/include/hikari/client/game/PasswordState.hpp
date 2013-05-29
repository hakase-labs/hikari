#ifndef HIKARI_CLIENT_GAME_PASSWORDSTATE
#define HIKARI_CLIENT_GAME_PASSWORDSTATE

#include "hikari/core/game/GameState.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>
#include <memory>
#include <string>

namespace gcn {
    class Container;
    class Label;
}

namespace Json {
    class Value;
}

namespace hikari {

    class GuiService;
    
    namespace gui {
        class Panel;
    }

    class ServiceLocator;

    class PasswordState : public GameState {
    private:
        std::string name;
        std::unique_ptr<gui::Panel> passwordGrid;
        std::unique_ptr<gcn::Container> guiWrapper;
        std::unique_ptr<gcn::Label> testLabel;
        std::weak_ptr<GuiService> guiService;

    public:
        PasswordState(const std::string &name, const Json::Value &params, ServiceLocator &services);
        virtual ~PasswordState();

        virtual void handleEvent(sf::Event &event);
        virtual void render(sf::RenderTarget &target);
        virtual bool update(const float &dt);
        virtual void onEnter();
        virtual void onExit();
        virtual const std::string &getName() const;
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_PASSWORDSTATE