#ifndef HIKARI_CORE_GAME_MAP_TILECOLLISIONRESOLVER
#define HIKARI_CORE_GAME_MAP_TILECOLLISIONRESOLVER

#include <hikari/core/Platform.hpp>
#include <memory>
#include <functional>

namespace hikari {

    class AABB;
    class Room;
    class Vector2D;

    class HIKARI_API TileCollisionResolver {
//    private:
    //    static RectangleAABB tileAabb;
    public:
        typedef std::function<void(const AABB&, const int&, const float&, const int&, const int&)> TileCollisionHandler;
        static void checkTilesWithinAabb(const Room& room, const Vector2D& min, const Vector2D& max, TileCollisionHandler handler, const float& dt);
    };

} // hikari

#endif