#include "hikari/client/gui/IconAnimator.hpp"
#include "hikari/client/gui/Icon.hpp"
#include "hikari/core/game/Animation.hpp"
#include <guichan/rectangle.hpp>

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

            icon.setSubrectangle(gcn::Rectangle(
                currentFrameRectangle.getX(),
                currentFrameRectangle.getY(),
                currentFrameRectangle.getWidth(),
                currentFrameRectangle.getHeight()
            ));
        }
    }

} // gui
} // hikari