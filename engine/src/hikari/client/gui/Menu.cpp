#include "hikari/client/gui/Menu.hpp"
#include "hikari/client/gui/MenuItem.hpp"

#include <guichan/graphics.hpp>

#include <algorithm>

namespace hikari {
namespace gui {

    Menu::Menu()
        : gcn::Container()
        , gcn::KeyListener()
        , items()
        , enableSelectionWrap(true)
        , selectedIndex(0)
    {
        setFocusable(true);
        setSelectedIndex(0);
        addKeyListener(this);
    }

    Menu::Menu(const std::list<std::shared_ptr<MenuItem>> & items)
        : gcn::Container()
        , gcn::KeyListener()
        , items(items)
        , enableSelectionWrap(true)
        , selectedIndex(0)
    {
        setFocusable(true);
        setSelectedIndex(0);
        addKeyListener(this);
    }

    Menu::~Menu() {
    }

    std::list<std::shared_ptr<MenuItem>> & Menu::getItems() {
        return items;
    }

    int Menu::getSelectedIndex() const {
        return selectedIndex;
    }

    void Menu::setSelectedIndex(int index) {
        selectedIndex = index;

        // Validate selected index
        if(enableSelectionWrap) {
            std::size_t itemCount = getItems().size();

            if(itemCount == 0) {
                selectedIndex = 0;
            } else {
                if(getSelectedIndex() < 0) {
                    selectedIndex = itemCount - 1;
                } else if(static_cast<std::size_t>(getSelectedIndex()) >= itemCount) {
                    selectedIndex = 0;
                }
            }
        }

        int i = 0;

        std::for_each(
            std::begin(items),
            std::end(items),
            [&](std::shared_ptr<MenuItem> & item) {
                if(i == selectedIndex) {
                    item->select();
                } else {
                    item->deselect();
                }

                i++;
            }
        );
    }

    void Menu::addItem(const std::shared_ptr<MenuItem> & item) {
        if(item) {
            items.push_back(item);

            add(item.get());

            setSelectedIndex(getSelectedIndex());
        }
    }

    void Menu::removeItem(const std::shared_ptr<MenuItem> & item) {
        if(item) {
            items.remove(item);

            remove(item.get());

            setSelectedIndex(getSelectedIndex());
        }
    }

    void Menu::draw(gcn::Graphics* graphics) {
        graphics->setColor(getBackgroundColor());
        graphics->fillRectangle(0, 0, getWidth(), getHeight());
    }

    void Menu::keyPressed(gcn::KeyEvent& keyEvent) {
        gcn::Key key = keyEvent.getKey();

        if(key.getValue() == gcn::Key::Up) {
            setSelectedIndex(getSelectedIndex() - 1);
            keyEvent.consume();
        } else if(key.getValue() == gcn::Key::Down) {
            setSelectedIndex(getSelectedIndex() + 1);
            keyEvent.consume();
        }
    }

} // hikari::gui
} // hikari
