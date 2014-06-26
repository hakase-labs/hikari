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

    struct FadeOutShaderScreenEffect : public ScreenEffect {
        float timer = 0;
        float fadeDuration = 1.0f;
        std::unique_ptr<sf::Shader> pixelShader;

        FadeOutShaderScreenEffect(float fadeDuration = 1.0f)
            : fadeDuration(fadeDuration)
        {
            const std::string shaderCode = FileSystem::readFileAsString("assets/shaders/fade.frag");
            pixelShader.reset(new sf::Shader());
            pixelShader->loadFromMemory(shaderCode, sf::Shader::Fragment);
            pixelShader->setParameter("texture", sf::Shader::CurrentTexture);
        }

        virtual void update(float dt) {
            timer += dt;

            pixelShader->setParameter("fadePercent", (timer / fadeDuration) * 100.0f);
        }

        virtual void render(sf::RenderTarget & target) {
            target.draw(*inputSprite, pixelShader.get());
        }
    };

    struct FadeInShaderScreenEffect : public ScreenEffect {
        float timer = 0;
        float fadeDuration = 1.0f;
        std::unique_ptr<sf::Shader> pixelShader;

        FadeInShaderScreenEffect(float fadeDuration = 1.0f)
            : timer(fadeDuration)
            , fadeDuration(fadeDuration)
        {
            const std::string shaderCode = FileSystem::readFileAsString("assets/shaders/fade.frag");
            pixelShader.reset(new sf::Shader());
            pixelShader->loadFromMemory(shaderCode, sf::Shader::Fragment);
            pixelShader->setParameter("texture", sf::Shader::CurrentTexture);
            pixelShader->setParameter("fadePercent", (timer / fadeDuration) * 100.0f);
        }

        virtual void update(float dt) {
            timer -= dt;

            if(timer < 0) {
                timer = 0;
            }
        }

        virtual void render(sf::RenderTarget & target) {
            pixelShader->setParameter("fadePercent", (timer / fadeDuration) * 100.0f);
            target.draw(*inputSprite, pixelShader.get());
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
