#include "hikari/core/util/ImageCache.hpp"
#include "hikari/core/util/FileSystem.hpp"
#include "hikari/core/util/Log.hpp"
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

        Resource texture(new sf::Texture());

        if(FileSystem::exists(fileName)) {
            auto handle = FileSystem::openFile(fileName);

            // Figure out how many bytes the image is
            handle->seekg(0, std::ios::end);
            auto length = handle->tellg();
            handle->seekg(0, std::ios::beg);

            HIKARI_LOG(debug4) << "Image data length: " << length << " bytes";

            // Create a buffer to load image data
            std::unique_ptr<char[]> buffer(new char[static_cast<unsigned int>(length)]);

            handle->read(buffer.get(), length);

            sf::Image imageData;

            // Fill an image buffer with pixel data
            if(imageData.loadFromMemory(buffer.get(), static_cast<size_t>(length))) {
                if(enableMask) {
                    imageData.createMaskFromColor(maskColor);
                }

                // Then copy the processed pixels to the texture
                if(texture->create(imageData.getSize().x, imageData.getSize().y)) {
                    texture->update(imageData);
                    texture->setSmooth(enableSmoothing);
                    texture->setRepeated(false);
                } else {
                    // Throw.
                    // Couldn't create texture for some reason.
                    std::stringstream ss;
                    ss << "Couldn't create texture for \"" << fileName << "\".";
                    throw std::runtime_error(ss.str().c_str());
                }
            } else {
                // Throw.
                // Couldn't load image data from memory.
                std::stringstream ss;
                ss << "Couldn't load image data from memory for \"" << fileName << "\".";
                throw std::runtime_error(ss.str().c_str());
            }
        } else {
            // Throw file not found exception
            std::stringstream ss;
            ss << "Couldn't load image because file was not found. File: \"" << fileName << "\".";
            throw std::runtime_error(ss.str().c_str());
        }

        return texture;
    }

} // hikari