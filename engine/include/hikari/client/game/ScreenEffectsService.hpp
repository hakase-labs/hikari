#ifndef HIKARI_CLIENT_SCREENEFFECTSSERVICE
#define HIKARI_CLIENT_SCREENEFFECTSSERVICE

#include "hikari/core/util/Service.hpp"
#include "hikari/core/util/NonCopyable.hpp"

#include <memory>
#include <vector>

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace sf {
    class RenderTarget;
    class Texture;
}

namespace hikari {

    class EventBusService;

    struct ScreenEffect {
        sf::Texture * inputTexture;

        virtual void update(float dt) {

        }

        virtual void render(sf::RenderTarget & target) {

        }
    };

    struct FadingScreenEffect : public ScreenEffect {
        float timer = 0;
        sf::RectangleShape mask;

        FadingScreenEffect() {
            mask.setSize(sf::Vector2f(256, 240));
            mask.setFillColor(sf::Color::Black);
        }

        virtual void update(float dt) {
            timer += dt;

            sf::Color fillColor = mask.getFillColor();
            fillColor.a = std::min(255, static_cast<int>((timer / 1.0) * 255.0f));

            mask.setFillColor(fillColor);

            if(timer >= 1.0) {
                timer = 0;
            }
        }

        virtual void render(sf::RenderTarget & target) {
            sf::Sprite sprite(*inputTexture);

            target.draw(sprite);
            target.draw(mask);
        }
    };

    class ScreenEffectsService : public Service, public NonCopyable {
    private:
        std::weak_ptr<EventBusService> eventBus;
        sf::RenderTexture backBuffer;
        sf::Sprite inputSprite;
        std::vector<std::shared_ptr<ScreenEffect>> effects;

    public:
        explicit ScreenEffectsService(const std::weak_ptr<EventBusService> & eventBus, int bufferWidth, int bufferHeight);
        virtual ~ScreenEffectsService();

        void setInputTexture(const sf::RenderTexture & texture);

        void update(float dt);
        void render(sf::RenderTarget & target);
    };

} // hikari

#endif // HIKARI_CLIENT_SCREENEFFECTSSERVICE
