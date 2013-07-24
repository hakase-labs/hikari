#ifndef HIKARI_CORE_UTIL_ANIMATIONSETCACHE
#define HIKARI_CORE_UTIL_ANIMATIONSETCACHE

#include "hikari/core/Platform.hpp"
#include "hikari/core/util/ResourceCache.hpp"
#include "hikari/core/util/Service.hpp"

#include "hikari/core/game/AnimationSet.hpp"

#include <memory>

namespace hikari {

    class AnimationLoader;

    class HIKARI_API AnimationSetCache : public Service, public ResourceCache<AnimationSet> {
    private:
        std::shared_ptr<AnimationLoader> loader;

    protected:
        virtual AnimationSetCache::Resource loadResource(const std::string &fileName);

    public:
        AnimationSetCache(const std::shared_ptr<AnimationLoader> & loader);

        virtual ~AnimationSetCache() { }
    };

} // hikari

#endif // HIKARI_CORE_UTIL_ANIMATIONSETCACHE