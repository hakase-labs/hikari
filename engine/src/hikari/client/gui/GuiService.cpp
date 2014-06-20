#include "hikari/client/gui/GuiService.hpp"
#include "hikari/client/gui/HikariImageLoader.hpp"
#include "hikari/client/gui/InputHelper.hpp"
#include "hikari/client/Services.hpp"
#include "hikari/core/util/FileSystem.hpp"
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

    const std::string GuiService::DEFAULT_FONT_NAME = "default";

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
        , fontImageMap()
        , fontMap()
    {
        // Set up the global image loader (a proxy to the image caching service)
        imageLoader.reset(new gui::HikariImageLoader(imageCache));
        gcn::Image::setImageLoader(imageLoader.get());

        buildFontMap(config["gui"]["fonts"]);

        if(auto globalFont = getFontByName(DEFAULT_FONT_NAME)) {
            gcn::Widget::setGlobalFont(globalFont.get());
        }

        // Set up the GUI's inputs/outputs
        gui->setInput(input.get());
        gui->setGraphics(graphics.get());
        gui->setTop(rootWidget.get());

        // Base the root containers size off the size of the render target
        unsigned int windowWidth = renderTarget.getSize().x;
        unsigned int windowHeight = renderTarget.getSize().y;

        // The root widget is an internal container for the other "layers" of widgets.
        rootWidget->setSize(windowWidth, windowHeight);
        rootWidget->setX(0);
        rootWidget->setY(0);
        rootWidget->setBaseColor(gcn::Color(0, 0, 0, 0));

        // This is where game state widgets go (almost everything)
        rootContainer->setSize(windowWidth, windowHeight);
        rootContainer->setX(0);
        rootContainer->setY(0);
        rootContainer->setBaseColor(gcn::Color(0, 0, 0, 0));

        // This is where overlaying widgets go (fps counter, console, etc.)
        hudContainer->setSize(windowWidth, windowHeight);
        hudContainer->setX(0);
        hudContainer->setY(0);
        hudContainer->setBaseColor(gcn::Color(0, 0, 0, 0));

        // The HUD container is always the "front"-most widget.
        rootWidget->add(rootContainer.get());
        rootWidget->add(hudContainer.get());

        // Set up key mappings
        // TODO: Get actual key mappings
        gui::InputHelper::setMappedButtonForKey(gcn::Key::Up, Input::BUTTON_UP);
        gui::InputHelper::setMappedButtonForKey(gcn::Key::Right, Input::BUTTON_RIGHT);
        gui::InputHelper::setMappedButtonForKey(gcn::Key::Down, Input::BUTTON_DOWN);
        gui::InputHelper::setMappedButtonForKey(gcn::Key::Left, Input::BUTTON_LEFT);
        gui::InputHelper::setMappedButtonForKey('A', /* gcn::Key::A */ Input::BUTTON_SHOOT);
        gui::InputHelper::setMappedButtonForKey('S', /* gcn::Key::S */ Input::BUTTON_JUMP);
        gui::InputHelper::setMappedButtonForKey(gcn::Key::Enter, Input::BUTTON_START);
        gui::InputHelper::setMappedButtonForKey(gcn::Key::RightShift, Input::BUTTON_SELECT);
        gui::InputHelper::setMappedButtonForKey(gcn::Key::Escape, Input::BUTTON_CANCEL);
    }

    GuiService::~GuiService() {

    }

    void GuiService::buildFontMap(const Json::Value & fontConfig) {
        auto fontNames = fontConfig.getMemberNames();

        std::for_each(std::begin(fontNames), std::end(fontNames), [&](const std::string & fontName) {
            HIKARI_LOG(debug3) << "Creating font \"" << fontName << "\"";

            auto & fontSettings = fontConfig[fontName];
            bool isConfigValid = false;

            if(fontSettings.isMember("image")) {
                if(fontSettings.isMember("glyphs")) {
                    if(fontSettings.isMember("glyphSize")) {
                        isConfigValid = true;
                    }
                }
            }

            if(isConfigValid) {
                std::string imageName = fontSettings["image"].asString();
                std::string glyphs = fontSettings["glyphs"].asString();
                int glyphSize = fontSettings["glyphSize"].asInt();

                if(FileSystem::exists(imageName)) {
                    auto glyphImage = std::shared_ptr<gcn::Image>(gcn::Image::load(imageName));

                    if(glyphImage) {
                        this->fontImageMap.insert(std::make_pair(fontName, glyphImage));

                        auto font = std::make_shared<gcn::FixedImageFont>(glyphImage.get(), glyphSize, glyphs);
                        this->fontMap.insert(std::make_pair(fontName, font));

                        HIKARI_LOG(debug3) << "Font \"" << fontName << "\" successfully loaded.";
                    } else {
                        HIKARI_LOG(warning) << "Font image for \"" << fontName << "\" could not be loaded; ignoring.";
                    }
                } else {
                    HIKARI_LOG(warning) << "Font image for \"" << fontName << "\" could not be found; ignoring.";
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

    std::shared_ptr<gcn::Font> GuiService::getFontByName(const std::string & fontName) const {
        std::shared_ptr<gcn::Font> font;

        auto found = fontMap.find(fontName);

        if(found != fontMap.end()) {
            font = found->second;
        }

        return font;
    }

    void GuiService::renderRootContainer() {
        bool isHudVisible = hudContainer->isVisible();
        hudContainer->setVisible(false);

        gui->draw();

        hudContainer->setVisible(isHudVisible);
    }

    void GuiService::renderRootContainer(sf::RenderTarget & target) {
        sf::RenderTarget & oldTarget = graphics->getRenderTarget();

        graphics->setRenderTarget(target);
        renderRootContainer();

        graphics->setRenderTarget(oldTarget);
    }

    void GuiService::renderAsTop(gcn::Widget * widget, sf::RenderTarget & target) {
        if(widget) {
            sf::RenderTarget & oldTarget = graphics->getRenderTarget();

            graphics->setRenderTarget(target);
            widget->_draw(graphics.get());

            graphics->setRenderTarget(oldTarget);
        }
    }

    void GuiService::renderHudContainer() {
        bool isRootVisible = rootContainer->isVisible();
        rootContainer->setVisible(false);

        gui->draw();

        rootContainer->setVisible(isRootVisible);
    }

} // hikari
