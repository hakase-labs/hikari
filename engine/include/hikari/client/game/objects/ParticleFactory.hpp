#ifndef HIKARI_CLIENT_GAME_OBJECT_PARTICLEFACTORY
#define HIKARI_CLIENT_GAME_OBJECT_PARTICLEFACTORY

#include <memory>
#include <string>
#include <unordered_map>

#include "hikari/core/util/Service.hpp"

namespace hikari {

    class AnimationSetCache;
    class ImageCache;
    class Particle;

    class ParticleFactory : public Service {
    private:
        //
        // Fields
        //
        std::weak_ptr<AnimationSetCache> animationSetCache;
        std::weak_ptr<ImageCache> imageCache;
        std::unordered_map<std::string, std::shared_ptr<Particle>> prototypeRegistry;

    public:
        //
        // Constructor
        //
        ParticleFactory(const std::weak_ptr<AnimationSetCache>& animationSetCache,
            const std::weak_ptr<ImageCache>& imageCache);
        virtual ~ParticleFactory();

        //
        // Methods
        //
        std::unique_ptr<Particle> create(const std::string& enemyType);

        void registerPrototype(const std::string & prototypeName, const std::shared_ptr<Particle> & instancee);
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECT_PARTICLEFACTORY