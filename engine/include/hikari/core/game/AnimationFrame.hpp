#ifndef HIKARI_CORE_GAME_ANIMATIONFRAME
#define HIKARI_CORE_GAME_ANIMATIONFRAME

#include "hikari/core/Platform.hpp"
#include "hikari/core/geom/Point2D.hpp"
#include "hikari/core/geom/Rectangle2D.hpp"

namespace hikari {

    class HIKARI_API AnimationFrame {
    private:
        Rectangle2D<int> sourceRect;
        Point2D<int> hotspot;
        float displayTime;

    public:
        AnimationFrame(const Rectangle2D<int> &sourceRect, const float &displayTime, const Point2D<int> &hotspot = Point2D<int>(0, 0));

        const Rectangle2D<int>& getSourceRectangle() const;
        const Point2D<int>& getHotspot() const;
        const float& getDisplayTime() const;

        void setSourceRectangle(const Rectangle2D<int> &newSourceRect);
        void setHotspot(const Point2D<int> &newHotspot);
        void setDisplayTime(const float &newTime);
    };

} // hikari

#endif // HIKARI_CORE_GAME_ANIMATIONFRAME