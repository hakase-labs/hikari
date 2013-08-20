#include "hikari/client/gui/Icon.hpp"

#include <guichan/graphics.hpp>

namespace hikari {
namespace gui { 
    Icon::Icon()
        : gcn::Icon()
        , imageSubrectangle()
    {

    }

    Icon::Icon(const std::string& filename)
        : gcn::Icon( filename)
        , imageSubrectangle()
    {
        if(mImage) {
            imageSubrectangle = gcn::Rectangle(0, 0, mImage->getWidth(), mImage->getHeight());
        }
    }

    Icon::Icon(const gcn::Image* image)
        : gcn::Icon(image)
        , imageSubrectangle()
    {
        if(mImage) {
            imageSubrectangle = gcn::Rectangle(0, 0, mImage->getWidth(), mImage->getHeight());
        }
    }

    Icon::~Icon() {

    }

    gcn::Rectangle Icon::getSubrectangle() const {
        return imageSubrectangle;
    }

    void Icon::setSubrectangle(const gcn::Rectangle & rect) {
        imageSubrectangle = rect;
        setWidth(imageSubrectangle.width);
        setHeight(imageSubrectangle.height);
    }

    void Icon::draw(gcn::Graphics* graphics) {
        if(isOpaque()) {
            graphics->setColor(getBaseColor());
            graphics->fillRectangle(getX(), getX(), getWidth(), getHeight());
        }

        if(mImage) {
            graphics->drawImage(
                mImage,
                imageSubrectangle.x,
                imageSubrectangle.y,
                getX(),
                getY(),
                imageSubrectangle.width,
                imageSubrectangle.height
            );
        }
    }
} // gui
} // hikari