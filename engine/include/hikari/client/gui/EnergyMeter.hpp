#ifndef HIKARI_CORE_GUI_ENERGYMETER
#define HIKARI_CORE_GUI_ENERGYMETER

#include <hikari/client/gui/Widget.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace hikari {
namespace gui {

    class EnergyMeter : public Widget {
    private:
        static const float HORIZONTAL_ROTATION_ANGLE;
        static const float VERTICAL_ROTATION_ANGLE;
        static const float HIGHLIGHT_OFFSET_X;

        float value;
        float maximumValue;
        int orientation;

        sf::Sprite overlay;
        sf::RectangleShape primaryBackground;
        sf::RectangleShape secondaryBackground;
        sf::RectangleShape foreground;
        sf::Color primaryColor;
        sf::Color secondaryColor;
        sf::Color fillColor;

        void updateFill();
        void updateOrientation();

    public:
        static const int HORIZONTAL_ORIENTATION;
        static const int VERTICAL_ORIENTATION;

        static const sf::Color DEFAULT_FILL_COLOR;
        static const sf::Color DEFAULT_PRIMARY_COLOR;
        static const sf::Color DEFAULT_SECONDARY_COLOR;

        EnergyMeter(const sf::Sprite &overlay, const float &maximumValue);
        virtual ~EnergyMeter() {}

        void setOrientation(const int &orientation);
        virtual void setPosition(const sf::Vector2i &newPosition);

        const float& getValue() const;
        const float& getMaximumValue() const;
        const sf::Color& getFillColor() const;
        const sf::Color& getPrimaryColor() const;
        const sf::Color& getSecondaryColor() const;
        const int& getOrientation() const;

        void setValue(const float &newValue);
        void setMaximumValue(const float &newValue);
        void setFillColor(const sf::Color &newColor);
        void setPrimaryColor(const sf::Color &newColor);
        void setSecondaryColor(const sf::Color &newColor);

        virtual void render(sf::RenderTarget &target);
        virtual void update(const float &delta);
    };

} // hikari::gui
} // hikari

#endif // HIKARI_CORE_GUI_WIDGET