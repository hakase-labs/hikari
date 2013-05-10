#include "hikari/core/gui/ImageFont.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace hikari {

    ImageFont::ImageFont(const std::shared_ptr<sf::Texture> &glyphTexture, const std::string &glyphs, 
        const int &glyphWidth, const int &glyphHeight)
        : glyphWidth(glyphWidth)
        , glyphHeight(glyphHeight)
        , glyphTexture(glyphTexture)
        , glyphSprite(*glyphTexture)
        , glyphs(glyphs)
    {
            // Loop through the glyphs
            // For each letter, create a rect that represents that letter in the image
            int i = 0;
            for(std::string::const_iterator itr = glyphs.begin(), end = glyphs.end(); 
                itr < end; 
                itr++, i++) {
                    char glyph = (*itr);
                    glyphMap[glyph] = sf::IntRect(i * glyphWidth, 0, glyphWidth, glyphHeight);
            }
    }

    ImageFont::~ImageFont() { }

    const int& ImageFont::getGlyphWidth() const {
        return glyphWidth;
    }

    const int& ImageFont::getGlyphHeight() const {
        return glyphHeight;
    }

    void ImageFont::renderText(sf::RenderTarget &target, const std::string &text, const int &x, const int &y, const sf::Color &color) {
        int i = 0;
        int dx = 0;
        int dy = 0;

        for(std::string::const_iterator itr = text.begin(), end = text.end(); 
            itr < end; 
            itr++, i++) {
                char glyph = (*itr);

                // Adjust for new lines.
                if(glyph == '\n') {
                    dy += glyphHeight; // TODO: Add kerning? Line-height adjustment?
                    dx = 0;
                    continue;
                }

                glyphSprite.setTextureRect(glyphMap[glyph]);
                glyphSprite.setPosition(static_cast<float>(x + dx), static_cast<float>(y + dy));
                glyphSprite.setColor(color);

                target.draw(glyphSprite);

                // Add horizontal offset here so the first character will be 
                // drawn at x.
                dx += glyphWidth;
        }
    }

} // hikari