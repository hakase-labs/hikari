#ifndef HIKARI_CORE_GUI_BOSSMENU
#define HIKARI_CORE_GUI_BOSSMENU

#include "hikari/client/gui/Widget.hpp"
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <vector>

namespace hikari {

    class BossMenu : public Widget {
    private:
        int selectedIndex;
        std::vector<bool> validIndicies;
        sf::Sprite background;
        sf::Sprite frame;
    };

} // hikari

#endif // HIKARI_CORE_GUI_BOSSMENU