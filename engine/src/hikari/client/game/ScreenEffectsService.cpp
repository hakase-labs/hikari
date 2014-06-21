#include "hikari/client/game/ScreenEffectsService.hpp"
#include "hikari/client/game/EventBusService.hpp"
#include "hikari/core/util/Log.hpp"

#include <SFML/Graphics.hpp>

namespace hikari {

    ScreenEffectsService::ScreenEffectsService(const std::weak_ptr<EventBusService> & eventBus, int bufferWidth, int bufferHeight)
        : Service()
        , eventBus(eventBus)
        , backBuffer()
        , inputSprite()
        , effects()
    {
        backBuffer.create(bufferWidth, bufferHeight);

        auto effect = std::make_shared<FadingScreenEffect>();
        effects.push_back(effect);
    }

    ScreenEffectsService::~ScreenEffectsService() {

    }

    void ScreenEffectsService::setInputTexture(const sf::RenderTexture & texture) {
        inputSprite.setTexture(texture.getTexture());
    }

    void ScreenEffectsService::update(float dt) {
        std::for_each(
            std::begin(effects),
            std::end(effects),
            [&](std::shared_ptr<ScreenEffect> & effect) {
                effect->update(dt);
            }
        );
    }

    void ScreenEffectsService::render(sf::RenderTarget & target) {
        backBuffer.clear(sf::Color::Black);

        std::for_each(
            std::begin(effects),
            std::end(effects),
            [&](std::shared_ptr<ScreenEffect> & effect) {
                // Render the effect to the buffer, and then swap the buffer
                // pointers.
                effect->inputSprite = &inputSprite;
                effect->render(backBuffer);
            }
        );

        //backBuffer.draw(inputSprite /* , effectShader */);
        backBuffer.display();

        sf::Sprite renderSprite(backBuffer.getTexture());
        target.draw(renderSprite);
    }

} // hikari
