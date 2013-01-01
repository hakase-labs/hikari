#include <hikari/core/util/ImageCache.hpp>
#include <hikari/core/util/PhysFS.hpp>
#include <hikari/core/util/PhysFSUtils.hpp>
#include <hikari/core/util/Log.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <iostream>
#include <sstream>

namespace hikari {

    const bool ImageCache::USE_SMOOTHING = true;
    const bool ImageCache::NO_SMOOTHING = false;
    const bool ImageCache::USE_MASKING = true;
    const bool ImageCache::NO_MASKING = false;

    ImageCache::ImageCache(bool smoothing, bool masking, const sf::Color &mask)
        : enableSmoothing(smoothing)
        , enableMask(masking)
        , maskColor(mask) { 

    }

    ImageCache::Resource ImageCache::loadResource(const std::string &fileName) {
        HIKARI_LOG(debug) << "Caching image: " << fileName;
        
        // TODO: Add some exception throwing or catching if something goes wrong.
        Resource texture(new sf::Texture());

        bool success = PhysFSUtils::loadImage(fileName, *texture);
        if(!success) {
            std::stringstream ss;
            ss << "Loading of resource \"" << fileName << "\" failed.";
            throw std::runtime_error(ss.str().c_str());
        }

        texture->setSmooth(enableSmoothing);
        //if(enableMask) {
        //    image->createMaskFromColor(maskColor);
        //}

        return texture;
    }

} // hikari