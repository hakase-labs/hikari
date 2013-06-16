#ifndef HIKARI_CLIENT_GAME_OBJECTS_PARTICLE
#define HIKARI_CLIENT_GAME_OBJECTS_PARTICLE

#include "hikari/client/game/objects/GameObject.hpp"
#include "hikari/core/geom/BoundingBox.hpp"
#include "hikari/core/math/Vector2.hpp"
#include "hikari/core/game/Animator.hpp"

#include <SFML/Graphics/Sprite.hpp>

#include <memory>

namespace sf {
    class RenderTarget;
}

namespace hikari {
    class Animation;
    class AnimationSet;

    /**
     * Particles are non-interactive sprites that have specific lifespans.
     */
    class Particle : public GameObject {
    private:
        float age;
        float maximumAge;
        Vector2<float> velocity;
        BoundingBox<float> boundingBox;

        sf::Sprite sprite;
        std::weak_ptr<Animation> animation;
        std::weak_ptr<AnimationSet> animationSet;
        Animator animator;

    public:
        explicit Particle(float maximumAge);
        Particle(const Particle& proto); 
        virtual ~Particle();

        virtual void update(float dt);
        virtual void render(sf::RenderTarget &target);

        void setVelocity(const Vector2<float> velocity);
        const Vector2<float> getVelocity() const;
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_PARTICLE
