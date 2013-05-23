#ifndef HIKARI_CLIENT_GUI_GUISERVICE
#define HIKARI_CLIENT_GUI_GUISERVICE

#include "hikari/core/util/Service.hpp"

#include <memory>
#include <string>

namespace gcn {
    class Graphics;
    class Container;
    class Image;
    class ImageLoader;
    class Input;
    class FixedImageFont;
    class Gui;
}

namespace Json {
    class Value;
}

namespace sf {
    class RenderWindow;
}

namespace hikari {

    class ImageCache;

    class GuiService : public Service {
    private:
        std::unique_ptr<gcn::Gui> gui;
        std::unique_ptr<gcn::Graphics> graphics;
        std::unique_ptr<gcn::Input> input;
        std::unique_ptr<gcn::ImageLoader> imageLoader;
        std::unique_ptr<gcn::Container> rootWidget;
        std::unique_ptr<gcn::Image> globalFontImage;
        std::unique_ptr<gcn::FixedImageFont> globalFont;

    public:
        explicit GuiService(const Json::Value & config, const std::weak_ptr<ImageCache> & imageCache, sf::RenderWindow & window);
        virtual ~GuiService();

        gcn::Gui & getGui();
    };

} // hikari

#endif // HIKARI_CLIENT_GUI_GUISERVICE
