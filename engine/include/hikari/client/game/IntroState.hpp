#ifndef HIKARI_CLIENT_GAME_INTROSTATE
#define HIKARI_CLIENT_GAME_INTROSTATE

#include "hikari/core/game/GameState.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>
#include <memory>
#include <json/value.h>
#include <string>

namespace hikari {

// Forward declaration
namespace core {
namespace gui {
    class ImageFont;
}
}

namespace client {
namespace game {

    class IntroState : public hikari::core::game::GameState {
    private:
        std::string name;
        sf::RenderTarget &target;
        sf::View view;
        std::shared_ptr<hikari::core::gui::ImageFont> guiFont;

    public:
        IntroState(const Json::Value &params, std::shared_ptr<hikari::core::gui::ImageFont> guiFont);
        virtual ~IntroState() {}

        virtual void handleEvent(sf::Event &event);
        virtual void render(sf::RenderTarget &target);
        virtual bool update(const float &dt);
        virtual void onEnter();
        virtual void onExit();
        virtual const std::string &getName() const;
    };

} // hikari.client.game
} // hikari.client
} // hikari

#endif