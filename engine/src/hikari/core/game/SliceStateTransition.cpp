#include "hikari/core/game/SliceStateTransition.hpp"
#include "hikari/core/game/GameState.hpp"
#include "hikari/core/util/Log.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace hikari {

    std::unique_ptr<sf::RenderTexture> SliceStateTransition::exitingStateTexture(nullptr);
    std::unique_ptr<sf::RenderTexture> SliceStateTransition::enteringStateTexture(nullptr);

    void SliceStateTransition::createSharedTextures() {
        exitingStateTexture.reset(new sf::RenderTexture());
        exitingStateTexture->create(256, 240);
        enteringStateTexture.reset(new sf::RenderTexture());
        enteringStateTexture->create(256, 240);
    }

    void SliceStateTransition::destroySharedTextures() {
        exitingStateTexture.reset();
        enteringStateTexture.reset();
    }

    SliceStateTransition::SliceStateTransition(SliceDirection direction, float duration)
        : StateTransition()
        , direction(direction)
        , duration(duration)
        , accumulator(0.0f)
        , exitingStateSpriteLayerTop()
        , exitingStateSpriteLayerMiddle()
        , exitingStateSpriteLayerBottom()
        , enteringStateSpriteLayer()
    {
        setComplete(false);

        exitingStateSpriteLayerTop.setTexture(exitingStateTexture->getTexture());
        exitingStateSpriteLayerTop.setTextureRect(sf::IntRect(0, 0, 256, 240 / 3));
        exitingStateSpriteLayerMiddle.setTexture(exitingStateTexture->getTexture());
        exitingStateSpriteLayerMiddle.setTextureRect(sf::IntRect(0, 240 / 3, 256, 240 / 3));
        exitingStateSpriteLayerBottom.setTexture(exitingStateTexture->getTexture());
        exitingStateSpriteLayerBottom.setTextureRect(sf::IntRect(0, 240 / 3 * 2, 256, 240 / 3));
        enteringStateSpriteLayer.setTexture(enteringStateTexture->getTexture());
    }

    SliceStateTransition::~SliceStateTransition() {

    }

    void SliceStateTransition::render(sf::RenderTarget &target) {
        //const sf::View & oldView = target.getView();

        //sf::View overlayView(sf::FloatRect(0.0f, 0.0f, 256.0f, 240.0f));

        //target.setView(overlayView);
        // target.draw(overlay);
        //if(!isComplete()) {
            if(direction == SLICE_LEFT) {
                if(enteringState) {
                    enteringStateTexture->clear(sf::Color::Transparent);
                    enteringState->render(*enteringStateTexture);
                    enteringStateTexture->display();
                    target.draw(enteringStateSpriteLayer);
                }
                if(exitingState) {
                    exitingStateTexture->clear(sf::Color::Transparent);
                    exitingState->render(*exitingStateTexture);
                    exitingStateTexture->display();
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
        //}

        //target.setView(oldView);
    }

    void SliceStateTransition::update(float dt) {
        accumulator += dt;

        float percentComplete = accumulator / duration;

        exitingStateSpriteLayerTop.setPosition(
            sf::Vector2f(
                1 * (percentComplete * 256),
                0
            )
        );

        exitingStateSpriteLayerMiddle.setPosition(
            sf::Vector2f(
                -1 * (percentComplete * 256),
                240 / 3
            )
        );

        exitingStateSpriteLayerBottom.setPosition(
            sf::Vector2f(
                1 * (percentComplete * 256),
                240 / 3 * 2
            )
        );

        if(accumulator >= duration) {
            setComplete(true);
        }
    }

} // hikari
