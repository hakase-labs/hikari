#include "hikari/client/gui/MenuItem.hpp"

#include <guichan/graphics.hpp>
#include <guichan/font.hpp>

namespace hikari {
namespace gui {

    MenuItem::MenuItem()
        : gcn::Widget()
        , name("")
        , selected(false)
    {
        adjustSize();
    }

    MenuItem::MenuItem(const std::string & name)
        : gcn::Widget()
        , name(name)
        , selected(false)
    {
        adjustSize();
    }

    MenuItem::~MenuItem() {
    }

    void MenuItem::adjustSize() {
        setWidth(getFont()->getWidth(getName()));
        setHeight(getFont()->getHeight());
    }

    const std::string & MenuItem::getName() const {
        return name;
    }

    void MenuItem::setName(const std::string & name) {
        this->name = name;
        adjustSize();
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
        
        graphics->setFont(getFont());
        graphics->drawText(getName(), 0, 0, gcn::Graphics::Left);
    }

    gcn::Rectangle MenuItem::getChildrenArea() {
        return gcn::Rectangle(0, 0, getWidth(), getHeight());
    }

} // hikari::gui
} // hikari
