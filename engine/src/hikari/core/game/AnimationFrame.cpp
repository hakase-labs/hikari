#include "hikari/core/game/AnimationFrame.hpp"

namespace hikari {
    
    AnimationFrame::AnimationFrame(const Rectangle2D<int> &sourceRect, const float &displayTime, const Point2D<int> &hotspot)
        : sourceRect(sourceRect)
        , hotspot(hotspot)
        , displayTime(displayTime)
    {

    }

    const Rectangle2D<int>& AnimationFrame::getSourceRectangle() const {
        return sourceRect;
    }

    const Point2D<int>& AnimationFrame::getHotspot() const {
        return hotspot;
    }

    const float& AnimationFrame::getDisplayTime() const {
        return displayTime;
    }

    void AnimationFrame::setSourceRectangle(const Rectangle2D<int> &newSourceRect) {
        sourceRect = newSourceRect;
    }

    void AnimationFrame::setHotspot(const Point2D<int> &newHotspot) {
        hotspot = newHotspot;
    }

    void AnimationFrame::setDisplayTime(const float &newTime) {
        displayTime = newTime;
    }

}