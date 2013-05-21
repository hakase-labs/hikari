#include "hikari/client/gui/Panel.hpp"

#include <guichan/graphics.hpp>
#include <guichan/color.hpp>
#include <guichan/image.hpp>

namespace hikari {
namespace gui {

    Panel::Panel()
        : gcn::Container()
        , innerBorderColor(gcn::Color(255, 255, 255, 255))
        , shadowColor(gcn::Color(0, 0, 255, 64))
        , drawShadow(false)
    {
    }

    Panel::~Panel() {

    }

    void Panel::drawInnerBorder(gcn::Graphics* graphics) const {
        graphics->setColor(innerBorderColor);

        // Draw inner border
        graphics->drawRectangle(gcn::Rectangle(1, 1, getWidth() - 2, getHeight() - 2));
        graphics->drawRectangle(gcn::Rectangle(2, 2, getWidth() - 4, getHeight() - 4));
    }

    void Panel::draw(gcn::Graphics* graphics) {
        gcn::Container::draw(graphics);
        drawInnerBorder(graphics);
    }

} // hikari::gui
} // hikari
