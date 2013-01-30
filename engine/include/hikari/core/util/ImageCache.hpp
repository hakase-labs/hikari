#ifndef HIKARI_CORE_UTIL_IMAGECACHE
#define HIKARI_CORE_UTIL_IMAGECACHE

#include "hikari/core/Platform.hpp"
#include "hikari/core/util/ResourceCache.hpp"
#include "hikari/core/util/Service.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace hikari {

    class HIKARI_API ImageCache : public Service, public ResourceCache<sf::Texture> {
    public:
        static const bool USE_SMOOTHING;
        static const bool NO_SMOOTHING;
        static const bool USE_MASKING;
        static const bool NO_MASKING;

    private:
        sf::Color maskColor;
        bool enableMask;
        bool enableSmoothing;

    protected:
        virtual ImageCache::Resource loadResource(const std::string &fileName);

    public:
        ImageCache(bool smoothing, bool masking, const sf::Color &mask = sf::Color(255, 0, 255));

        virtual ~ImageCache() { }
    };

} // hikari

#endif // HIKARI_CORE_UTIL_IMAGECACHE