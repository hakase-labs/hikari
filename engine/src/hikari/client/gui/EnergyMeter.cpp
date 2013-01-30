#include "hikari/client/gui/EnergyMeter.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

namespace hikari {
namespace gui {

    const float EnergyMeter::HORIZONTAL_ROTATION_ANGLE = 270.0f;
    const float EnergyMeter::VERTICAL_ROTATION_ANGLE = 0.0f;
    const float EnergyMeter::HIGHLIGHT_OFFSET_X = 3.0f;

    const sf::Color EnergyMeter::DEFAULT_FILL_COLOR = sf::Color(0, 0, 0, 255);
    const sf::Color EnergyMeter::DEFAULT_PRIMARY_COLOR = sf::Color(252, 216, 168, 255);
    const sf::Color EnergyMeter::DEFAULT_SECONDARY_COLOR = sf::Color(255, 255, 255, 255);

    const int EnergyMeter::HORIZONTAL_ORIENTATION = 0;
    const int EnergyMeter::VERTICAL_ORIENTATION = 1;

    EnergyMeter::EnergyMeter(const sf::Sprite &overlay, const float &maximumValue)
        : orientation(-1)
        , overlay(overlay)
        , maximumValue(maximumValue)
        , value(maximumValue)
        , fillColor(DEFAULT_FILL_COLOR)
        , primaryColor(DEFAULT_PRIMARY_COLOR)
        , secondaryColor(DEFAULT_SECONDARY_COLOR)
    {
        setVisible(true);

        foreground.setSize(
            sf::Vector2f(
                overlay.getLocalBounds().width, 
                overlay.getLocalBounds().height
            )
        );

        primaryBackground.setSize(foreground.getSize());

        secondaryBackground.setSize(
            sf::Vector2f(
                2.0f,
                foreground.getSize().y
            )
        );

        setFillColor(fillColor);
        setPrimaryColor(primaryColor);
        setSecondaryColor(secondaryColor); 

        setPosition(
            sf::Vector2i(
                static_cast<int>(overlay.getPosition().x),
                static_cast<int>(overlay.getPosition().y)
            )
        );

        setOrientation(VERTICAL_ORIENTATION);

        updateFill();
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

            overlay.setOrigin(bgSize.x, 0.0f);
            overlay.setRotation(HORIZONTAL_ROTATION_ANGLE);

            foreground.setOrigin(bgSize.x, 0.0f);
            foreground.setRotation(HORIZONTAL_ROTATION_ANGLE);
            foreground.setPosition(bgPosition.x, bgPosition.y);

            primaryBackground.setOrigin(bgSize.x, 0.0f);
            primaryBackground.setRotation(HORIZONTAL_ROTATION_ANGLE);
            primaryBackground.setPosition(bgPosition.x, bgPosition.y);

            secondaryBackground.setOrigin(bgSize.x - HIGHLIGHT_OFFSET_X, 0.0f);
            secondaryBackground.setRotation(HORIZONTAL_ROTATION_ANGLE);
            secondaryBackground.setPosition(bgPosition.x, bgPosition.y);
        } else if(orientation == VERTICAL_ORIENTATION) {
            const sf::Vector2f bgPosition = overlay.getPosition();

            overlay.setOrigin(0.0f, 0.0f);
            overlay.setRotation(VERTICAL_ROTATION_ANGLE);

            foreground.setOrigin(0.0f, 0.0f);
            foreground.setRotation(VERTICAL_ROTATION_ANGLE);
            foreground.setPosition(bgPosition);

            primaryBackground.setOrigin(0.0f, 0.0f);
            primaryBackground.setRotation(VERTICAL_ROTATION_ANGLE);
            primaryBackground.setPosition(bgPosition);

            secondaryBackground.setOrigin(-HIGHLIGHT_OFFSET_X, 0.0f);
            secondaryBackground.setRotation(VERTICAL_ROTATION_ANGLE);
            secondaryBackground.setPosition(bgPosition);
        }
    }

    void EnergyMeter::setPosition(const sf::Vector2i &newPosition) {
        Widget::setPosition(newPosition);
        overlay.setPosition(static_cast<float>(newPosition.x), static_cast<float>(newPosition.y));
        foreground.setPosition(static_cast<float>(newPosition.x), static_cast<float>(newPosition.y));
        primaryBackground.setPosition(foreground.getPosition());
        secondaryBackground.setPosition(foreground.getPosition().x, foreground.getPosition().y);
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

} // hikari::gui
} // hikari