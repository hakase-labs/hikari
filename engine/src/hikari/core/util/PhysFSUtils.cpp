#include "hikari/core/util/PhysFSUtils.hpp"
#include "hikari/core/util/FileSystem.hpp"
#include "hikari/core/util/PhysFS.hpp"
#include <json/reader.h>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <sstream>
#include <stdexcept>

namespace hikari {

    bool PhysFSUtils::loadImage(const std::string &fileName, sf::Texture &texture) {
        if(PhysFS::exists(fileName)) {
            auto fs = FileSystem::openFileRead(fileName);

            fs->seekg(0, std::ios::end);
            auto length = fs->tellg();
            fs->seekg(0, std::ios::beg);

            std::unique_ptr<char[]> buffer(new char[static_cast<unsigned int>(length)]);

            fs->read(buffer.get(), length);

            // Load into a sf::Image first so you can apply color keying
            sf::Image rawImage;
            bool success = rawImage.loadFromMemory(buffer.get(), static_cast<size_t>(length));
            rawImage.createMaskFromColor(sf::Color(255, 0, 255));

            // Then copy the color-keyed pixels to the texture
            success &= texture.create(rawImage.getSize().x, rawImage.getSize().y);
            texture.update(rawImage);
            texture.setSmooth(false);
            texture.setRepeated(false);

            return success;
        } else {
            return false;
        }
    }

    Json::Value PhysFSUtils::loadJson(const std::string &fileName) {
        Json::Value result;

        if(PhysFS::exists(fileName)) {
            auto fs = FileSystem::openFileRead(fileName);
            Json::Reader reader;
            
            bool parseSuccessful = reader.parse(*fs, result);

            if(!parseSuccessful) {
                throw std::runtime_error("Error parsing JSON: " + fileName + "\"; " + reader.getFormatedErrorMessages());
            }
        } else {
            throw std::runtime_error("Error loading JSON, file does not exist: \"" + fileName + "\"");
        }

        return result;
    }

    const std::string PhysFSUtils::readFileAsString(const std::string &fileName) {
        std::stringstream result;

        if(PhysFS::exists(fileName)) {
            auto fs = FileSystem::openFileRead(fileName);

            result << fs->rdbuf();
        }

        return result.str();
    }

} // hikari