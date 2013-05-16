#include "hikari/client/gui/EnergyGague.hpp"

#include <guichan/graphics.hpp>
#include <guichan/color.hpp>
#include <guichan/image.hpp>

namespace hikari {

    const unsigned int EnergyGague::DEFAULT_WIDTH = 8;
    const unsigned int EnergyGague::DEFAULT_HEIGHT = 56;
    const float EnergyGague::DEFAULT_VALUE = 100.0f;
    const float EnergyGague::DEFAULT_MAXIMUM_VALUE = 100.0f;

    EnergyGague::EnergyGague()
        : gcn::Widget()
        , value(DEFAULT_VALUE)
        , maximumValue(DEFAULT_MAXIMUM_VALUE)
    {
        setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
        setBaseColor(gcn::Color(0x000000));
        setBackgroundColor(gcn::Color(0xfcd8a8));
        setForegroundColor(gcn::Color(0xfcfcfc));
    }

    EnergyGague::EnergyGague(float maximumValue)
        : gcn::Widget()
        , value(DEFAULT_VALUE)
        , maximumValue(maximumValue)
    {
        setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
        setBaseColor(gcn::Color(0x000000));
        setBackgroundColor(gcn::Color(0xfcd8a8));
        setForegroundColor(gcn::Color(0xfcfcfc));
    }

    EnergyGague::~EnergyGague() {

    }

    float EnergyGague::getValue() const {
        return value;
    }

    float EnergyGague::getMaximumValue() const {
        return maximumValue;
    }

    void EnergyGague::setValue(float value) {
        this->value = value;
    }

    void EnergyGague::setMaximumValue(float maximumValue) {
        this->maximumValue = maximumValue;
    }

    void EnergyGague::draw(gcn::Graphics* graphics) {
        graphics->setColor(getBackgroundColor());
        graphics->fillRectangle(0, 0, getWidth(), getHeight());

        graphics->setColor(getForegroundColor());
        graphics->fillRectangle(3, 0, 2, getHeight());

        graphics->setColor(getBaseColor());

        // Draw horizontal ticks
        for(unsigned int row = 0, height = getHeight(); row < height; ++row) {
            if(row % 2 == 1) {
                graphics->drawLine(0, row, getWidth(), row);
            }
        }

        // Draw overlay rectangle
        float percentageFilled = value / maximumValue;

        int fillHeight = getHeight() - static_cast<int>(percentageFilled * static_cast<float>(getHeight()));

        graphics->fillRectangle(0, 0, getWidth(), fillHeight);

        // Draw border
        graphics->drawLine(0, 0, 0, getHeight());
        graphics->drawLine(getWidth() - 1, 0, getWidth() - 1, getHeight());
    }

} // hikari