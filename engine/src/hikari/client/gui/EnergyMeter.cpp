#include <hikari/client/gui/EnergyMeter.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace hikari {
    
    const float EnergyMeter::HORIZONTAL_ROTATION_ANGLE = 270.0f;
    const float EnergyMeter::VERTICAL_ROTATION_ANGLE = 0.0f;

    const int EnergyMeter::HORIZONTAL_ORIENTATION = 0;
    const int EnergyMeter::VERTICAL_ORIENTATION = 1;

    EnergyMeter::EnergyMeter(const sf::Sprite &overlay, const float &maximumValue)
        : overlay(overlay)
        , maximumValue(maximumValue)
        , value(maximumValue)
        , fillColor(sf::Color::Black)
        , primaryColor(sf::Color(252, 216, 168, 255))
        , secondaryColor(sf::Color::White)
    {
        setVisible(true);
        setFillColor(fillColor);

        foreground.setPosition(overlay.getPosition());
        foreground.setSize(sf::Vector2f(overlay.getLocalBounds().width, overlay.getLocalBounds().height));
        foreground.setFillColor(fillColor);

        primaryBackground.setPosition(foreground.getPosition());
        primaryBackground.setSize(foreground.getSize());
        primaryBackground.setFillColor(primaryColor);

        secondaryBackground.setPosition(foreground.getPosition().x + 3, foreground.getPosition().y);
        secondaryBackground.setSize(sf::Vector2f(2.0f, foreground.getSize().y));
        secondaryBackground.setFillColor(secondaryColor);  

        updateFill();

        setOrientation(VERTICAL_ORIENTATION);
    }

    void EnergyMeter::updateFill() {
        float yScale = ((maximumValue - value)/maximumValue);

        foreground.setScale(1.0f, yScale);
    }

    void EnergyMeter::setOrientation(const int &newOrientation) {
        if(newOrientation != orientation) {
            orientation = newOrientation;
        }
        updateOrientation();
    }

    void EnergyMeter::updateOrientation() {
        if(orientation == HORIZONTAL_ORIENTATION) {
            const sf::Vector2f bgSize(overlay.getLocalBounds().width, overlay.getLocalBounds().height);
            const sf::Vector2f bgPosition = overlay.getPosition();

            // Rotates around top left corner. It makes sense.
            overlay.setOrigin(0.0f, bgSize.y);
            overlay.setRotation(HORIZONTAL_ROTATION_ANGLE);
            foreground.setOrigin(0.0f, 0.0f);
            foreground.setRotation(HORIZONTAL_ROTATION_ANGLE);
            foreground.setPosition(bgPosition.x + bgSize.y, bgPosition.y);
        } else if(orientation == VERTICAL_ORIENTATION) {
            overlay.setOrigin(0.0f, 0.0f);
            overlay.setRotation(VERTICAL_ROTATION_ANGLE);
            foreground.setOrigin(0.0f, 0.0f);
            foreground.setRotation(VERTICAL_ROTATION_ANGLE);
            foreground.setPosition(overlay.getPosition());
        }
    }

    void EnergyMeter::setPosition(const sf::Vector2i &newPosition) {
        Widget::setPosition(newPosition);
        overlay.setPosition(static_cast<float>(newPosition.x), static_cast<float>(newPosition.y));
        foreground.setPosition(static_cast<float>(newPosition.x), static_cast<float>(newPosition.y));
        primaryBackground.setPosition(foreground.getPosition());
        secondaryBackground.setPosition(foreground.getPosition().x + 3, foreground.getPosition().y);
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

    const sf::Color& EnergyMeter::getPrimaryColor() const {
        return primaryColor;
    }

    const sf::Color& EnergyMeter::getSecondaryColor() const {
        return secondaryColor;
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
        overlay.setColor(fillColor);
    }

    void EnergyMeter::setPrimaryColor(const sf::Color &newColor) {
        primaryColor = newColor;
        primaryBackground.setFillColor(getPrimaryColor());
    }

    void EnergyMeter::setSecondaryColor(const sf::Color &newColor) {
        secondaryColor = newColor;
        secondaryBackground.setFillColor(getSecondaryColor());
    }

    void EnergyMeter::render(sf::RenderTarget &target) {
        if(isVisible()) {
            target.draw(primaryBackground);
            target.draw(secondaryBackground);
            target.draw(overlay);
            
            if(getValue() < getMaximumValue()) {
                target.draw(foreground);
            }
        }
    }

    void EnergyMeter::update(const float &delta) {

    }

} // hikari