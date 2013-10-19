#include "hikari/client/game/objects/PalettedAnimatedSprite.hpp"

#include "hikari/core/util/FileSystem.hpp"

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <iostream>

namespace hikari {

    std::unique_ptr<sf::Shader> PalettedAnimatedSprite::pixelShader(nullptr);
    std::unique_ptr<sf::Image> PalettedAnimatedSprite::colorTableImage(nullptr);
    std::unique_ptr<sf::Texture> PalettedAnimatedSprite::colorTableTexture(nullptr);

    const unsigned int PalettedAnimatedSprite::colorTableWidth = 8;
    const unsigned int PalettedAnimatedSprite::colorTableHeight = 32;

    int PalettedAnimatedSprite::sharedPaletteIndex = 0;

    void PalettedAnimatedSprite::setShaderFile(const std::string & file) {
        const std::string shaderCode = FileSystem::readFileAsString(file);
        pixelShader.reset(new sf::Shader());
        pixelShader->loadFromMemory(shaderCode, sf::Shader::Fragment);
        pixelShader->setParameter("texture", sf::Shader::CurrentTexture);
    }

    void PalettedAnimatedSprite::createColorTable(const std::vector<std::vector<sf::Color>> & colors) {
        if(!colorTableImage) {
            colorTableImage.reset(new sf::Image());
        }

        colorTableImage->create(colorTableWidth, colorTableHeight, sf::Color(0, 0, 255, 255));

        for(unsigned int row = 0; row < colors.size(); ++row) {
            const auto & paletteRow = colors[row];
            for(unsigned int column = 0; column < paletteRow.size(); ++column) {
                const auto color = paletteRow[column];

                colorTableImage->setPixel(column, row, color);
            }
        }

        if(!colorTableTexture) {
            colorTableTexture.reset(new sf::Texture());
        }

        colorTableTexture->create(colorTableWidth, colorTableHeight);
        colorTableTexture->update(*colorTableImage);
        pixelShader->setParameter("colorTableTexture", *colorTableTexture);
        pixelShader->setParameter("colorTableWidth", static_cast<float>(colorTableWidth));
        pixelShader->setParameter("colorTableHeight", static_cast<float>(colorTableHeight));
    }

    void PalettedAnimatedSprite::destroySharedResources() {
        colorTableTexture.release();
        pixelShader.release();
    }

    void PalettedAnimatedSprite::setSharedPaletteIndex(int index) {
        sharedPaletteIndex = index;
    }

    int PalettedAnimatedSprite::getSharedPaletteIndex() {
        return sharedPaletteIndex;
    }

    PalettedAnimatedSprite::PalettedAnimatedSprite()
        : AnimatedSprite()
        , paletteIndex(0)
        , usePalette(false)
        , useSharedPalette(false)
    {

    }

    PalettedAnimatedSprite::PalettedAnimatedSprite(const PalettedAnimatedSprite & proto)
        : AnimatedSprite(proto)
        , paletteIndex(proto.paletteIndex)
        , usePalette(proto.usePalette)
        , useSharedPalette(proto.useSharedPalette)
    {

    }

    PalettedAnimatedSprite::~PalettedAnimatedSprite() {

    }

    void PalettedAnimatedSprite::update(float dt) {
        AnimatedSprite::update(dt);
    }

    void PalettedAnimatedSprite::render(sf::RenderTarget &target) const {
        if(isUsingPalette()) {
            if(pixelShader) {
                pixelShader->setParameter("paletteIndex",
                    static_cast<float>(isUsingSharedPalette() ? sharedPaletteIndex : paletteIndex));
                target.draw(sprite, pixelShader.get());
            }
        } else {
            AnimatedSprite::render(target);
        }
    }

    int PalettedAnimatedSprite::getPaletteIndex() const {
        return paletteIndex;
    }

    void PalettedAnimatedSprite::setPaletteIndex(int index) {
        paletteIndex = index;
    }

    bool PalettedAnimatedSprite::isUsingSharedPalette() const {
        return useSharedPalette;
    }

    void PalettedAnimatedSprite::setUseSharedPalette(bool flag) {
        useSharedPalette = flag;
    }

    bool PalettedAnimatedSprite::isUsingPalette() const {
        return usePalette;
    }

    void PalettedAnimatedSprite::setUsePalette(bool flag) {
        usePalette = flag;
    }

} // hikari
