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

    GuiService::GuiService(const Json::Value & config, const std::weak_ptr<ImageCache> & imageCache, sf::RenderTarget & renderTarget)
        : Service()
        , renderTarget(renderTarget)
        , gui(new gcn::Gui())
        , graphics(new gcn::SFMLGraphics(renderTarget))
        , input(new gcn::SFMLInput())
        , imageLoader(nullptr)
        , rootWidget(new gcn::Container())
        , rootContainer(new gcn::Container())
        , hudContainer(new gcn::Container())
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

        // The root widget is an internal container for the other "layers" of widgets.
        rootWidget->setSize(256, 240);
        rootWidget->setX(0);
        rootWidget->setY(0);
        rootWidget->setBaseColor(gcn::Color(0, 0, 0, 0));

        // This is where game state widgets go (almost everything)
        rootContainer->setSize(256, 240);
        rootContainer->setX(0);
        rootContainer->setY(0);
        rootContainer->setBaseColor(gcn::Color(0, 0, 0, 0));

        // This is where overlaying widgets go (fps counter, console, etc.)
        hudContainer->setSize(256, 240);
        hudContainer->setX(0);
        hudContainer->setY(0);
        hudContainer->setBaseColor(gcn::Color(0, 0, 0, 0));

        // The HUD container is always the "front"-most widget.
        rootWidget->add(rootContainer.get());
        rootWidget->add(hudContainer.get());
    }

    GuiService::~GuiService() {

    }

    void GuiService::processEvent(sf::Event & evt) {
        if(input) {
            input->pushInput(evt, renderTarget);
        }
    }

    gcn::Gui & GuiService::getGui() {
        return *gui;
    }

    gcn::Container & GuiService::getHudContainer() {
        return *hudContainer;
    }

    gcn::Container & GuiService::getRootContainer() {
        return *rootContainer;
    }

} // hikari