#include "guichan/sfml/sfmlfont.hpp"
#include "guichan/sfml/sfmlgraphics.hpp"

#include <limits>
#include <string>

#include <SFML/Graphics/RenderTarget.hpp>

#include "guichan/exception.hpp"
#include "guichan/graphics.hpp"
#include "guichan/rectangle.hpp"

namespace gcn
{
    SFMLFont::SFMLFont(const std::string& filename, unsigned int size)
    {
        if (!mFont.loadFromFile(filename))
        {
            throw GCN_EXCEPTION("Unable to load font from file \"" + filename + "\"");
        }

        mColor = sf::Color::White;

        mText.setFont(mFont);
        mText.setCharacterSize(size);
        mText.setStyle(sf::Text::Regular);
    }

    const sf::Color& SFMLFont::getColor() const
    {
        return mColor;
    }

    void SFMLFont::setColor(const sf::Color& color)
    {
        mColor = color;
        mText.setColor(mColor);
    }

    const sf::Font& SFMLFont::getFont() const
    {
        return mFont;
    }

    int SFMLFont::getHeight() const
    {
        return mText.getCharacterSize();
    }

    int SFMLFont::getWidth(const std::string& text) const
    {
        sf::Text measureText(text, mFont, mText.getCharacterSize());
        sf::Vector2f renderedDimensions = measureText.findCharacterPos(text.size());

        return static_cast<int>(renderedDimensions.x);
    }

    void SFMLFont::drawString(Graphics* graphics, const std::string& text, int x, int y)
    {
        const SFMLGraphics* sfmlGraphics = dynamic_cast<SFMLGraphics*>(graphics);
        
        if (sfmlGraphics == NULL)
	    {
            throw GCN_EXCEPTION("Graphics is not of type SFMLGraphics");
	    }

        const gcn::ClipRectangle& clip = graphics->getCurrentClipArea();

        x += clip.xOffset;
        y += clip.yOffset;

        mText.setString(text);
        mText.setPosition(static_cast<float>(x), static_cast<float>(y));

        sf::RenderTarget& target = sfmlGraphics->getRenderTarget();

        target.draw(mText);
    }

    int SFMLFont::getStringIndexAt(const std::string& text, int x) const
    {
        if (x > (int)text.size() * 8)
        {
            return text.size();
        }

        return x / 8;
    }
}
