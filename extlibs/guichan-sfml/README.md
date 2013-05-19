guichan-sfml
============

A set of classes which provide an SFML-powered backend for [Guichan](http://gitorious.org/guichan). 

## Implemented Features ##

* `SFMLFont`: `sf::Font` rendering, `gcn::Graphics::Alignment` supported
* `SFMLGraphics`: Drawing images, lines, points, and rectangles (outline or filled), clip rectangles are emulated using `sf::View`s
* `SFMLImage`: Wrapper for `sf::Texture` which also supports pixel reading/manipulation
* `SFMLImageLoader`: Load images via SFML's `loadFromFile`
* `SFMLInput`: Input events (keyboard, mouse, mouse wheel, window focus)

## Example Usage ##

The following example shows how to create an SFML window and integrate Guichan. A few widgets are also created, including
one that draws an image and one that uses fonts.

```c++
#include <guichan/sfml.hpp>
#include <guichan/gui.hpp>
#include <guichan/widgets/button.hpp>
#include <guichan/widgets/checkbox.hpp>
#include <guichan/widgets/container.hpp>
#include <guichan/widgets/icon.hpp>

#include <SFML/Graphics.hpp>

#include <memory>

int main() {
    sf::VideoMode videoMode(640, 480, 32);
    std::unique_ptr<sf::RenderWindow> window(new sf::RenderWindow(videoMode, "Guichan SFML Test"));
    std::unique_ptr<gcn::SFMLGraphics> guiGraphics(new gcn::SFMLGraphics(*window));
    std::unique_ptr<gcn::SFMLFont> guiFont(new gcn::SFMLFont("arial.ttf", 10));
    std::unique_ptr<gcn::SFMLInput> guiInput(new gcn::SFMLInput());
    std::unique_ptr<gcn::SFMLImageLoader> guiImageLoader(new gcn::SFMLImageLoader());

    gcn::Image::setImageLoader(guiImageLoader.get());
    gcn::Widget::setGlobalFont(guiFont.get());

    std::unique_ptr<gcn::Gui> gui(new gcn::Gui());
    std::unique_ptr<gcn::Container> topContainer(new gcn::Container());
    std::unique_ptr<gcn::Icon> guiIcon(new gcn::Icon("sfml-small.png"));
    std::unique_ptr<gcn::Button> guiButton(new gcn::Button("Clicky!"));
    std::unique_ptr<gcn::CheckBox> guiCheckbox(new gcn::CheckBox());

    gui->setInput(guiInput.get());
    gui->setGraphics(guiGraphics.get());
    gui->setTop(topContainer.get());

    guiButton->setWidth(100);
    guiButton->setHeight(40);

    guiCheckbox->setWidth(16);
    guiCheckbox->setHeight(16);

    topContainer->setSize(400, 300);
    topContainer->add(guiIcon.get(), 50, 50);
    topContainer->add(guiButton.get(), 100, 100);
    topContainer->add(guiCheckbox.get(), 210, 100);

    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
            }

            guiInput->pushInput(event, *window);
        }

        window->clear();

        gui->logic();
        gui->draw();

        window->display();
    }

    return 0;
}
```
