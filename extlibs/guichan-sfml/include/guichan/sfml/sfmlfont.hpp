#ifndef GCN_SFMLFONT_HPP
#define GCN_SFMLFONT_HPP

#include "guichan/font.hpp"
#include "guichan/platform.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

namespace gcn
{
    /**
     * A font which supports rendering an sf:Font object.
     */
    class GCN_EXTENSION_DECLSPEC SFMLFont : public Font
    {
    public:
        /**
         * Constructor.
         */
        SFMLFont(const std::string& filename, unsigned int size);

        /**
         * Destructor.
         */
        virtual ~SFMLFont(){}

        const sf::Color& getColor() const;

        void setColor(const sf::Color& color);

        const sf::Font& getFont() const;

        // Inherited from Font

        virtual void drawString(Graphics* graphics, const std::string& text, int x, int y);

        virtual int getWidth(const std::string& text) const;

        virtual int getHeight() const;

        virtual int getStringIndexAt(const std::string& text, int x) const;

    protected:
        sf::Color mColor;
        sf::Font mFont;
        sf::Text mText;
    };
}

#endif // end GCN_SFMLFONT_HPP
