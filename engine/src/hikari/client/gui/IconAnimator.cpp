#include "hikari/client/gui/IconAnimator.hpp"
#include "hikari/client/gui/Icon.hpp"
#include "hikari/core/game/Animation.hpp"
#include <guichan/rectangle.hpp>
#include <cmath>

namespace hikari {
namespace gui {

    IconAnimator::IconAnimator(Icon & icon)
        : Animator()
        , icon(icon) {

    }

    IconAnimator::~IconAnimator() {

    }

    void IconAnimator::update(float delta) {
        Animator::update(delta);

        if(const auto& animation = getAnimation()) {
            const auto& currentFrame = animation->getFrameAt(getCurrentFrameIndex());
            const auto& currentFrameRectangle = currentFrame.getSourceRectangle();
            const auto& currentFrameHotSpot = currentFrame.getHotspot();
            const gcn::Rectangle frameDimension(
                currentFrameRectangle.getX(),
                currentFrameRectangle.getY(),
                currentFrameRectangle.getWidth(),
                currentFrameRectangle.getHeight()
            );

            icon.setSubrectangle(frameDimension);
            icon.setImageOffset(currentFrameHotSpot.getX(), currentFrameHotSpot.getY());
            //icon.setDimension(frameDimension);
            //icon.setFrameSize(std::max(currentFrameHotSpot.getX(), currentFrameHotSpot.getY()));
        }
    }

} // gui
} // hikari