#include <hikari/core/util/AnimationSetCache.hpp>
#include <hikari/core/game/AnimationLoader.hpp>
#include <hikari/core/util/Log.hpp>

namespace hikari {

    AnimationSetCache::AnimationSetCache() {

    }

    AnimationSetCache::Resource AnimationSetCache::loadResource(const std::string &fileName) {
        HIKARI_LOG(debug) << "Caching animation set: " << fileName;

        Resource animationSet = AnimationLoader::loadSet(fileName);

        return animationSet;
    }

} // hikari