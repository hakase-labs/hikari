#ifndef HIKARI_CLIENT_SCREENEFFECTSSERVICE
#define HIKARI_CLIENT_SCREENEFFECTSSERVICE

#include "hikari/core/util/Service.hpp"
#include "hikari/core/util/NonCopyable.hpp"

#include <memory>
#include <vector>

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "hikari/core/util/FileSystem.hpp"

namespace sf {
    class RenderTarget;
    class Texture;
}

namespace hikari {

    class EventBusService;

    struct ScreenEffect {
        sf::Sprite * inputSprite;

        virtual void update(float dt) {

        }

        virtual void render(sf::RenderTarget & target) {

        }
    };

    class ScreenEffectsService : public Service, public NonCopyable {
    private:
        std::weak_ptr<EventBusService> eventBus;
        sf::RenderTexture backBuffer;
        sf::Sprite inputSprite;
        std::vector<std::shared_ptr<ScreenEffect>> effects;

    public:
        static std::unique_ptr<sf::Shader> FADE_OUT_SHADER;
        static std::unique_ptr<sf::Shader> FADE_IN_SHADER;
        static void preloadShaders();
        static void destroyShaders();

        explicit ScreenEffectsService(const std::weak_ptr<EventBusService> & eventBus, int bufferWidth, int bufferHeight);
        virtual ~ScreenEffectsService();

        void setInputTexture(const sf::RenderTexture & texture);

        void update(float dt);
        void render(sf::RenderTarget & target);

        void fadeOut(float fadeDuration = 0.2167f);
        void fadeIn(float fadeDuration = 0.2167f);

        void clearEffects();
    };

    struct FadeOutShaderScreenEffect : public ScreenEffect {
        float timer;
        float fadeDuration;
        sf::Shader * pixelShader; // non-owning

        FadeOutShaderScreenEffect(float fadeDuration = 1.0f)
            : timer(0)
            , fadeDuration(fadeDuration)
        {
            pixelShader = ScreenEffectsService::FADE_IN_SHADER.get();

            if(pixelShader) {
                pixelShader->setParameter("texture", sf::Shader::CurrentTexture);
                pixelShader->setParameter("fadePercent", (timer / fadeDuration) * 100.0f);
            }
        }

        virtual void update(float dt) {
            timer += dt;
        }

        virtual void render(sf::RenderTarget & target) {
            pixelShader->setParameter("fadePercent", (timer / fadeDuration) * 100.0f);
            target.draw(*inputSprite, pixelShader);
        }
    };

    struct FadeInShaderScreenEffect : public ScreenEffect {
        float timer;
        float fadeDuration;
        sf::Shader * pixelShader; // non-owning

        FadeInShaderScreenEffect(float fadeDuration = 1.0f)
            : timer(fadeDuration)
            , fadeDuration(fadeDuration)
        {
            pixelShader = ScreenEffectsService::FADE_IN_SHADER.get();

            if(pixelShader) {
                pixelShader->setParameter("texture", sf::Shader::CurrentTexture);
                pixelShader->setParameter("fadePercent", (timer / fadeDuration) * 100.0f);
            }
        }

        virtual void update(float dt) {
            timer -= dt;

            if(timer < 0) {
                timer = 0;
            }
        }

        virtual void render(sf::RenderTarget & target) {
            pixelShader->setParameter("fadePercent", (timer / fadeDuration) * 100.0f);
            target.draw(*inputSprite, pixelShader);
        }
    };

} // hikari

#endif // HIKARI_CLIENT_SCREENEFFECTSSERVICE
