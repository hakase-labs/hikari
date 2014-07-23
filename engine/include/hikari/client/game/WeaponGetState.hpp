#ifndef HIKARI_CLIENT_GAME_WEAPONGETSTATE
#define HIKARI_CLIENT_GAME_WEAPONGETSTATE

#include "hikari/core/game/GameState.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>

#include <memory>
#include <queue>
#include <string>

namespace gcn {
    class Container;
    class LabelEx;
    class Icon;
}

namespace hikari {

    class AnimationSet;
    class AudioService;
    class GameConfig;
    class GameProgress;
    class GuiService;
    class ImageFont;
    class Input;
    class ServiceLocator;
    class GameController;
    class Task;

    namespace gui {
        // Forward-declare any GUI classes here
        class Icon;
        class IconAnimator;
    }

    class WeaponGetState : public GameState {
    private:
        std::string name;
        GameController & controller;
        std::weak_ptr<GameConfig> gameConfig;
        sf::View view;
        std::weak_ptr<GuiService> guiService;
        std::weak_ptr<AudioService> audioService;
        std::weak_ptr<GameProgress> gameProgress;
        std::shared_ptr<Input> keyboardInput;
        std::queue<std::shared_ptr<Task>> taskQueue;
        bool goToNextState;

        std::unique_ptr<gcn::Container> guiContainer;
        std::unique_ptr<gcn::LabelEx> guiYouGotText;
        std::unique_ptr<gcn::LabelEx> guiWeaponGetText;
        std::unique_ptr<gui::Icon> guiBackground;
        std::unique_ptr<gui::Icon> guiRockman;

        void buildGui(ServiceLocator & services);

    public:
        WeaponGetState(const std::string & name, GameController & controller, const std::weak_ptr<GameConfig> & gameConfig, ServiceLocator &services);
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
