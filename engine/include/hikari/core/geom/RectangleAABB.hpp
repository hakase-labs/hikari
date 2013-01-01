#ifndef HIKARI_CORE_GAME_MAP_RECTANGLEAABB
#define HIKARI_CORE_GAME_MAP_RECTANGLEAABB

#include <hikari/core/Platform.hpp>
#include <hikari/core/geom/AABB.hpp>
#include <hikari/core/geom/Vector2D.hpp>

namespace hikari {

    class HIKARI_API RectangleAABB : public AABB {
    private:
        Vector2D center;
        Vector2D halfExtents;

    public:
        RectangleAABB(const Vector2D& center, const Vector2D& halfExtents);

        virtual ~RectangleAABB();

        const Vector2D& getCenter() const;
        const Vector2D& getHalfExtents() const;
        
        void setCenter(const Vector2D& newCenter);
        void setHalfExtents(const Vector2D& newHalfExtents);

        Vector2D getBottomLeft();
        Vector2D getBottomRight();
        Vector2D getTopLeft();
        Vector2D getTopRight();
        
        Vector2D getMin();
        Vector2D getMax();
    };

}

#endif // HIKARI_CORE_GAME_MAP_RECTANGLEAABB