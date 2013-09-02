#include "hikari/client/gui/WeaponMenuItem.hpp"
#include "hikari/client/gui/EnergyGauge.hpp"
#include "hikari/client/gui/Icon.hpp"
#include "hikari/client/gui/Orientation.hpp"

#include <guichan/graphics.hpp>
#include <guichan/font.hpp>

namespace hikari {
namespace gui {

    const int WeaponMenuItem::DEFAULT_SELECT_BOX_WIDTH = 16;
    const int WeaponMenuItem::DEFAULT_SELECT_BOX_HEIGHT = 16;
    const int WeaponMenuItem::DEFAULT_SELECT_BOX_SPACING = 9;

    WeaponMenuItem::WeaponMenuItem()
        : MenuItem()
        , selectBoxWidth(DEFAULT_SELECT_BOX_WIDTH)
        , selectBoxHeight(DEFAULT_SELECT_BOX_HEIGHT)
        , selectBoxSpacing(DEFAULT_SELECT_BOX_SPACING)
        , gauge(new EnergyGauge())
    {
        initGauge();
        adjustSize();
    }

    WeaponMenuItem::WeaponMenuItem(const std::string & name)
        : MenuItem(name)
        , selectBoxWidth(DEFAULT_SELECT_BOX_WIDTH)
        , selectBoxHeight(DEFAULT_SELECT_BOX_HEIGHT)
        , selectBoxSpacing(DEFAULT_SELECT_BOX_SPACING)
        , gauge(new EnergyGauge())
    {
        initGauge();
        adjustSize();
    }

    WeaponMenuItem::~WeaponMenuItem() {

    }

    void WeaponMenuItem::initGauge() {
        gauge->setOrientation(Orientation::HORIZONTAL);
        gauge->setWidth(56);
        gauge->setHeight(8);
        gauge->setX(24);
        gauge->setY(8);

        add(gauge.get());
    }

    void WeaponMenuItem::adjustSize() {
        MenuItem::adjustSize();
        setHeight(selectBoxHeight);

        int labelWidth = getFont()->getWidth(getName());
        int gaugeWidth = gauge->getWidth();

        setWidth(std::max(labelWidth, gaugeWidth) + selectBoxWidth + selectBoxSpacing);
    }

    void WeaponMenuItem::draw(gcn::Graphics* graphics) {
        gcn::Color boxOutlineColor(0, 112, 236);
        gcn::Color boxBackgroundColor(128, 208, 16);

        if(!isSelected()) {
            boxOutlineColor = gcn::Color(188, 188, 188);
            boxBackgroundColor = gcn::Color(116, 116, 116);
            gauge->setForegroundColor(gcn::Color(0xfcfcfc));
            gauge->setBackgroundColor(gcn::Color(0xfcd8a8));
        } else {
            gauge->setForegroundColor(gcn::Color(0x00e8d8));
            gauge->setBackgroundColor(gcn::Color(0, 112, 236));
        }

        // Draw icon box
        graphics->setColor(boxOutlineColor);
        graphics->drawLine(1, 0, selectBoxWidth - 2, 0);                                        // top
        graphics->drawLine(selectBoxWidth - 1, 1, selectBoxWidth - 1, selectBoxHeight - 2);     // right
        graphics->drawLine(1, selectBoxHeight - 1, selectBoxWidth - 2, selectBoxHeight - 1);    // bottom
        graphics->drawLine(0, 1, 0, selectBoxHeight - 2);                                       // left

        graphics->setColor(boxBackgroundColor);
        graphics->fillRectangle(2, 2, selectBoxWidth - 4, selectBoxHeight - 4);

        graphics->setFont(getFont());
        graphics->drawText(getName(), selectBoxWidth + selectBoxSpacing, 0, gcn::Graphics::Left);
    }

} // hikari::gui
} // hikari
