#include "hikari/client/gui/EnergyGauge.hpp"

#include <guichan/graphics.hpp>
#include <guichan/color.hpp>
#include <guichan/image.hpp>

namespace hikari {
namespace gui {

    const unsigned int EnergyGauge::DEFAULT_WIDTH = 8;
    const unsigned int EnergyGauge::DEFAULT_HEIGHT = 56;
    const unsigned int EnergyGauge::DEFAULT_BASE_COLOR = 0x000000;
    const unsigned int EnergyGauge::DEFAULT_FOREGROUND_COLOR = 0xfcd8a8;
    const unsigned int EnergyGauge::DEFAULT_BACKGROUND_COLOR = 0xfcfcfc;
    const float EnergyGauge::DEFAULT_VALUE = 100.0f;
    const float EnergyGauge::DEFAULT_MAXIMUM_VALUE = 100.0f;

    EnergyGauge::EnergyGauge(float maximumValue)
        : gcn::Widget()
        , value(DEFAULT_VALUE)
        , maximumValue(maximumValue)
        , orientation(Orientation::VERTICAL)
    {
        initialize();
    }

    EnergyGauge::~EnergyGauge() {

    }

    void EnergyGauge::initialize() {
        setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
        setBaseColor(gcn::Color(DEFAULT_BASE_COLOR));
        setBackgroundColor(gcn::Color(DEFAULT_FOREGROUND_COLOR));
        setForegroundColor(gcn::Color(DEFAULT_BACKGROUND_COLOR));
    }

    float EnergyGauge::getValue() const {
        return value;
    }

    float EnergyGauge::getMaximumValue() const {
        return maximumValue;
    }

    Orientation::Type EnergyGauge::getOrientation() const {
        return orientation;
    }

    void EnergyGauge::setValue(float value) {
        this->value = std::min(value, getMaximumValue());
    }

    void EnergyGauge::setMaximumValue(float maximumValue) {
        this->maximumValue = std::max(0.0f, maximumValue);
    }

    void EnergyGauge::setOrientation(Orientation::Type orientation) {
        this->orientation = orientation;
    }

    void EnergyGauge::draw(gcn::Graphics* graphics) {
        int highlightThickness = (getOrientation() == Orientation::VERTICAL ? getWidth() : getHeight() ) / 4;   // width of the highlight stripe, typically 2 pixels
        const float percentageFilled = value / maximumValue;

        // Draw the background of the gague
        graphics->setColor(getBackgroundColor());
        graphics->fillRectangle(0, 0, getWidth(), getHeight());

        if(getOrientation() == Orientation::VERTICAL) {
            // Draw the highlight stripe
            graphics->setColor(getForegroundColor());
            const int highlightStartX = (getWidth() / 2) - (highlightThickness / 2);
            graphics->fillRectangle(highlightStartX, 0, highlightThickness, getHeight());

            graphics->setColor(getBaseColor());
        
            // Draw horizontal ticks
            for(unsigned int row = 0, height = getHeight(); row < height; ++row) {
                if(row % 2 == 1) {
                    graphics->drawLine(0, row, getWidth(), row);
                }
            }

            // Draw overlay rectangle
            int fillHeight = getHeight() - static_cast<int>(percentageFilled * static_cast<float>(getHeight()));
            graphics->fillRectangle(0, 0, getWidth(), fillHeight);

            // Draw border
            graphics->drawLine(0, 0, 0, getHeight());
            graphics->drawLine(getWidth() - 1, 0, getWidth() - 1, getHeight());
        } else {
            // Draw horizontal orientation
            graphics->setColor(getForegroundColor());
            const int highlightStartY = (getHeight() / 2) - (highlightThickness / 2);
            graphics->fillRectangle(0, highlightStartY, getWidth(), highlightThickness);

            graphics->setColor(getBaseColor());
        
            // Draw horizontal ticks
            for(unsigned int column = 0, width = getWidth(); column < width; ++column) {
                if(column % 2 == 1) {
                    graphics->drawLine(column, 0, column, getHeight());
                }
            }

            // Draw overlay rectangle
            int fillWidth = getWidth() - static_cast<int>(percentageFilled * static_cast<float>(getWidth()));
            graphics->fillRectangle(0, 0, fillWidth, getHeight());

            // Draw border
            graphics->drawLine(0, 0, getWidth(), 0);
            graphics->drawLine(0, getHeight() - 1, getWidth(), getHeight() - 1);
        }
    }

} // hikari::gui
} // hikari
