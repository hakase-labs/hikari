#include <hikari/core/game/AnimationSet.hpp>

namespace hikari {

    const AnimationPtr AnimationSet::NULL_ANIMATION = AnimationPtr();

    AnimationSet::AnimationSet(const std::string& name, const std::string& imageFileName)
        : name(name)
        , imageFileName(imageFileName) {

    }

    const std::string& AnimationSet::getName() const {
        return name;
    }

    const std::string& AnimationSet::getImageFileName() const {
        return imageFileName;
    }

    bool AnimationSet::add(const std::string& name, const AnimationPtr& animation) {
        if(has(name)) {
            return false;
        }

        animationMap.insert(
            std::make_pair<std::string, AnimationPtr>(name, animation)
        );

        return true;
    }

    bool AnimationSet::has(const std::string& name) const {
        return animationMap.find(name) != animationMap.end();
    }

    bool AnimationSet::remove(const std::string& name) {
        animationMap.erase(name);

        return true;
    }

    const AnimationPtr& AnimationSet::get(const std::string& name) {
        if(has(name)) {
            return animationMap.find(name)->second;
        }

        return NULL_ANIMATION;
    }

} // hikari