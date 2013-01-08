#ifndef HIKARI_CORE_UTIL_ANIMATIONSETCACHE
#define HIKARI_CORE_UTIL_ANIMATIONSETCACHE

#include <hikari/core/Platform.hpp>
#include <hikari/core/util/ResourceCache.hpp>
#include <hikari/core/util/Service.hpp>

#include <hikari/core/game/AnimationSet.hpp>

namespace hikari {

    class HIKARI_API AnimationSetCache : public Service, public ResourceCache<AnimationSet> {
    private:
    protected:
        virtual AnimationSetCache::Resource loadResource(const std::string &fileName);

    public:
        AnimationSetCache();

        virtual ~AnimationSetCache() { }
    };

} // hikari

#endif // HIKARI_CORE_UTIL_ANIMATIONSETCACHE