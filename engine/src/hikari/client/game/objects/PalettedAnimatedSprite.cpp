#include "hikari/client/game/objects/PalettedAnimatedSprite.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <iostream>

namespace hikari {

    std::unique_ptr<sf::Shader> PalettedAnimatedSprite::pixelShader(nullptr);

    int PalettedAnimatedSprite::sharedPaletteIndex = 0;

    const int PalettedAnimatedSprite::USE_NO_PALETTE_INDEX = -1;
    const int PalettedAnimatedSprite::USE_SHARED_PALETTE_INDEX = -2;

    void PalettedAnimatedSprite::setShaderFile(const std::string & file) {

    }

    PalettedAnimatedSprite::PalettedAnimatedSprite()
        : AnimatedSprite()
        , paletteIndex(USE_NO_PALETTE_INDEX)
    {

    }

    PalettedAnimatedSprite::PalettedAnimatedSprite(const PalettedAnimatedSprite & proto)
        : AnimatedSprite(proto)
        , paletteIndex(proto.paletteIndex)
    {

    }

    PalettedAnimatedSprite::~PalettedAnimatedSprite() {

    }

    void PalettedAnimatedSprite::update(float dt) {
        AnimatedSprite::update(dt);
    }

    void PalettedAnimatedSprite::render(sf::RenderTarget &target) const {
        if(paletteIndex == USE_NO_PALETTE_INDEX) {
            AnimatedSprite::render(target);
        } else {
            if(pixelShader) {
                pixelShader->setParameter("texture", *(sprite.getTexture()));
                pixelShader->setParameter("paletteIndex", paletteIndex == USE_SHARED_PALETTE_INDEX ? sharedPaletteIndex : paletteIndex);
                target.draw(sprite, pixelShader.get());
            }
        }
    }

    int PalettedAnimatedSprite::getPaletteIndex() const {
        return paletteIndex;
    }

    void PalettedAnimatedSprite::setPaletteIndex(int index) {
        paletteIndex = index;
    }

} // hikari
