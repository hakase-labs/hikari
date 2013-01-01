#ifndef HIKARI_CORE_GAME_TILEANIMATOR
#define HIKARI_CORE_GAME_TILEANIMATOR

#include <hikari/core/Platform.hpp>
#include <hikari/core/game/Animator.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <vector>

namespace hikari {

    class HIKARI_API TileAnimator : public Animator {
    private:
        int tileIndex;
        std::vector<sf::IntRect> &tiles;

    public:
        TileAnimator(std::vector<sf::IntRect> &tiles, const unsigned int& tileIndex);
        const int& getUpdatedTileIndex() const;
        virtual ~TileAnimator() { }
        virtual void update(float delta);
        void update(float delta, sf::IntRect &tileRect);
    };

} // hikari

#endif // HIKARI_CORE_GAME_TILEANIMATOR