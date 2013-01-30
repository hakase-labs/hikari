#include "hikari/core/game/map/Tileset.hpp"
#include <algorithm>

namespace hikari {

    Tileset::Tileset(
        const std::shared_ptr<sf::Texture> &texture, 
        const size_t& tileSize, 
        const std::vector<sf::IntRect> &tiles,
        const std::vector<TileAnimator> &tileAnimators
    )
        : tileSize(tileSize)
        , tiles(tiles)
        , tileAnimators(tileAnimators)
        , texture(texture) {

    }

    const size_t& Tileset::getTileSize() const {
        return tileSize; 
    }

    const sf::IntRect& Tileset::getTileRect(const unsigned int &index) const {
        if(index >=0 && index < tiles.size()) {
            return tiles.at(index);
        } else {
            return tiles.at(0);
        }
    }

    const std::shared_ptr<sf::Texture> Tileset::getTexture() const {
        return texture;
    }

    void Tileset::update(float delta) {
        if(tileAnimators.size() > 0) {
            std::for_each(tileAnimators.begin(), tileAnimators.end(), [&](TileAnimator &animator){
                animator.update(delta, tiles.at(animator.getUpdatedTileIndex()));
            });
        }
    }

} // hikari