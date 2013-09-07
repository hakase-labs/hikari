#include "hikari/core/game/StateTransition.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace hikari {

    class SliceStateTransition : public StateTransition {
    public:
        enum SliceDirection {
            SLICE_LEFT = 0,
            SLICE_RIGHT = 1
        };

    private:
        SliceDirection direction;
        const float duration;
        float accumulator;
        sf::RectangleShape overlay;
        sf::RenderTexture exitingStateTexture;
        sf::RenderTexture enteringStateTexture;
        sf::Sprite exitingStateSpriteLayerTop;
        sf::Sprite exitingStateSpriteLayerMiddle;
        sf::Sprite exitingStateSpriteLayerBottom;

        sf::Sprite enteringStateSpriteLayer;

    public:
        SliceStateTransition(SliceDirection direction, float duration);
        virtual ~SliceStateTransition();

        virtual void render(sf::RenderTarget &target);
        virtual void update(float dt);
    };

} // hikari
