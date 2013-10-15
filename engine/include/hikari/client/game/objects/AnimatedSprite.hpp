#ifndef HIKARI_CLIENT_GAME_OBJECTS_ANIMATEDSPRITE
#define HIKARI_CLIENT_GAME_OBJECTS_ANIMATEDSPRITE

#include "hikari/core/game/SpriteAnimator.hpp"
#include "hikari/core/game/Updatable.hpp"
#include "hikari/core/math/Vector2.hpp"

#include <SFML/Graphics/Sprite.hpp>

#include <string>
#include <memory>

namespace sf {
    class RenderTarget;
}

namespace hikari {
    class Animation;
    class AnimationSet;

    class AnimatedSprite {
    protected:
        sf::Sprite sprite;

    private:
        std::string currentAnimation;
        std::weak_ptr<AnimationSet> animationSet;
        SpriteAnimator animator;
        bool isXAxisFlipped;
        bool isYAxisFlipped;

    public:
        AnimatedSprite();
        AnimatedSprite(const AnimatedSprite & proto);

        //
        // Inherited from hikari::Updatable
        //
        virtual void update(float dt);

        virtual void render(sf::RenderTarget &target) const;

        void setAnimation(const std::string & animationName);
        void setAnimationSet(const std::weak_ptr<AnimationSet> & animationSetPtr);

        const std::string & getAnimation() const;
        const std::weak_ptr<AnimationSet> getAnimationSet() const;

        bool isXFlipped() const;
        void setXFlipped(bool isFlipped);

        bool isYFlipped() const;
        void setYFlipped(bool isFlipped);

        Vector2<float> getPosition() const;
        void setPosition(const Vector2<float> & position);

        /**
         * Starts an animation playback from the beginning.
         */
        void rewind();

        /**
         * Pauses any animation playback.
         *
         * @see unpause()
         */
        void pause();

        /**
         * Unpauses any animation playback.
         *
         * @see pause()
         */
        void unpause();

        /**
         * Determines whether the animation playback is paused.
         *
         * @see pause()
         * @see unapuse()
         * 
         * @return true if playback is paused, false otherwise
         */
        bool isPaused() const;
    };
} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_ANIMATEDSPRITE