#include <hikari/client/gui/Widget.hpp>

namespace hikari {

    const sf::Vector2i& Widget::getPosition() const {
        return position;
    }

    void Widget::setPosition(const sf::Vector2i &newPosition) {
        position = newPosition;
    }
    
    const bool& Widget::isVisible() const {
        return visible;
    }

    void Widget::setVisible(const bool &isVisible) {
        visible = isVisible;
    }

}