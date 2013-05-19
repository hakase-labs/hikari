#include "guichan/sfml/sfmlimage.hpp"

#include <SFML/Graphics/Texture.hpp>

#include "guichan/exception.hpp"
#include "guichan/sfml/sfmlimageloader.hpp"

namespace gcn {
    Image* SFMLImageLoader::load(const std::string& filename,
                                bool convertToDisplayFormat)
    {
        sf::Texture *loadedTexture = loadSFMLTexture(filename);

        if (loadedTexture == NULL)
        {
            throw GCN_EXCEPTION(
                    std::string("Unable to load image file: ") + filename);
        }

        Image *image = new SFMLImage(loadedTexture, true);

        if (convertToDisplayFormat)
        {
            image->convertToDisplayFormat();
        }

        return image;
    }

    sf::Texture* SFMLImageLoader::loadSFMLTexture(const std::string& filename)
    {
        sf::Texture *texture = new sf::Texture();

        bool textureLoaded = texture->loadFromFile(filename);

        if(!textureLoaded) {
            delete texture;
            return NULL;
        }

        return texture;
    }
}