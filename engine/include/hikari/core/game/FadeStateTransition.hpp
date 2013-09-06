#include "hikari/core/game/StateTransition.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace hikari {

    class FadeStateTransition : public StateTransition {
    public:
        enum FadeDirection {
            FADE_OUT = 0,
            FADE_IN = 1
        };

    private:
        FadeDirection direction;
        sf::RectangleShape overlay;
        const float duration;
        float accumulator;

    public:
        FadeStateTransition(FadeDirection direction, sf::Color color, float duration);
        virtual ~FadeStateTransition();

        virtual void render(sf::RenderTarget &target);
        virtual void update(float dt);
    };

} // hikari
