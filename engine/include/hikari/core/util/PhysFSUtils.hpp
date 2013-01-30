#ifndef HIKARI_CORE_UTIL_PHYSFSUTILS
#define HIKARI_CORE_UTIL_PHYSFSUTILS

#include "hikari/core/Platform.hpp"
#include <json/value.h>
#include <string>

namespace sf {
    class Texture;
}

namespace hikari {

    class HIKARI_API PhysFSUtils {
    public:
        /**
         * Loads an SFML image using PhysFS.
         * @param fileName  The PhysFS-style file path or name of the image to load.
         * @param image     The SFML Image to load the data in to.
         */
        static bool loadImage(const std::string &fileName, sf::Texture &texture);

        static Json::Value loadJson(const std::string &fileName);

        /**
         * Opens a file and reads its contents into a string, returning the string.
         */
        static const std::string readFileAsString(const std::string &fileName);
    };

} // hikari

#endif // HIKARI_CORE_UTIL_PHYSFSUTILS