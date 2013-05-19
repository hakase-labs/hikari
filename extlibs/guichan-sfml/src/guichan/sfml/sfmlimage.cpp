#include "guichan/sfml/sfmlimage.hpp"
#include "guichan/sfml/sfmlgraphics.hpp"

#include "guichan/exception.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

namespace gcn
{
    SFMLImage::SFMLImage(sf::Texture* texture, bool autoFree)
    {
        mAutoFree = autoFree;
        mTexture = texture;

        if (mTexture != NULL)
        {
            mImage = mTexture->copyToImage();
        }
    }

    SFMLImage::~SFMLImage()
    {
        if (mAutoFree)
        {
            free();
        }
    }

    sf::Texture* SFMLImage::getTexture() const
    {
        return mTexture;
    }

    int SFMLImage::getWidth() const
    {
        if (mTexture == NULL)
        {
            throw GCN_EXCEPTION("Trying to get the width of a non loaded image.");
        }

        return mTexture->getSize().x;
    }

    int SFMLImage::getHeight() const
    {
        if (mTexture == NULL)
        {
            throw GCN_EXCEPTION("Trying to get the height of a non loaded image.");
        }

        return mTexture->getSize().y;
    }

    Color SFMLImage::getPixel(int x, int y)
    {
        if (mTexture == NULL)
        {
            throw GCN_EXCEPTION("Trying to get a pixel from a non loaded image.");
        }

        if (x < 0 || x >= static_cast<int>(mImage.getSize().x) || y < 0 || y >= static_cast<int>(mImage.getSize().y))
        {
            throw GCN_EXCEPTION("Trying to get a pixel from a location outside image bounds.");
        }

        sf::Color sfmlColor = mImage.getPixel(static_cast<unsigned int>(x), static_cast<unsigned int>(y));

        return SFMLGraphics::convertSFMLColorToGuichanColor(sfmlColor);
    }

    void SFMLImage::putPixel(int x, int y, const Color& color)
    {
        if (mTexture == NULL)
        {
            throw GCN_EXCEPTION("Trying to put a pixel in a non loaded image.");
        }

        if (x < 0 || x >= static_cast<int>(mImage.getSize().x) || y < 0 || y >= static_cast<int>(mImage.getSize().y))
        {
            throw GCN_EXCEPTION("Trying to set a pixel from a location outside image bounds.");
        }

        sf::Color sfmlColor = SFMLGraphics::convertGuichanColorToSFMLColor(color);

        mImage.setPixel(static_cast<unsigned int>(x), static_cast<unsigned int>(y), sfmlColor);
        
        mTexture->update(mImage);
    }

    void SFMLImage::convertToDisplayFormat()
    {
        if (mTexture == NULL)
        {
            throw GCN_EXCEPTION("Trying to convert a non loaded image to display format.");
        }

        // Do nothing since we can't change the display format.
    }

    void SFMLImage::free()
    {
        if(mTexture != NULL) {
            delete mTexture;
        }
    }
}
