#ifndef GCN_SFMLIMAGE_HPP
#define GCN_SFMLIMAGE_HPP

#include "guichan/color.hpp"
#include "guichan/platform.hpp"
#include "guichan/image.hpp"

#include <SFML/Graphics/Image.hpp>

namespace sf {
    class Texture;
}

namespace gcn
{
    /**
     * SFML implementation of Image.
     */
    class GCN_EXTENSION_DECLSPEC SFMLImage : public Image
    {
    public:
        /**
         * Constructor. Load an image from an SFML surface.
         *
         * @param texture the texture to use.
         * @param autoFree true if the texture should automatically be deleted.
         */
        SFMLImage(sf::Texture* texture, bool autoFree);

        /**
         * Destructor.
         */
        virtual ~SFMLImage();

        /**
         * Gets the SFML Texture for the image.
         *
         * @return the SFML Texture for the image.
         */
        virtual sf::Texture* getTexture() const;


        // Inherited from Image

        virtual void free();

        virtual int getWidth() const;

        virtual int getHeight() const;

        virtual Color getPixel(int x, int y);

        virtual void putPixel(int x, int y, const Color& color);

        virtual void convertToDisplayFormat();

    protected:
        sf::Texture* mTexture; // Used to store texture for graphics card
        sf::Image mImage;     // Used to store texture pixels for manipulation
        bool mAutoFree;
    };
}

#endif // end GCN_SFMLIMAGE_HPP