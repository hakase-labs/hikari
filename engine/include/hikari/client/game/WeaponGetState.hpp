#ifndef HIKARI_CLIENT_GAME_WEAPONGETSTATE
#define HIKARI_CLIENT_GAME_WEAPONGETSTATE

#include "hikari/core/game/GameState.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>
#include <string>

namespace hikari {

    class WeaponGetState : public GameState {
    private:
        std::string name;

    public:
        WeaponGetState(const std::string & name);
        virtual ~WeaponGetState();

        virtual void handleEvent(sf::Event &event);
        virtual void render(sf::RenderTarget &target);
        virtual bool update(float dt);
        virtual void onEnter();
        virtual void onExit();

        virtual const std::string & getName() const;
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_WEAPONGETSTATE
