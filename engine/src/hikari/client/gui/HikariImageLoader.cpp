#include "hikari/client/gui/HikariImageLoader.hpp"
#include "guichan/sfml/sfmlimage.hpp"

namespace hikari {

    HikariImageLoader::HikariImageLoader(const std::weak_ptr<ImageCache> & imageCache)
        : imageCache(imageCache)
    {

    }

    HikariImageLoader::~HikariImageLoader() {

    }

    gcn::Image* HikariImageLoader::load(const std::string& filename, bool convertToDisplayFormat) {
        ImageCache::Resource loadedImage = loadTextureFromCache(filename);
        gcn::Image * image = nullptr;

        if(loadedImage) {
            image = new gcn::SFMLImage(loadedImage.get(), false);

            if (convertToDisplayFormat) {
                image->convertToDisplayFormat();
            }
        }

        return image;
    }

    ImageCache::Resource HikariImageLoader::loadTextureFromCache(const std::string& filename) {
        ImageCache::Resource loadedImage;

        if(auto cache = imageCache.lock()) {
            loadedImage = cache->get(filename);
        }

        return loadedImage;
    }

} // hikari