#ifndef HIKARI_CLIENT_SERVICES
#define HIKARI_CLIENT_SERVICES

#include <string>

namespace hikari {
namespace Services {

    /*enum Enum {
        AUDIO,
        GAMEPROGRESS,
        IMAGECACHE,
        MAPLOADER,
        SCRIPTING,
        GUIFONT
    };*/

    const static std::string AUDIO = "Audio";
    const static std::string GAMEPROGRESS = "GameProgress";
    const static std::string IMAGECACHE = "ImageCache";
    const static std::string MAPLOADER = "MapLoader";
    const static std::string SCRIPTING = "Scripting";
    const static std::string GUIFONT = "GuiFont";
    const static std::string ANIMATIONSETCACHE = "AnimationSetCache";
    const static std::string ITEMFACTORY = "ItemFactory";

} // hikari::direction
} // hikari

#endif // HIKARI_CLIENT_GAME_DIRECTION