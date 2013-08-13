#include "hikari/client/gui/MenuItem.hpp"

#include <guichan/graphics.hpp>

namespace hikari {
namespace gui {

    MenuItem::MenuItem()
        : gcn::Widget()
        , name("")
        , selected(false)
    {
    }

    MenuItem::MenuItem(const std::string & name)
        : gcn::Widget()
        , name(name)
        , selected(false)
    {
    }

    MenuItem::~MenuItem() {
    }

    const std::string & MenuItem::getName() const {
        return name;
    }

    void MenuItem::setName(const std::string & name) {
        this->name = name;
    }

    bool MenuItem::isSelected() const {
        return selected;
    }

    void MenuItem::select() {
        selected = true;
    }

    void MenuItem::deselect() {
        selected = false;
    }

    void MenuItem::draw(gcn::Graphics* graphics) {
        graphics->setColor(isSelected() ? getSelectionColor() : getForegroundColor());
        graphics->fillRectangle(0, 0, getWidth(), getHeight());
    }

} // hikari::gui
} // hikari
