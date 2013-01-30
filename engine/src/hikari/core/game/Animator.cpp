#include "hikari/core/game/Animator.hpp"
#include "hikari/core/game/Animation.hpp"
#include "hikari/core/game/AnimationPlaybackException.hpp"

namespace hikari {
    
    Animator::Animator() 
        : paused(false)
        , currentFrameIndex(Animation::ANIMATION_BEGINNING_FRAME_INDEX)
        , timeElapsed(0.0f)
        , animation()
        , callback() {

    }

    Animator::~Animator() {

    }

    float Animator::getTimeElapsed() const {
        return timeElapsed;
    }

    unsigned int Animator::getCurrentFrameIndex() const {
        return currentFrameIndex;
    }

    std::shared_ptr<Animation> Animator::getAnimation() const {
        return animation;
    }

    void Animator::play(float delta) {
        timeElapsed += delta;

        float frameDuration = getCurrentFrameDuration();

        while(timeElapsed >= frameDuration) {
            currentFrameIndex += 1;
            timeElapsed -= frameDuration;
            handleEndOfAnimation();
            frameDuration = getCurrentFrameDuration();
        }
    }

    float Animator::getCurrentFrameDuration() const {
        if(getCurrentFrameIndex() < animation->getNumberOfFrames()) {
            return animation->getFrameAt(getCurrentFrameIndex()).getDisplayTime();
        } else {
            throw AnimationPlaybackException("Current frame index exceeds length of animation. Animator must be reset.");
        }
    }

    void Animator::handleEndOfAnimation() {
        if(currentFrameIndex >= animation->getNumberOfFrames()) {
            if(animation->doesRepeat()) {
                currentFrameIndex = animation->getKeyframeIndex();
            } else {
                currentFrameIndex -= 1;
            }

            if(callback) {
                callback(*animation);
            }
        }
    }

    void Animator::rewind() {
        currentFrameIndex = 0;
        timeElapsed = 0.0f;
    }

    void Animator::pause() {
        paused = true;
    }

    const bool Animator::isPaused() const {
        return paused;
    }

    void Animator::unpause() {
        paused = false;
    }

    void Animator::update(float delta) {
        if(animation && !isPaused()) {
            play(delta);    
        }
    }

    void Animator::setAnimation(std::shared_ptr<Animation> animation) {
        // Setting animation for the first time
        if(!this->animation) {
            this->animation = animation;
        }

        // Changing animation
        if(animation) {
            bool syncGroupChanged = this->animation->getSyncGroup() != animation->getSyncGroup();

            // FYI: Animations with default sync group always trigger a rewind()
            if(syncGroupChanged || animation->getSyncGroup() == Animation::ANIMATION_DEFAULT_SYNC_GROUP) {
                rewind();
            }

            this->animation = animation;
        } else {
            // TODO: thrown exception or what?
        }

        update(0.0f);
    }

}