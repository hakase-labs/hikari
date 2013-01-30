#ifndef HIKARI_CLIENT_GUI_COMMANDCONSOLE
#define HIKARI_CLIENT_GUI_COMMANDCONSOLE

#include "hikari/client/gui/Widget.hpp"

#include <SFML/Graphics.hpp>

#include <memory>

#include <string>
#include <vector>

namespace hikari {

    class ImageFont;

namespace gui {

    class CommandConsole : public Widget {
    private:
        enum ConsoleState {
            StateOpen = 0,
            StateOpening = 1,
            StateClosing = 2,
            StateClosed = 3
        };

        bool visible;
        ConsoleState state;
        std::string commandBuffer;
        std::vector<std::string> commandHistory;
        std::shared_ptr<hikari::ImageFont> font;
        sf::RectangleShape background;

    public:
        CommandConsole(const std::shared_ptr<hikari::ImageFont> &font);
        virtual ~CommandConsole() {}

        virtual void setPosition(const sf::Vector2i &newPosition);

        const std::string& getCommandBuffer() const;
        void setCommandBuffer(const std::string& buffer);
        void commitCommand();

        const bool isOpen() const;
        void toggle();

        const bool isVisible() const;
        void setVisible(bool visibility);

        virtual void render(sf::RenderTarget &target);
        virtual void update(const float &delta);
    };

} // hikari::gui
} // hikari

#endif // HIKARI_CLIENT_GUI_COMMANDCONSOLE