#include "hikari/core/game/TileAnimator.hpp"
#include "hikari/core/game/Animation.hpp"

namespace hikari {

    TileAnimator::TileAnimator(std::vector<sf::IntRect> &tiles, unsigned int tileIndex)
        : Animator()
        , tileIndex(tileIndex)
        , tiles(tiles) {

    }

    TileAnimator & TileAnimator::operator=(const TileAnimator & other) {
        // Call base-class op first!
        Animator::operator = (other);

        tileIndex = other.tileIndex;
        tiles = other.tiles;

        return *this;
    }

    const int& TileAnimator::getUpdatedTileIndex() const {
        return tileIndex;
    }

    void TileAnimator::update(float delta) {
        Animator::update(delta);
    }

    void TileAnimator::update(float delta, sf::IntRect &tileRect) {
        update(delta);

        const AnimationFrame &currentFrame = getAnimation()->getFrameAt(getCurrentFrameIndex());
        const Rectangle2D<int> &currentFrameRectangle = currentFrame.getSourceRectangle();

        tileRect.top = currentFrameRectangle.getTop();
        tileRect.width = currentFrameRectangle.getWidth();
        tileRect.height = currentFrameRectangle.getHeight();
        tileRect.left = currentFrameRectangle.getLeft();
    }

} // hikari