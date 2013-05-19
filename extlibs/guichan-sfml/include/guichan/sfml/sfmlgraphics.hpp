#ifndef GCN_SFMLGRAPHICS_HPP
#define GCN_SFMLGRAPHICS_HPP

#include "guichan/color.hpp"
#include "guichan/graphics.hpp"
#include "guichan/platform.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf
{
    class RenderTarget;
}

namespace gcn
{
    class Image;
    class Rectangle;

    /**
     * SFML implementation of the Graphics.
     */
    class GCN_EXTENSION_DECLSPEC SFMLGraphics : public Graphics
    {
    public:

        // Needed so that drawImage(gcn::Image *, int, int) is visible.
        using Graphics::drawImage;

        /**
         * Constructor.
         */
        SFMLGraphics(sf::RenderTarget& target);

        /**
         * Sets the RenderTarget to draw to. The target can be any RenderTarget.
         * This funtion also pushes a clip areas corresponding to the dimension 
         * of the target.
         *
         * @param target the target to draw to.
         */
        virtual void setRenderTarget(sf::RenderTarget& target);

        /**
         * Gets the RenderTarget.
         *
         * @return the RenderTarget.
         */
        virtual sf::RenderTarget& getRenderTarget() const;

        // Inherited from Graphics

        virtual void _beginDraw();

        virtual void _endDraw();

        virtual bool pushClipArea(Rectangle area);

        virtual void popClipArea();

        virtual void drawImage(const Image* image,
                               int srcX,
                               int srcY,
                               int dstX,
                               int dstY,
                               int width,
                               int height);

        virtual void drawPoint(int x, int y);

        virtual void drawLine(int x1, int y1, int x2, int y2);

        virtual void drawRectangle(const Rectangle& rectangle);

        virtual void fillRectangle(const Rectangle& rectangle);

        virtual void drawText(const std::string& text,
                              int x,
                              int y,
                              Alignment alignment = Left);

        virtual void setColor(const Color& color);

        virtual const Color& getColor() const;

        virtual void setFont(Font* font);

        /**
         * Converts an SFML color to a Guichan color.
         */
        static Color convertSFMLColorToGuichanColor(const sf::Color& color);

        /**
         * Converts a Guichan color to an SFML color.
         */
        static sf::Color convertGuichanColorToSFMLColor(const Color& color);

    protected:
        /**
         * Converts a ClipRectangle to an sf::View to be used for clipping by a RenderTarget.
         */
        sf::View convertClipRectangleToView(const ClipRectangle& rectangle) const;

        /**
         * Emulates drawing a pixel at (x, y) by drawing a quad. This should only
         * be called inside of another drawing method because it does not check
         * the clip rectangle stack.
         * @param x
         * @param y
         */
        void _drawFauxPixel(int x, int y);

        /**
         * Draws a horizontal line from (x1, y) to (x2, y).
         * @param x1 the starting x coordinate
         * @param y
         * @param x2 the terminating x coordinate
         */
        void drawHorizontalLine(int x1, int y, int x2);

        /**
         * Draws a vertical line from (x, y1) to (x, y2).
         * @param y1 the starting y coordinate
         * @param x
         * @param y2 the terminating y coordinate
         */
        void drawVerticalLine(int x, int y1, int y2);

        /**
         * Draws a line from (x1, y1) to (x2, y2) using Bresenham's line algorithm.
         * @param x1
         * @param y1
         * @param x2
         * @param y2
         */
        void drawBresenham(int x1, int y1, int x2, int y2);

        sf::RenderTarget* mTarget;
        sf::View mContextView;
        sf::Vector2f mSize;
        sf::Sprite mSprite;
        sf::Color mSfmlColor;
        Color mColor;

        /**
         * This offset is used for "exact pixelization".
         * http://www.opengl.org/archives/resources/faq/technical/transformations.htm#tran0030
         */
        static const float PIXEL_ALIGNMENT_OFFSET;
    };
}

#endif // end GCN_SFMLGRAPHICS_HPP