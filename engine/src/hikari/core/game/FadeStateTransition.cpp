#include "hikari/core/game/FadeStateTransition.hpp"
#include "hikari/core/game/GameState.hpp"
#include "hikari/core/util/Log.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace hikari {

    FadeStateTransition::FadeStateTransition(FadeDirection direction, sf::Color color, float duration)
        : StateTransition()
        , direction(direction)
        , overlay(sf::Vector2f(256.0f, 240.0f))
        , duration(duration)
        , accumulator(0.0f)
    {
        setComplete(false);

        color.a = direction == FADE_OUT ? 0 : 255;
        overlay.setFillColor(color);
    }

    FadeStateTransition::~FadeStateTransition() {

    }

    void FadeStateTransition::render(sf::RenderTarget &target) {
        // if(exitingState && direction == FADE_OUT) {
        //     exitingState->render(target);
        // }

        // if(exitingState && direction == FADE_OUT) {
        //     exitingState->render(target);
        // }
        if(direction == FADE_OUT) {
            if(exitingState) {
                exitingState->render(target);
            }
        } else {
            if(enteringState) {
                enteringState->render(target);
            }
        }

        const sf::View & oldView = target.getView();

        sf::View overlayView(sf::FloatRect(0.0f, 0.0f, 256.0f, 240.0f));

        target.setView(overlayView);
        target.draw(overlay);
        target.setView(oldView);
    }

    void FadeStateTransition::update(float dt) {
        accumulator += dt;

        sf::Color color = overlay.getFillColor();

        if(direction == FADE_OUT) {
            HIKARI_LOG(debug4) << "Fade out!!";
            color.a = std::min(255, static_cast<int>((accumulator / duration) * 255.0));
        } else {
            HIKARI_LOG(debug4) << "Fade in!!";
            color.a = std::max(0, 255 - static_cast<int>((accumulator / duration) * 255.0));
        }
        
        overlay.setFillColor(color);

        if(accumulator >= duration) {
            setComplete(true);
        }
    }

} // hikari
