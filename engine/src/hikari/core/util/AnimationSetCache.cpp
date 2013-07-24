#include "hikari/core/util/AnimationSetCache.hpp"
#include "hikari/core/game/AnimationLoader.hpp"
#include "hikari/core/util/Log.hpp"

namespace hikari {

    AnimationSetCache::AnimationSetCache(const std::shared_ptr<AnimationLoader> & loader)
        : loader(loader)
    {

    }

    AnimationSetCache::Resource AnimationSetCache::loadResource(const std::string &fileName) {
        HIKARI_LOG(debug) << "Caching animation set: " << fileName;

        Resource animationSet;

        if(loader) {
            animationSet = loader->loadSet(fileName);
        }

        return animationSet;
    }

} // hikari