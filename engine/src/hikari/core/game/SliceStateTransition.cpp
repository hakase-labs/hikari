#include "hikari/core/game/SliceStateTransition.hpp"
#include "hikari/core/game/GameState.hpp"
#include "hikari/core/util/Log.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace hikari {

    SliceStateTransition::SliceStateTransition(SliceDirection direction, float duration)
        : StateTransition()
        , direction(direction)
        , duration(duration)
        , accumulator(0.0f)
        , overlay(sf::Vector2f(256.0f, 240.0f))
        , exitingStateTexture()
        , enteringStateTexture()
        , exitingStateSpriteLayerTop()
        , exitingStateSpriteLayerMiddle()
        , exitingStateSpriteLayerBottom()
        , enteringStateSpriteLayer()
    {
        setComplete(false);

        exitingStateTexture.create(256, 240);
        enteringStateTexture.create(256, 240);
        exitingStateSpriteLayerTop.setTexture(exitingStateTexture.getTexture());
        exitingStateSpriteLayerTop.setTextureRect(sf::IntRect(0, 0, 256, 240 / 3));
        exitingStateSpriteLayerMiddle.setTexture(exitingStateTexture.getTexture());
        exitingStateSpriteLayerMiddle.setTextureRect(sf::IntRect(0, 240 / 3, 256, 240 / 3));
        exitingStateSpriteLayerBottom.setTexture(exitingStateTexture.getTexture());
        exitingStateSpriteLayerBottom.setTextureRect(sf::IntRect(0, 240 / 3 * 2, 256, 240 / 3));
        // exitingStateSpriteLayer.setColor(sf::Color::Red);
        enteringStateSpriteLayer.setTexture(enteringStateTexture.getTexture());
        enteringStateSpriteLayer.setColor(sf::Color::Green);

        // overlay.setFillColor(color);
    }

    SliceStateTransition::~SliceStateTransition() {

    }

    void SliceStateTransition::render(sf::RenderTarget &target) {
        //const sf::View & oldView = target.getView();

        //sf::View overlayView(sf::FloatRect(0.0f, 0.0f, 256.0f, 240.0f));

        //target.setView(overlayView);
        // target.draw(overlay);
        if(direction == SLICE_LEFT) {
            if(enteringState) {
                enteringStateTexture.clear(sf::Color::Green);
                enteringState->render(enteringStateTexture);
                enteringStateTexture.display();
                target.draw(enteringStateSpriteLayer);
            }
            if(exitingState) {
                exitingStateTexture.clear(sf::Color::Blue);
                exitingState->render(exitingStateTexture);
                exitingStateTexture.display();
                target.draw(exitingStateSpriteLayerTop);
                target.draw(exitingStateSpriteLayerMiddle);
                target.draw(exitingStateSpriteLayerBottom);
            }
        } else {
            // if(enteringState) {
            //     enteringStateTexture.clear(sf::Color::Green);
            //     enteringState->render(enteringStateTexture);
            //     enteringStateTexture.display();
            //     target.draw(enteringStateSpriteLayer);
            // }
        }

        //target.setView(oldView);
    }

    void SliceStateTransition::update(float dt) {
        accumulator += dt;

        float percentComplete = accumulator / duration;

        exitingStateSpriteLayerTop.setPosition(
            sf::Vector2f(
                -1 * (percentComplete * 256),
                0
            )
        );

        exitingStateSpriteLayerMiddle.setPosition(
            sf::Vector2f(
                1 * (percentComplete * 256),
                240 / 3
            )
        );

        exitingStateSpriteLayerBottom.setPosition(
            sf::Vector2f(
                -1 * (percentComplete * 256),
                240 / 3 * 2
            )
        );

        // sf::Color color = overlay.getFillColor();

        // if(direction == FADE_OUT) {
        //     HIKARI_LOG(debug4) << "Fade out!!";
        //     color.a = std::min(255, static_cast<int>((accumulator / duration) * 255.0));
        // } else {
        //     HIKARI_LOG(debug4) << "Fade in!!";
        //     color.a = std::max(0, 255 - static_cast<int>((accumulator / duration) * 255.0));
        // }
        
        // overlay.setFillColor(color);

        if(accumulator >= duration) {
            setComplete(true);
        }
    }

} // hikari
