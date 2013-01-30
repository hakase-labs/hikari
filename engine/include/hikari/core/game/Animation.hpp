#ifndef HIKARI_CORE_GAME_ANIMATION
#define HIKARI_CORE_GAME_ANIMATION

#include "hikari/core/Platform.hpp"
#include "hikari/core/game/AnimationFrame.hpp"
#include <vector>
#include <utility>

#pragma warning(push)
#pragma warning(disable:4251)

namespace hikari {

    typedef AnimationFrame Frame;
    typedef std::vector< Frame > FrameList;

    class HIKARI_API Animation {
    private:
        bool repeat;
        unsigned int keyframe;
        unsigned int syncGroup;
        FrameList frames;

    public:
        Animation(
            const FrameList &frames, 
            bool doesRepeat = ANIMATION_DEFAULT_REPEAT_SETTING, 
            unsigned int keyframe = ANIMATION_BEGINNING_FRAME_INDEX, 
            unsigned int syncGroup = ANIMATION_DEFAULT_SYNC_GROUP
        );

        bool doesRepeat() const;
        unsigned int getKeyframeIndex() const;
        unsigned int getNumberOfFrames() const;
        unsigned int getSyncGroup() const;
        const Frame& getFrameAt(unsigned int index) const;

        static const bool ANIMATION_DEFAULT_REPEAT_SETTING;
        static const unsigned int ANIMATION_BEGINNING_FRAME_INDEX;
        static const unsigned int ANIMATION_DEFAULT_SYNC_GROUP;
    };

} // hikari

#pragma warning(pop)

#endif // HIKARI_CORE_GAME_ANIMATION