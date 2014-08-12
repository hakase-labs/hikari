#ifndef HIKARI_CLIENT_GAME_OBJECTS_PARTICLE
#define HIKARI_CLIENT_GAME_OBJECTS_PARTICLE

#include "hikari/client/game/objects/GameObject.hpp"
#include "hikari/core/geom/BoundingBox.hpp"
#include "hikari/core/math/Vector2.hpp"
#include "hikari/core/game/Renderable.hpp"
#include "hikari/core/game/Animator.hpp"
#include "hikari/core/util/Cloneable.hpp"

#include <SFML/Graphics/Sprite.hpp>

#include <memory>

namespace sf {
    class RenderTarget;
    class Texture;
}

namespace hikari {
    class Animation;
    class AnimationSet;
    class Entity;

    /**
     * Particles are non-interactive sprites that have specific lifespans.
     */
    class Particle : public GameObject, public Renderable, public Cloneable<Particle> {
    private:
        int zIndex;
        float age;
        float maximumAge;
        Vector2<float> velocity;
        BoundingBox<float> boundingBox;

        sf::Sprite sprite;
        std::shared_ptr<sf::Texture> spriteTexture;
        std::weak_ptr<Animation> animation;
        std::weak_ptr<AnimationSet> animationSet;
        std::unique_ptr<Animator> animator;

        std::weak_ptr<Entity> trackedObject;
        bool trackX;
        bool trackY;

    public:
        explicit Particle(float maximumAge);
        Particle(const Particle& proto);
        virtual ~Particle();

        virtual std::unique_ptr<Particle> clone() const;

        virtual void update(float dt);
        virtual void render(sf::RenderTarget &target);
        virtual int getZIndex() const;
        virtual void setZIndex(int index);

        void setMaximumAge(float maximumAge);
        float getMaximumAge() const;

        void setPosition(const Vector2<float> & position);
        const Vector2<float> & getPosition() const;

        void setVelocity(const Vector2<float> & velocity);
        const Vector2<float> & getVelocity() const;

        void setBoundingBox(const BoundingBox<float> & boundingBox);
        const BoundingBox<float> & getBoundingBox() const;

        void setAnimationSet(const std::weak_ptr<AnimationSet> & animationSet);
        const std::weak_ptr<AnimationSet> & getAnimationSet() const;

        void setSpriteTexture(const std::shared_ptr<sf::Texture>& newTexture);

        void setCurrentAnimation(const std::string & animationName);

        void setTrackedObject(const std::weak_ptr<Entity> & trackedObject);
        const std::weak_ptr<Entity> & getTrackedObject() const;

        void setTrackX(bool track);
        void setTrackY(bool track);
        bool getTrackX() const;
        bool getTrackY() const;
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_PARTICLE
