#ifndef HIKARI_CORE_GAME_MAP_FORCE
#define HIKARI_CORE_GAME_MAP_FORCE

#include "hikari/core/Platform.hpp"
#include "hikari/core/geom/BoundingBox.hpp"
#include "hikari/core/math/Vector2.hpp"

namespace hikari {

    class HIKARI_API Force {
    private:
        BoundingBox<float> bounds;
        Vector2<float> velocity;
        bool activeFlag;

    protected:
        void setVelocity(const Vector2<float> & vel);

    public:
        Force(const BoundingBox<float> & bounds, const Vector2<float> & velocity);

        const BoundingBox<float> & getBounds() const;
        const Vector2<float> & getVelocity() const;
        bool isActive() const;

        void enable();
        void disable();
    };

} // hikari

#endif // HIKARI_CORE_GAME_MAP_FORCE
