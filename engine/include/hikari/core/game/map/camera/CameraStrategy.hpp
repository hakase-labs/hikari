#ifndef HIKARI_CORE_GAME_MAP_CAMERA_CAMERASTRATEGY
#define HIKARI_CORE_GAME_MAP_CAMERA_CAMERASTRATEGY

#include "hikari/core/Platform.hpp"

namespace hikari {

    class HIKARI_API CameraStrategy {
    public:
        virtual ~CameraStrategy() {}
        virtual void execute();
    };

} // hikari

#endif // HIKARI_CORE_GAME_MAP_CAMERA_CAMERASTRATEGY