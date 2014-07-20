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

        // We have to check the size here otherwise if there are no effects the
        // input sprite won't be drawn to the buffer at all! So if there aren't
        // any effects to process we just pass through.
        if(effects.size() > 0) {
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
        } else {
            backBuffer.draw(inputSprite);
        }

        backBuffer.display();

        sf::Sprite renderSprite(backBuffer.getTexture());
        target.draw(renderSprite);
    }

    void ScreenEffectsService::fadeOut(float fadeDuration) {
        HIKARI_LOG(debug) << "ScreenEffectsService::fadeOut";

        clearEffects();
        effects.push_back(std::make_shared<FadeOutShaderScreenEffect>(fadeDuration));
    }

    void ScreenEffectsService::fadeIn(float fadeDuration) {
        HIKARI_LOG(debug) << "ScreenEffectsService::fadeIn";

        clearEffects();
        effects.push_back(std::make_shared<FadeInShaderScreenEffect>(fadeDuration));
    }

    void ScreenEffectsService::clearEffects() {
        effects.clear();
    }

} // hikari
