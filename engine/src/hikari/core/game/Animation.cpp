#include <hikari/core/game/Animation.hpp>

namespace hikari {

    const bool Animation::ANIMATION_DEFAULT_REPEAT_SETTING = false;
    const unsigned int Animation::ANIMATION_BEGINNING_FRAME_INDEX = 0;
    const unsigned int Animation::ANIMATION_DEFAULT_SYNC_GROUP = 0;

    Animation::Animation(const FrameList &frames, bool doesRepeat, unsigned int keyframe, unsigned int syncGroup)
        : frames(frames)
        , repeat(doesRepeat)
        , keyframe(keyframe)
        , syncGroup(syncGroup) {

    }

    bool Animation::doesRepeat() const {
        return repeat;
    }

    unsigned int Animation::getKeyframeIndex() const {
        return keyframe;
    }

    unsigned int Animation::getNumberOfFrames() const {
        return frames.size();
    }

    unsigned int Animation::getSyncGroup() const {
        return syncGroup;
    }

    const Frame& Animation::getFrameAt(unsigned int index) const {
        return frames.at(index);
    }

} // hikari