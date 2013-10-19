#ifndef HIKARI_CLIENT_GAME_PALETTEHELPERS
#define HIKARI_CLIENT_GAME_PALETTEHELPERS

#include <string>
#include <vector>

#include <SFML/Graphics/Color.hpp>

namespace hikari {

namespace PaletteHelpers {

    /**
     * Loads a palette from the specified file and returns it as a vector of
     * palette entries.
     */
    std::vector<std::vector<sf::Color>> loadPaletteFile(const std::string & filePath);

} // hikari::PaletteHelpers
} // hikari

#endif // HIKARI_CLIENT_GAME_PALETTEHELPERS