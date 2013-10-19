#include "hikari/client/game/PaletteHelpers.hpp"

#include "hikari/core/util/FileSystem.hpp"
#include "hikari/core/util/Log.hpp"

#include <json/reader.h>

namespace hikari {
namespace PaletteHelpers {

    std::vector<std::vector<sf::Color>> loadPaletteFile(const std::string & filePath) {
        HIKARI_LOG(debug2) << "Loading palettes...";

        std::vector<std::vector<sf::Color>> palette;

        auto fileContents = FileSystem::openFileRead(filePath);
        Json::Value root;
        Json::Reader reader;

        if(reader.parse(*fileContents, root, false)) {
            auto paletteCount = root.size();

            HIKARI_LOG(debug3) << "Found " << paletteCount << " palette entries.";

            for(decltype(paletteCount) i = 0; i < paletteCount; ++i) {
                const auto & paletteTemplate = root[i];
                const auto & paletteEntries = paletteTemplate["colors"];
                const auto paletteName = paletteTemplate.get("name", "Anonymous palette").asString();

                HIKARI_LOG(debug3) << "Parsing palette \"" << paletteName << "\".";

                std::vector<sf::Color> paletteEntry;
                paletteEntry.reserve(paletteEntries.size());

                for(decltype(paletteEntries.size()) j = 0; j < paletteEntries.size(); ++j) {
                    const auto & colorComponents = paletteEntries[j];
                    const int red   = colorComponents[0u].asInt();
                    const int green = colorComponents[1u].asInt();
                    const int blue  = colorComponents[2u].asInt();
                    const int alpha = colorComponents[3u].asInt();

                    paletteEntry.emplace_back(red, green, blue, alpha);
                }

                palette.push_back(paletteEntry);
            }
        }

        return palette;
    }

} // hikari::PaletteHelpers
} // hikari