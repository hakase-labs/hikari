#ifndef HIKARI_CLIENT_GAME_GUITESTSTATE
#define HIKARI_CLIENT_GAME_GUITESTSTATE

#include "hikari/core/game/GameState.hpp"
#include "hikari/core/gui/ImageFont.hpp"
#include "hikari/client/gui/EnergyMeter.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <string>

namespace sf {
    class RenderTarget;
}

namespace hikari {

    class GuiTestState : public GameState {
    private:
        std::string name;
        sf::Texture energyMeterImage;
        std::shared_ptr<hikari::ImageFont> font;
        std::shared_ptr<hikari::gui::EnergyMeter> energyMeter;
        float ups;
    
    public:
        GuiTestState(const std::string &name, const std::shared_ptr<hikari::ImageFont> &font);
        
        virtual ~GuiTestState() {}

        virtual void handleEvent(sf::Event &event);
        virtual void render(sf::RenderTarget &target);
        virtual bool update(float dt);
        virtual void onEnter();
        virtual void onExit();
        virtual const std::string &getName() const;
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_GUITESTSTATE