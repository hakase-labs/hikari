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

    int PalettedAnimatedSprite::sharedPaletteIndex = 0;

    void PalettedAnimatedSprite::setShaderFile(const std::string & file) {
        const std::string shaderCode = FileSystem::readFileAsString(file);
        pixelShader.reset(new sf::Shader());
        pixelShader->loadFromMemory(shaderCode, sf::Shader::Fragment);
        pixelShader->setParameter("texture", sf::Shader::CurrentTexture);

        auto colors = std::vector<std::vector<sf::Color>>();
        auto palette0 = std::vector<sf::Color>(8, sf::Color(0, 0, 0, 0));         // transparent
        auto palette1 = std::vector<sf::Color>(8, sf::Color(0, 0, 0, 255));       // black
        auto palette2 = std::vector<sf::Color>(8, sf::Color(255, 255, 255, 255)); // white
        
        auto palette3 = std::vector<sf::Color>();                                // rockman (normal)
        palette3.push_back(sf::Color(0,   0,   0,   255));  // black (outline)
        palette3.push_back(sf::Color(0,   0,   0,   255));  // black (eyes)
        palette3.push_back(sf::Color(255, 255, 255, 255));  // white
        palette3.push_back(sf::Color(255, 228, 164, 255));  // tan
        palette3.push_back(sf::Color(0,   112, 236, 255));  // dark blue
        palette3.push_back(sf::Color(56,  188, 255, 255));  // light blue

        auto palette4 = std::vector<sf::Color>();                                // rockman (quickman)
        palette4.push_back(sf::Color(0,   0,   0,   255));  // black (outline)
        palette4.push_back(sf::Color(0,   0,   0,   255));  // black (eyes)
        palette4.push_back(sf::Color(255, 255, 255, 255));  // white
        palette4.push_back(sf::Color(255, 228, 164, 255));  // tan
        palette4.push_back(sf::Color(255, 0,   255, 255));  // magenta
        palette4.push_back(sf::Color(255, 156, 255, 255));  // pink

        colors.push_back(palette0);
        colors.push_back(palette1);
        colors.push_back(palette2);
        colors.push_back(palette3);
        colors.push_back(palette4);

        setColorTable(colors);
    }

    void PalettedAnimatedSprite::setColorTable(const std::vector<std::vector<sf::Color>> & colors) {
        if(!colorTableImage) {
            colorTableImage.reset(new sf::Image());
        }

        colorTableImage->create(8, 16, sf::Color(0, 0, 255, 255));

        for(int row = 0; row < colors.size(); ++row) {
            const auto & paletteRow = colors[row];
            for(int column = 0; column < paletteRow.size(); ++column) {
                const auto color = paletteRow[column];

                colorTableImage->setPixel(column, row, color);
            }
        }

        if(!colorTableTexture) {
            colorTableTexture.reset(new sf::Texture());
        }

        colorTableTexture->create(8, 16);
        colorTableTexture->update(*colorTableImage);
        pixelShader->setParameter("colorTableTexture", *colorTableTexture);
        pixelShader->setParameter("colorTableWidth", static_cast<float>(colorTableImage->getSize().x));
        pixelShader->setParameter("colorTableHeight", static_cast<float>(colorTableImage->getSize().y));
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
