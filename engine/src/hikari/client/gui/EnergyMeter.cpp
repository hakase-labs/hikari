#include <hikari/client/gui/EnergyMeter.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace hikari {
    
    const float EnergyMeter::HORIZONTAL_ROTATION_ANGLE = 270.0f;
    const float EnergyMeter::VERTICAL_ROTATION_ANGLE = 0.0f;

    const int EnergyMeter::HORIZONTAL_ORIENTATION = 0;
    const int EnergyMeter::VERTICAL_ORIENTATION = 1;

    EnergyMeter::EnergyMeter(const sf::Sprite &background, const float &maximumValue)
        : background(background)
        , maximumValue(maximumValue)
        , value(maximumValue)
        , fillColor(sf::Color::Black) {
            
            setVisible(true);
            /*
            foreground = sf::RectangleShape(
                background.getPosition().x,
                background.getPosition().y,
                background.GetSize().x,
                background.GetSize().y,
                sf::Color::White,
                0.0f
            );
            */
            foreground.setPosition(background.getPosition());
            foreground.setSize(sf::Vector2f(background.getLocalBounds().width, background.getLocalBounds().height));
            foreground.setFillColor(fillColor);
            //foreground.enableFill(true);
            //foreground.enableOutline(false);    

        updateFill();

        setOrientation(VERTICAL_ORIENTATION);
    }

    void EnergyMeter::updateFill() {
        foreground.setScale(1.0f, ((maximumValue - value)/maximumValue));
    }

    void EnergyMeter::setOrientation(const int &newOrientation) {
        if(newOrientation != orientation) {
            orientation = newOrientation;
            
        }
        updateOrientation();
    }

    void EnergyMeter::updateOrientation() {
        if(orientation == HORIZONTAL_ORIENTATION) {
            const sf::Vector2f bgSize(background.getLocalBounds().width, background.getLocalBounds().height);
            const sf::Vector2f bgPosition = background.getPosition();

            // Rotates around top left corner. It makes sense.
            background.setOrigin(0.0f, bgSize.y);
            background.setRotation(HORIZONTAL_ROTATION_ANGLE);
            foreground.setOrigin(0.0f, 0.0f);
            foreground.setRotation(HORIZONTAL_ROTATION_ANGLE);
            foreground.setPosition(bgPosition.x + bgSize.y, bgPosition.y);
        } else if(orientation == VERTICAL_ORIENTATION) {
            background.setOrigin(0.0f, 0.0f);
            background.setRotation(VERTICAL_ROTATION_ANGLE);
            foreground.setOrigin(0.0f, 0.0f);
            foreground.setRotation(VERTICAL_ROTATION_ANGLE);
            foreground.setPosition(background.getPosition());
        }
    }

    void EnergyMeter::setPosition(const sf::Vector2i &newPosition) {
        Widget::setPosition(newPosition);
        background.setPosition(static_cast<float>(newPosition.x), static_cast<float>(newPosition.y));
        foreground.setPosition(static_cast<float>(newPosition.x), static_cast<float>(newPosition.y));
    }

    const float& EnergyMeter::getValue() const {
        return value;
    }

    const float& EnergyMeter::getMaximumValue() const {
        return maximumValue;
    }

    const sf::Color& EnergyMeter::getFillColor() const {
        return fillColor;
    }

    const int& EnergyMeter::getOrientation() const {
        return orientation;
    }

    void EnergyMeter::setValue(const float &newValue) {
        if(newValue >= 0.0f) {
            value = newValue;
            value = std::min(value, getMaximumValue());
            value = std::max(value, 0.0f);
            updateFill();
        }
        // TODO: Notify caller? Exception?
    }

    void EnergyMeter::setMaximumValue(const float &newMaximumValue) {
        if(newMaximumValue > 0.0f) {
            maximumValue = newMaximumValue;
            updateFill();
        }
        // TODO: Throw exception? Something needs to happen.
    }

    void EnergyMeter::setFillColor(const sf::Color &newColor) {
        fillColor = newColor;
        foreground.setFillColor(fillColor);
    }

    void EnergyMeter::render(sf::RenderTarget &target) {
        if(isVisible()) {
            target.draw(background);
            
            if(getValue() < getMaximumValue()) {
                target.draw(foreground);
            }
        }
    }

    void EnergyMeter::update(const float &delta) {

    }

} // hikari