#ifndef HIKARI_CLIENT_GAME_WEAPONGETSTATE
#define HIKARI_CLIENT_GAME_WEAPONGETSTATE

#include "hikari/core/game/GameState.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>
#include <string>

namespace hikari {

    class AnimationSet;
    class AudioService;
    class GameProgress;
    class GuiService;
    class ImageFont;
    class ServiceLocator;
    class GameController;

    class WeaponGetState : public GameState {
    private:
        std::string name;
        GameController & controller;
        sf::View view;
        std::weak_ptr<GuiService> guiService;
        std::weak_ptr<AudioService> audioService;
        std::weak_ptr<GameProgress> gameProgress;

    public:
        WeaponGetState(const std::string & name, GameController & controller, ServiceLocator &services);
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
