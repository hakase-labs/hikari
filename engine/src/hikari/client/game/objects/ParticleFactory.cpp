#include "hikari/client/game/objects/ParticleFactory.hpp"
#include "hikari/client/game/objects/GameObject.hpp"
#include "hikari/client/game/objects/Particle.hpp"

#include "hikari/core/game/AnimationLoader.hpp"
#include "hikari/core/util/AnimationSetCache.hpp"
#include "hikari/core/util/ImageCache.hpp"

namespace hikari {

    ParticleFactory::ParticleFactory(const std::weak_ptr<AnimationSetCache>& animationSetCache, const std::weak_ptr<ImageCache>& imageCache)
        : Service()
        , animationSetCache(animationSetCache)
        , imageCache(imageCache)
        , prototypeRegistry()
    {
         
    }
    
    ParticleFactory::~ParticleFactory() {

    }

    std::unique_ptr<Particle> ParticleFactory::create(const std::string& ParticleType) {
        auto prototype = prototypeRegistry.find(ParticleType);

        if(prototype != std::end(prototypeRegistry)) {
            return (*prototype).second->clone();
        } else {
            // TODO: Return a "default" item so no nullptrs will be made?
            return std::unique_ptr<Particle>(nullptr);
        }
    }

    void ParticleFactory::registerPrototype(const std::string & prototypeName, const std::shared_ptr<Particle> & instance) {
        if(prototypeRegistry.find(prototypeName) == std::end(prototypeRegistry)) {
            prototypeRegistry.insert(std::make_pair(prototypeName, instance));
        } else {
            // Already registered; exception?
        }
    }
} // hikari