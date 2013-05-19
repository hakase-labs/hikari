#ifndef GCN_SFMLIMAGELOADER_HPP
#define GCN_SFMLIMAGELOADER_HPP

#include "guichan/imageloader.hpp"
#include "guichan/platform.hpp"

namespace sf {
    class Texture;
}

namespace gcn
{
    class Image;

    /**
     * SFML implementation of ImageLoader.
     */
    class GCN_EXTENSION_DECLSPEC SFMLImageLoader : public ImageLoader
    {
    public:

        // Inherited from ImageLoader

        virtual Image* load(const std::string& filename, bool convertToDisplayFormat = true);

    protected:
        virtual sf::Texture* loadSFMLTexture(const std::string& filename);
    };
}

#endif // end GCN_SFMLIMAGELOADER_HPP
