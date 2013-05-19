#ifndef HIKARI_CLIENT_GUI_HIKARIIMAGELOADER
#define HIKARI_CLIENT_GUI_HIKARIIMAGELOADER

#include <memory>
#include "guichan/imageloader.hpp"
#include "hikari/core/util/ImageCache.hpp"

namespace gcn {
    class Image;
}

namespace hikari {

    class HikariImageLoader : public gcn::ImageLoader {
    private:
        std::weak_ptr<ImageCache> imageCache;

    public:
        explicit HikariImageLoader(const std::weak_ptr<ImageCache> & imageCache);
        virtual ~HikariImageLoader();
        virtual gcn::Image* load(const std::string& filename, bool convertToDisplayFormat = true);

    protected:
        ImageCache::Resource loadTextureFromCache(const std::string& filename);
    };

} // hikari

#endif // HIKARI_CLIENT_GUI_HIKARIIMAGELOADER