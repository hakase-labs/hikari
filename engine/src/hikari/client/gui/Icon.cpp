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
        : gcn::Icon(filename)
        , imageSubrectangle()
        , opaqueFlag(false)
        , offsetX(0)
        , offsetY(0)
    {
        if(mImage) {
            setSubrectangle(gcn::Rectangle(0, 0, mImage->getWidth(), mImage->getHeight()));
        }
    }

    Icon::Icon(const gcn::Image* image)
        : gcn::Icon(image)
        , imageSubrectangle()
        , opaqueFlag(false)
        , offsetX(0)
        , offsetY(0)
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

    void Icon::setImageOffset(int x, int y) {
        offsetX = x;
        offsetY = y;
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

    void Icon::_draw(gcn::Graphics* graphics) {
        if (mFrameSize > 0) {
            gcn::Rectangle rec = mDimension;
            rec.x -= mFrameSize;
            rec.y -= mFrameSize;
            rec.width += 2 * mFrameSize;
            rec.height += 2 * mFrameSize;
            graphics->pushClipArea(rec);
            drawFrame(graphics);
            graphics->popClipArea();
        }

        gcn::Rectangle rec = mDimension;
        rec.x -= offsetX;
        rec.y -= offsetY;

        graphics->pushClipArea(rec);
        draw(graphics);

        const gcn::Rectangle& childrenArea = getChildrenArea();
        graphics->pushClipArea(childrenArea);

        std::list<Widget*>::const_iterator iter;
        for (iter = mChildren.begin(); iter != mChildren.end(); iter++)
        {
            gcn::Widget* widget = (*iter);
            // Only draw a widget if it's visible and if it visible
            // inside the children area.
            if (widget->isVisible() && childrenArea.isIntersecting(widget->getDimension()))
                widget->_draw(graphics);
        }

        graphics->popClipArea();
        graphics->popClipArea();
    }
} // gui
} // hikari