#include "hikari/client/gui/GuiService.hpp"
#include "hikari/client/gui/HikariImageLoader.hpp"
#include "hikari/client/Services.hpp"
#include "hikari/core/util/ImageCache.hpp"
#include "hikari/core/util/Log.hpp"

#include <SFML/Graphics.hpp>

#include <guichan/sfml.hpp>
#include <guichan/gui.hpp>
#include <guichan/widgets/container.hpp>
#include <guichan/hakase/fixedimagefont.hpp>
#include <guichan/hakase/functoractionlistener.hpp>
#include <guichan/actionevent.hpp>

#include <json/reader.h>

namespace hikari {

    GuiService::GuiService(const Json::Value & config, const std::weak_ptr<ImageCache> & imageCache, sf::RenderWindow & window)
        : Service()
        , gui(new gcn::Gui())
        , graphics(new gcn::SFMLGraphics(window))
        , input(new gcn::SFMLInput())
        , imageLoader(nullptr)
        , rootWidget(new gcn::Container())
        , globalFontImage(nullptr)
        , globalFont(nullptr)
    {
        // Set up the global image loader (a proxy to the image caching service)
        imageLoader.reset(new gui::HikariImageLoader(imageCache));
        gcn::Image::setImageLoader(imageLoader.get());

        // Load the global/default font
        const std::string fontImagePath = config["gui"]["font"]["image"].asString();
        const std::string fontGlyphMap = config["gui"]["font"]["glyphs"].asString();
        const int fontGlyphSize = config["gui"]["font"]["glyphSize"].asInt();

        globalFontImage.reset(gcn::Image::load(fontImagePath));
        globalFont.reset(new gcn::FixedImageFont(globalFontImage.get(), fontGlyphSize, fontGlyphMap));

        gcn::Widget::setGlobalFont(globalFont.get());

        // Set up the GUI's inputs/outputs
        gui->setInput(input.get());
        gui->setGraphics(graphics.get());
        gui->setTop(rootWidget.get());
    }

    GuiService::~GuiService() {

    }

} // hikari