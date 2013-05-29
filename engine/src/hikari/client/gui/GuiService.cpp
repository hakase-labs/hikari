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
        , fontImageMap()
        , fontMap()
    {
        // Set up the global image loader (a proxy to the image caching service)
        imageLoader.reset(new gui::HikariImageLoader(imageCache));
        gcn::Image::setImageLoader(imageLoader.get());

        buildFontMap(config["gui"]["fonts"]);
        globalFont = fontMap.at("default");
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

    void GuiService::buildFontMap(const Json::Value & fontConfig) {
        auto fontNames = fontConfig.getMemberNames();

        std::for_each(std::begin(fontNames), std::end(fontNames), [&](const std::string & fontName) {
            HIKARI_LOG(debug3) << "Creating font \"" << fontName << "\"";

            auto & fontSettings = fontConfig[fontName];
            bool isValid = false;

            if(fontSettings.isMember("image")) {
                if(fontSettings.isMember("glyphs")) {
                    if(fontSettings.isMember("glyphSize")) {
                        isValid = true;
                    }
                }
            }

            if(isValid) {
                std::string imageName = fontSettings["image"].asString();
                std::string glyphs = fontSettings["glyphs"].asString();
                int glyphSize = fontSettings["glyphSize"].asInt();
                auto glyphImage = std::shared_ptr<gcn::Image>(gcn::Image::load(imageName));

                if(glyphImage) {
                    this->fontImageMap[fontName] = glyphImage;
                    this->fontMap[fontName] = std::make_shared<gcn::FixedImageFont>(glyphImage.get(), glyphSize, glyphs);
                    HIKARI_LOG(debug3) << "Font \"" << fontName << "\" successfully loaded.";
                } else {
                    HIKARI_LOG(warning) << "Font image for \"" << fontName << "\" could not be loaded; ignoring.";
                }
            } else {
                HIKARI_LOG(warning) << "Font \"" << fontName << "\" is not properly defined; ignoring.";
            }
        });
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