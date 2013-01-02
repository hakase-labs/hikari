/*
#ifndef HIKARI_CLIENT_GAME_OBJECTS_ROCKMANHERO
#define HIKARI_CLIENT_GAME_OBJECTS_ROCKMANHERO

#include <hikari/client/game/objects/Hero.hpp>
#include <hikari/client/game/Input.hpp>
#include <hikari/core/game/AnimationSet.hpp>
#include <hikari/core/game/SpriteAnimator.hpp>

#include <memory>

namespace hikari {

    class RockmanHero : public Hero {
    private:
        // Flags
        bool isShooting;
        bool isSliding;

        // Timers
        float shootingTimer;
        float slidingTimer;

        std::shared_ptr<AnimationSet> animations;
        SpriteAnimator animator;

        std::shared_ptr<Input> input;
    public:
        RockmanHero(const int& id, std::shared_ptr<AnimationSet> animations, std::shared_ptr<Input> input);
        void setAnimationSet(std::shared_ptr<AnimationSet> animations);
        virtual ~RockmanHero();
        virtual void render(sf::RenderTarget &target);
        virtual void update(const float &dt);
    };

}

#endif // HIKARI_CLIENT_GAME_OBJECTS_HERO
*/