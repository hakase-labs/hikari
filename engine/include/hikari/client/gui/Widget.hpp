#ifndef HIKARI_CORE_GUI_WIDGET
#define HIKARI_CORE_GUI_WIDGET

#include <memory>
#include <SFML/System/Vector2.hpp>

namespace sf {
    class RenderTarget;
}

namespace hikari {

    class Widget {
    private:
        int id;
        bool visible;
        sf::Vector2i position;
    public:
        virtual ~Widget() {}

        virtual const sf::Vector2i& getPosition() const;
        virtual void setPosition(const sf::Vector2i &newPosition);

        const bool& isVisible() const;
        void setVisible(const bool &isVisible);

        virtual void render(sf::RenderTarget &target) = 0;
        virtual void update(const float &delta) = 0;
    };

    typedef std::shared_ptr<Widget> WidgetPtr;

}

#endif // HIKARI_CORE_GUI_WIDGET