#ifndef HIKARI_CORE_GUI_IMAGEFONT
#define HIKARI_CORE_GUI_IMAGEFONT

#include <hikari/core/Platform.hpp>
#include <hikari/core/util/Service.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <memory>
#include <map>
#include <string>

namespace sf {
    class Texture;
    class RenderTarget;
} 

namespace hikari {

    class HIKARI_API ImageFont : public Service {
    private:
        int glyphWidth;
        int glyphHeight;
        std::shared_ptr<sf::Texture> glyphTexture;
        sf::Sprite glyphSprite;
        std::string glyphs;
        std::map< char, sf::IntRect > glyphMap;
    public:
        ImageFont(const std::shared_ptr<sf::Texture> &glyphTexture, const std::string &glyphs, const int &glyphWidth, const int &glyphHeight);
        virtual ~ImageFont();
        
        const int& getGlyphWidth() const;
        const int& getGlyphHeight() const;

        /**
            Renders a string to an sf::RenderTarget at a specified location
            optionally with a color. The color is applied as a filter to the
            source image.

            The newline character (\n) is supported and will reset the cursor
            to the position specified by the parameter x. The next line begins
            at the number of pixels specified by the glyph's height when the
            font was constructed.

            @param target the target to render to
            @param glyphs the text to be rendered
            @param x the x-coordinate where the text should be rendered
            @param y the y-coordinate where the text should be rendered
            @param color a color filter to apply to the text, transparent by default
        */
        void renderText(sf::RenderTarget &target, const std::string &glyphs, const int &x, const int &y, const sf::Color &color = sf::Color::White);
    };

} // hikari

#endif // HIKARI_CORE_GUI_IMAGEFONT