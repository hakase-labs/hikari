#include "guichan/sfml/sfmlgraphics.hpp"

#include "guichan/exception.hpp"
#include "guichan/font.hpp"
#include "guichan/image.hpp"
#include "guichan/sfml/sfmlimage.hpp"

#include <SFML/Graphics.hpp>

#include <cmath>

namespace gcn
{
    const float SFMLGraphics::PIXEL_ALIGNMENT_OFFSET = 0.375f;

    Color SFMLGraphics::convertSFMLColorToGuichanColor(const sf::Color& color)
    {
        return Color(color.r, color.g, color.b, color.a);
    }

    sf::Color SFMLGraphics::convertGuichanColorToSFMLColor(const Color& color)
    {
        return sf::Color(color.r, color.g, color.b, color.a);
    }

    SFMLGraphics::SFMLGraphics(sf::RenderTarget& target)
        : mTarget(&target)
    {
        mContextView = mTarget->getView();
        mSize = mContextView.getSize();

        setColor(Color());
    }

    void SFMLGraphics::_beginDraw()
    {
        // Save the view before drawing.
        mContextView = mTarget->getView();
        mSize = mContextView.getSize();

        pushClipArea(Rectangle(0, 0, static_cast<int>(mSize.x), static_cast<int>(mSize.y)));
    }

    void SFMLGraphics::_endDraw()
    {
        popClipArea();

        // Restore the view after drawing.
        mTarget->setView(mContextView);
    }

    void SFMLGraphics::setRenderTarget(sf::RenderTarget& target)
    {
        mTarget = &target;
        mContextView = mTarget->getView();
        mSize = mContextView.getSize();
    }

    bool SFMLGraphics::pushClipArea(Rectangle area)
    {
        bool result = Graphics::pushClipArea(area);

        if (result)
        {
            const sf::View clippingView = convertClipRectangleToView(mClipStack.top());
            mTarget->setView(clippingView);
        }

        return result;
    }

    void SFMLGraphics::popClipArea()
    {
        Graphics::popClipArea();

        if (mClipStack.empty())
        {
            return;
        }

        const sf::View clippingView = convertClipRectangleToView(mClipStack.top());
        mTarget->setView(clippingView);
    }

    sf::RenderTarget& SFMLGraphics::getRenderTarget() const {
        return *mTarget;
    }

    void SFMLGraphics::drawImage(const Image* image,
                                int srcX,
                                int srcY,
                                int dstX,
                                int dstY,
                                int width,
                                int height)
    {
        const SFMLImage* srcImage = dynamic_cast<const SFMLImage*>(image);

        if (srcImage == NULL)
        {
            throw GCN_EXCEPTION("Trying to draw an image of unknown format, must be an SFMLImage.");
        }

        if (mClipStack.empty())
        {
            throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw function outside of _beginDraw() and _endDraw()?");
        }

        const ClipRectangle& top = mClipStack.top();

        dstX += top.xOffset;
        dstY += top.yOffset;

        const sf::IntRect srcRect(srcX, srcY, width, height);

        mSprite.setTexture(*srcImage->getTexture(), false);
        mSprite.setTextureRect(srcRect);
        mSprite.setPosition(static_cast<float>(dstX), static_cast<float>(dstY));
        
        mTarget->draw(mSprite);
    }

    void SFMLGraphics::drawPoint(int x, int y)
    {
        if (mClipStack.empty())
        {
            throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw function outside of _beginDraw() and _endDraw()?");
        }

        const ClipRectangle& top = mClipStack.top();

        x += top.xOffset;
        y += top.yOffset;

        _drawFauxPixel(x, y);
    }

    void SFMLGraphics::drawLine(int x1, int y1, int x2, int y2)
    {
        if (y1 == y2)
        {
            drawHorizontalLine(x1, y1, x2);
            return;
        }
        else if (x1 == x2)
        {
            drawVerticalLine(x1, y1, y2);
            return;
        }
        else
        {
            drawBresenham(x1, y1, x2, y2);
            return;
        }
    }

    void SFMLGraphics::drawRectangle(const Rectangle& rectangle)
    {
        if (mClipStack.empty())
        {
            throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw function outside of _beginDraw() and _endDraw()?");
        }

        int x1 = rectangle.x;
        int x2 = rectangle.x + rectangle.width - 1;
        int y1 = rectangle.y;
        int y2 = rectangle.y + rectangle.height - 1;

        drawHorizontalLine(x1, y1, x2);
        drawHorizontalLine(x1, y2, x2);

        drawVerticalLine(x1, y1, y2);
        drawVerticalLine(x2, y1, y2); // Fill in the "missing" pixel.
    }

    void SFMLGraphics::fillRectangle(const Rectangle& rectangle)
    {
        if (mClipStack.empty())
        {
            throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw function outside of _beginDraw() and _endDraw()?");
        }

        const ClipRectangle& top = mClipStack.top();

        const float x = static_cast<float>(rectangle.x + top.xOffset);
        const float y = static_cast<float>(rectangle.y + top.yOffset);
        const float w = static_cast<float>(rectangle.width);
        const float h = static_cast<float>(rectangle.height);

        sf::Vertex rect[4] = 
        {
            sf::Vertex(sf::Vector2f(x, y), mSfmlColor),
            sf::Vertex(sf::Vector2f(x + w, y), mSfmlColor),
            sf::Vertex(sf::Vector2f(x + w, y + h), mSfmlColor),
            sf::Vertex(sf::Vector2f(x, y + h), mSfmlColor)
        };

        mTarget->draw(rect, 4, sf::Quads);
    }

    void SFMLGraphics::drawText(const std::string& text,
                                int x,
                                int y,
                                Alignment alignment)
    {
        if (mFont == NULL)
        {
            GCN_EXCEPTION("No font set in graphics.");
        }

        if (alignment == Graphics::Center)
        {
            const int textWidth = mFont->getWidth(text);

            x -= textWidth / 2;
        }
        else if (alignment == Graphics::Right)
        {
            const int textWidth = mFont->getWidth(text);

            x -= textWidth;
        }

        mFont->drawString(this, text, x, y);
    }

    void SFMLGraphics::setColor(const Color& color)
    {
        mColor = color;
        mSfmlColor = convertGuichanColorToSFMLColor(color);
    }

    const Color& SFMLGraphics::getColor() const
    {
        return mColor;
    }

    void SFMLGraphics::setFont(Font* font)
    {
        mFont = font;
    }

    sf::View SFMLGraphics::convertClipRectangleToView(const ClipRectangle& rectangle) const
    {
        const sf::Vector2f& currentViewSize = mContextView.getSize();

        sf::FloatRect clipRectangle;
        clipRectangle.top    = static_cast<float>(rectangle.y);
        clipRectangle.left   = static_cast<float>(rectangle.x);
        clipRectangle.width  = static_cast<float>(rectangle.width);
        clipRectangle.height = static_cast<float>(rectangle.height);

        // Use normalized viewport coordinates to emulate clipping.
        // Special case: if the viewport size is 0, then the width/height is 0.
        sf::FloatRect clipViewport;
        clipViewport.top    = currentViewSize.y == 0 ? 0 : (rectangle.y / (currentViewSize.y - PIXEL_ALIGNMENT_OFFSET));
        clipViewport.left   = currentViewSize.x == 0 ? 0 : (rectangle.x / (currentViewSize.x - PIXEL_ALIGNMENT_OFFSET));
        clipViewport.width  = currentViewSize.x == 0 ? 0 : (rectangle.width / (currentViewSize.x - PIXEL_ALIGNMENT_OFFSET));
        clipViewport.height = currentViewSize.y == 0 ? 0 : (rectangle.height / (currentViewSize.y - PIXEL_ALIGNMENT_OFFSET));

        sf::View clippingView(clipRectangle);
        clippingView.setViewport(clipViewport);

        return clippingView;
    }

    void SFMLGraphics::_drawFauxPixel(int x, int y) {
        float px = static_cast<float>(x);
        float py = static_cast<float>(y);

        sf::Vertex rect[4] = 
        {
            sf::Vertex(sf::Vector2f(px, py), mSfmlColor),
            sf::Vertex(sf::Vector2f(px + 1, py), mSfmlColor),
            sf::Vertex(sf::Vector2f(px + 1, py + 1), mSfmlColor),
            sf::Vertex(sf::Vector2f(px, py + 1), mSfmlColor)
        };

        mTarget->draw(rect, 4, sf::Quads);
    }

    void SFMLGraphics::drawHorizontalLine(int x1, int y, int x2) {
        if (mClipStack.empty())
        {
            throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw function outside of _beginDraw() and _endDraw()?");
        }

        const ClipRectangle& top = mClipStack.top();

        x1 += top.xOffset;
        x2 += top.xOffset;
        y += top.yOffset;

        if (y < top.y || y >= top.y + top.height)
        {
            return;
        }

        if(x1 > x2) {
            std::swap(x1, x2);
        }

        if (top.x > x1)
        {
            if (top.x > x2)
            {
                return;
            }

            x1 = top.x;
        }

        if (top.x + top.width <= x2)
        {
            if (top.x + top.width <= x1)
            {
                return;
            }

            x2 = top.x + top.width - 1;
        }

        // Overdraw by 1 pixel; Widgets expect this behavior
        x2 += 1;

        float lineX1 = static_cast<float>(x1);
        float lineX2 = static_cast<float>(x2);
        float lineY = static_cast<float>(y);

        sf::Vertex rect[4] = 
        {
            sf::Vertex(sf::Vector2f(lineX1, lineY), mSfmlColor),
            sf::Vertex(sf::Vector2f(lineX2, lineY), mSfmlColor),
            sf::Vertex(sf::Vector2f(lineX2, lineY + 1), mSfmlColor),
            sf::Vertex(sf::Vector2f(lineX1, lineY + 1), mSfmlColor)
        };

        mTarget->draw(rect, 4, sf::Quads);
    }

    void SFMLGraphics::drawVerticalLine(int x, int y1, int y2) {
        if (mClipStack.empty())
        {
            throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw function outside of _beginDraw() and _endDraw()?");
        }

        const ClipRectangle& top = mClipStack.top();

        x += top.xOffset;
        y1 += top.yOffset;
        y2 += top.yOffset;

        if (x < top.x || x >= top.x + top.width)
        {
            return;
        }

        if(y1 > y2) {
            std::swap(y1, y2);
        }

        if (top.y > y1)
        {
            if (top.y > y2)
            {
                return;
            }

            y1 = top.y;
        }

        if (top.y + top.height <= y2)
        {
            if (top.y + top.height <= y1)
            {
                return;
            }

            y2 = top.y + top.height - 1;
        }

        // Overdraw by 1 pixel; Widgets expect this behavior
        y2 += 1;
        
        float lineX = static_cast<float>(x);
        float lineY1 = static_cast<float>(y1);
        float lineY2 = static_cast<float>(y2);

        sf::Vertex rect[4] = 
        {
            sf::Vertex(sf::Vector2f(lineX, lineY1), mSfmlColor),
            sf::Vertex(sf::Vector2f(lineX, lineY2), mSfmlColor),
            sf::Vertex(sf::Vector2f(lineX + 1, lineY2), mSfmlColor),
            sf::Vertex(sf::Vector2f(lineX + 1, lineY1), mSfmlColor)
        };

        mTarget->draw(rect, 4, sf::Quads);
    }

    void SFMLGraphics::drawBresenham(int x1, int y1, int x2, int y2) {
        if (mClipStack.empty())
        {
            throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw function outside of _beginDraw() and _endDraw()?");
        }

        const ClipRectangle& top = mClipStack.top();

        x1 += top.xOffset;
        x2 += top.xOffset;
        y1 += top.yOffset;
        y2 += top.yOffset;

        int dx = std::abs(x2 - x1);
        int dy = std::abs(y2 - y1);

        if (dx > dy)
        {
            if (x1 > x2)
            {
                std::swap(x1, x2);
                std::swap(y1, y2);
            }

            if (y1 < y2)
            {
                int y = y1;
                int p = 0;

                for (int x = x1; x <= x2; x++)
                {
                    if (top.isContaining(x, y))
                    {
                        _drawFauxPixel(x, y);
                    }

                    p += dy;

                    if (p * 2 >= dx)
                    {
                        y++;
                        p -= dx;
                    }
                }
            }
            else
            {
                int y = y1;
                int p = 0;

                for (int x = x1; x <= x2; x++)
                {
                    if (top.isContaining(x, y))
                    {
                        _drawFauxPixel(x, y);
                    }

                    p += dy;

                    if (p * 2 >= dx)
                    {
                        y--;
                        p -= dx;
                    }
                }
            }
        }
        else
        {
            if (y1 > y2)
            {
                std::swap(y1, y2);
                std::swap(x1, x2);
            }

            if (x1 < x2)
            {
                int x = x1;
                int p = 0;

                for (int y = y1; y <= y2; y++)
                {
                    if (top.isContaining(x, y))
                    {
                        _drawFauxPixel(x, y);
                    }

                    p += dx;

                    if (p * 2 >= dy)
                    {
                        x++;
                        p -= dy;
                    }
                }
            }
            else
            {
                int x = x1;
                int p = 0;

                for (int y = y1; y <= y2; y++)
                {
                    if (top.isContaining(x, y))
                    {
                        _drawFauxPixel(x, y);
                    }

                    p += dx;

                    if (p * 2 >= dy)
                    {
                        x--;
                        p -= dy;
                    }
                }
            }
        }
    }
}