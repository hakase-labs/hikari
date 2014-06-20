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
    {
        backBuffer.create(bufferWidth, bufferHeight);
    }

    ScreenEffectsService::~ScreenEffectsService() {

    }

    void ScreenEffectsService::setInputTexture(const sf::RenderTexture & texture) {
        inputSprite.setTexture(texture.getTexture());
    }

    void ScreenEffectsService::update(float dt) {

    }

    void ScreenEffectsService::render(sf::RenderTarget & target) {
        backBuffer.clear(sf::Color::Black);
        backBuffer.draw(inputSprite /* , effectShader */);
        backBuffer.display();

        sf::Sprite renderSprite(backBuffer.getTexture());
        target.draw(renderSprite);
    }

} // hikari
