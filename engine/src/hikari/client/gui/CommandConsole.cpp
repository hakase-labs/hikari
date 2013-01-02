#include <hikari/client/gui/CommandConsole.hpp>
#include <hikari/core/gui/ImageFont.hpp>

namespace hikari {

    CommandConsole::CommandConsole(const std::shared_ptr<hikari::ImageFont> &font)
        : font(font)
        , background(sf::RectangleShape())
        , visible(true) 
        , state(StateClosed)
        , commandBuffer("")
    {
        background.setSize(sf::Vector2f(256.0f, 100.0f));
        background.setFillColor(sf::Color(102, 102, 102, 224));
        background.setOutlineThickness(2.0f);
        background.setOutlineColor(sf::Color(51, 51, 51));
    }

    void CommandConsole::setPosition(const sf::Vector2i &newPosition) {
        sf::Vector2f floatPosition(
            static_cast<float>(newPosition.x), 
            static_cast<float>(newPosition.y)
        );
        background.setPosition(floatPosition);
    }

    const std::string& CommandConsole::getCommandBuffer() const {
        return commandBuffer;
    }

    void CommandConsole::setCommandBuffer(const std::string& buffer) {
        commandBuffer = buffer;
    }

    void CommandConsole::commitCommand() {
        // TODO: Execute command
        commandHistory.push_back(commandBuffer);
        commandBuffer.clear();
    }

    const bool CommandConsole::isOpen() const {
        return state != StateClosed;
    }

    void CommandConsole::toggle() {
        if(isOpen()) {
            state = StateClosing;
        } else {
            state = StateOpening;
        }
    }

    const bool CommandConsole::isVisible() const {
        return visible;
    }

    void CommandConsole::setVisible(bool visibility) {
        visible = visibility;
    }

    void CommandConsole::render(sf::RenderTarget &target) {
        if(isOpen()) {
            target.draw(background);

            // Render the last n history
            std::vector<std::string>::const_reverse_iterator itr = commandHistory.rbegin();
            std::vector<std::string>::const_reverse_iterator stop = itr + std::min(static_cast<int>(commandHistory.size()), 12);
            for(int line = 2; itr < stop; itr++, line++) {
                font->renderText(target, *itr, 1, static_cast<int>(background.getPosition().y) + 100 - font->getGlyphHeight() * line);
            }

            // Render command buffer
            font->renderText(target, ">", 1, static_cast<int>(background.getPosition().y) + 100 - font->getGlyphHeight());
            font->renderText(target, commandBuffer, 9, static_cast<int>(background.getPosition().y) + 100 - font->getGlyphHeight());
        }
    }

    void CommandConsole::update(const float &delta) {
        if(state == StateClosing) {
            if(background.getPosition().y > - 102.0f) {
                background.move(0.0f, -10.0f);
            } else {
                state = StateClosed;
            }
        }

        if(state == StateOpening) {
            if(background.getPosition().y < 0.0f) {
                background.move(0.0f, 10.0f);
            } else {
                state = StateOpen;
            }
        }
    }

}