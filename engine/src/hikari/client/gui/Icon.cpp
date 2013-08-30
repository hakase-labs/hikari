#include "hikari/client/gui/Icon.hpp"

#include <guichan/graphics.hpp>

namespace hikari {
namespace gui { 
    Icon::Icon()
        : gcn::Icon()
        , imageSubrectangle()
        , opaqueFlag(false)
    {

    }

    Icon::Icon(const std::string& filename)
        : gcn::Icon( filename)
        , imageSubrectangle()
        , opaqueFlag(false)
    {
        if(mImage) {
            setSubrectangle(gcn::Rectangle(0, 0, mImage->getWidth(), mImage->getHeight()));
        }
    }

    Icon::Icon(const gcn::Image* image)
        : gcn::Icon(image)
        , imageSubrectangle()
        , opaqueFlag(false)
    {
        if(mImage) {
            setSubrectangle(gcn::Rectangle(0, 0, mImage->getWidth(), mImage->getHeight()));
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


    bool Icon::isOpaque() const {
        return opaqueFlag;
    }

    void Icon::setOpaque(bool opaque) {
        opaqueFlag = opaque;
    }

    void Icon::draw(gcn::Graphics* graphics) {
        // if(isOpaque()) {
        //     graphics->setColor(getBaseColor());
        //     graphics->fillRectangle(0, 0, getWidth(), getHeight());
        // }

        if(mImage) {
            graphics->drawImage(
                mImage,
                imageSubrectangle.x,
                imageSubrectangle.y,
                0,
                0,
                imageSubrectangle.width,
                imageSubrectangle.height
            );
        }
    }
} // gui
} // hikari