#ifndef HIKARI_CORE_GAME_MAP_CAMERA_FOLLOWINGCAMERASTRATEGY
#define HIKARI_CORE_GAME_MAP_CAMERA_FOLLOWINGCAMERASTRATEGY

#include "hikari/core/Platform.hpp"
#include "hikari/core/game/map/camera/CameraStrategy.hpp"

namespace hikari {

    class HIKARI_API FollowingCameraStrategy : public CameraStrategy {
    public:
        virtual ~FollowingCameraStrategy() {}
        virtual void execute();
    };

} // hikari

#endif // HIKARI_CORE_GAME_MAP_CAMERA_FOLLOWINGCAMERASTRATEGY