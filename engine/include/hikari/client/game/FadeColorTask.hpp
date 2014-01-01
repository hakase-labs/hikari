#ifndef HIKARI_CLIENT_GAME_FADE_COLOR_TASK
#define HIKARI_CLIENT_GAME_FADE_COLOR_TASK

#include "hikari/client/game/BaseTask.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace hikari {

    /**
     * A Task which fades the color of an sf::RectangleShape in or out. Fades to
     * or from transparent. Takes a duration after which the fade is complete and
     * the task is marked as complete.
     */
    class FadeColorTask : public BaseTask {
    public:
        enum FadeDirection {
            FADE_OUT = 0,
            FADE_IN = 1
        };

    private:
        FadeDirection direction;
        sf::RectangleShape & rectangle;
        const float duration;
        float accumulator;

    public:
        FadeColorTask(FadeDirection direction, sf::RectangleShape & rectangle,
            float duration);

        virtual ~FadeColorTask();

        virtual void update(float dt);
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_FADE_COLOR_TASK