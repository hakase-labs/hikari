#include "hikari/client/game/FadeColorTask.hpp"
#include "hikari/core/util/Log.hpp"

namespace hikari {

    FadeColorTask::FadeColorTask(FadeDirection direction, sf::RectangleShape & rectangle,
            float duration)
        : BaseTask(0, Task::TYPE_BLOCKING)
        , direction(direction)
        , rectangle(rectangle)
        , duration(duration)
        , accumulator(0.0f)
    {

    }

    FadeColorTask::~FadeColorTask() {

    }

    void FadeColorTask::update(float dt) {
        accumulator += dt;

        sf::Color color = rectangle.getFillColor();

        if(direction == FADE_OUT) {
            HIKARI_LOG(debug4) << "Fade out!!";
            color.a = std::min(255, static_cast<int>((accumulator / duration) * 255.0f));
        } else {
            HIKARI_LOG(debug4) << "Fade in!!";
            color.a = std::max(0, 255 - static_cast<int>((accumulator / duration) * 255.0f));
        }

        rectangle.setFillColor(color);

        if(accumulator >= duration) {
            markAsCompleted();
        }
    }

} // hikari
