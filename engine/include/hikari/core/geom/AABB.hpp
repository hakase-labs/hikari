#ifndef HIKARI_CORE_GEOM_AABB
#define HIKARI_CORE_GEOM_AABB

#include <hikari/core/Platform.hpp>

namespace hikari {

    class Vector2D;

    class HIKARI_API AABB {
    public:
        virtual ~AABB();
        virtual const Vector2D& getCenter() const = 0;
        virtual const Vector2D& getHalfExtents() const = 0;

        static bool overlap(const AABB& left, const AABB& right);
    };

}

#endif // HIKARI_CORE_GEOM_AABB