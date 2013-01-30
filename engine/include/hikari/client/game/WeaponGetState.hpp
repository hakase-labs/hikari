#ifndef HIKARI_CLIENT_GAME_WEAPONGETSTATE
#define HIKARI_CLIENT_GAME_WEAPONGETSTATE

#include "hikari/core/game/GameState.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>
#include <json/value.h>
#include <string>

namespace hikari {
namespace client {
namespace game {

    class WeaponGetState : public hikari::core::game::GameState {
    private:
        std::string name;
        sf::RenderTarget &target;
        sf::View view;

    public:
        WeaponGetState(const Json::Value &params);
        virtual ~WeaponGetState() {}

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

#endif // HIKARI_CLIENT_GAME_WEAPONGETSTATE