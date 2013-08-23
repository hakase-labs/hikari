#include "hikari/client/gui/Menu.hpp"
#include "hikari/client/gui/MenuItem.hpp"
#include "hikari/client/game/Input.hpp"

#include <guichan/graphics.hpp>
#include <guichan/selectionlistener.hpp>

#include <algorithm>
#include <cmath>

namespace hikari {
namespace gui {

    Menu::Menu()
        : gcn::Container()
        , gcn::KeyListener()
        , items()
        , selectionListeners()
        , enableSelectionWrap(true)
        , selectedIndex(0)
    {
        setFocusable(true);
        setSelectedIndex(0);
        addKeyListener(this);
    }

    Menu::Menu(const std::vector<std::shared_ptr<MenuItem>> & items)
        : gcn::Container()
        , gcn::KeyListener()
        , items(items)
        , selectionListeners()
        , enableSelectionWrap(true)
        , selectedIndex(0)
    {
        setFocusable(true);
        setSelectedIndex(0);
        addKeyListener(this);
    }

    Menu::~Menu() {
    }

    void Menu::setInput(const std::weak_ptr<hikari::Input> & input) {
        this->input = input;
    }

    std::vector<std::shared_ptr<MenuItem>> & Menu::getItems() {
        return items;
    }

    int Menu::getSelectedIndex() const {
        return selectedIndex;
    }

    void Menu::setSelectedIndex(int index) {
        selectedIndex = index;

        // Prevent selecting beyond the length of menu items
        if(selectedIndex >= getItemCount()) {
            selectedIndex = getItemCount() - 1;
        }

        selectMenuItem(selectedIndex);
    }

    int Menu::getNextIndex(int startingPoint) {
        int nextIndex = startingPoint + 1;

        if(enableSelectionWrap) {
            std::size_t itemCount = getItemCount();

            if(itemCount == 0) {
                nextIndex = 0;
            } else {
                if(nextIndex < 0) {
                    nextIndex = itemCount - 1;
                } else if(nextIndex >= itemCount) {
                    nextIndex = 0;
                }
            }
        } else {
            nextIndex = std::min(nextIndex, static_cast<int>(getItemCount()) - 1);
            nextIndex = std::max(nextIndex, 0);
        }

        return nextIndex;
    }

    int Menu::getPreviousIndex(int startingPoint) {
        int previousIndex = startingPoint - 1;

        if(enableSelectionWrap) {
            std::size_t itemCount = getItemCount();

            if(itemCount == 0) {
                previousIndex = 0;
            } else {
                if(previousIndex < 0) {
                    previousIndex = itemCount - 1;
                } else if(previousIndex >= itemCount) {
                    previousIndex = 0;
                }
            }
        } else {
            previousIndex = std::min(previousIndex, static_cast<int>(getItemCount()) - 1);
            previousIndex = std::max(previousIndex, 0);
        }

        return previousIndex;
    }

    void Menu::selectMenuItem(int itemIndex) {
        int i = 0;

        std::for_each(
            std::begin(items),
            std::end(items),
            [&](std::shared_ptr<MenuItem> & item) {
                if(i == getSelectedIndex()) {
                    item->select();
                } else {
                    item->deselect();
                }

                i++;
            }
        );

        distributeSelectionChangedEvent();
    }

    void Menu::selectNextItem() {
        if(getItemCount() > 1) {
            int startingPoint = getSelectedIndex();
            int nextIndex = getNextIndex(startingPoint);

            while(nextIndex != startingPoint) {
                if(items.at(nextIndex)->isEnabled()) {
                    setSelectedIndex(nextIndex);
                    break;
                }

                nextIndex = getNextIndex(nextIndex);
            }
        }

        // Don't do anything if there is only 1 or 0 items
    }

    void Menu::selectPreviousItem() {
        if(getItemCount() > 1) {
            int startingPoint = getSelectedIndex();
            int previousIndex = getPreviousIndex(startingPoint);

            while(previousIndex != startingPoint) {
                if(items.at(previousIndex)->isEnabled()) {
                    setSelectedIndex(previousIndex);
                    break;
                }

                previousIndex = getPreviousIndex(previousIndex);
            }
        }

        // Don't do anything if there is only 1 or 0 items
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
            items.erase(
                std::remove(
                    std::begin(items),
                    std::end(items),
                    item
                ),
                std::end(items)
            );

            remove(item.get());
            setSelectedIndex(getSelectedIndex());
        }
    }

    int Menu::getItemCount() const {
        return items.size();
    }

    std::shared_ptr<MenuItem> Menu::getMenuItemAt(int index) const {
        if(index >= 0 && index < items.size()) {
            return items.at(index);
        }

        return std::shared_ptr<MenuItem>(nullptr);
    }

    bool Menu::isWrappingEnabled() const {
        return enableSelectionWrap;
    }

    void Menu::enableWrapping() {
        enableSelectionWrap = true;
    }

    void Menu::disableWrapping() {
        enableSelectionWrap = false;
    }

    void Menu::draw(gcn::Graphics* graphics) {
        graphics->setColor(getBackgroundColor());
        graphics->fillRectangle(0, 0, getWidth(), getHeight());
    }

    void Menu::logic() {
        processInputState();
        Widget::logic();
    }

    void Menu::keyPressed(gcn::KeyEvent& keyEvent) {
        // Only listen to keyboard events if there is no input assigned
        if(input.expired()) {
            gcn::Key key = keyEvent.getKey();

            if(key.getValue() == gcn::Key::Up) {
                selectPreviousItem();
                keyEvent.consume();
            } else if(key.getValue() == gcn::Key::Down) {
                selectNextItem();
                keyEvent.consume();
            } else if(key.getValue() == gcn::Key::Enter) {
                distributeActionEvent();
            }
        }
    }

    void Menu::addSelectionListener(gcn::SelectionListener* selectionListener) {
        selectionListeners.push_back(selectionListener);
    }

    void Menu::removeSelectionListener(gcn::SelectionListener* selectionListener) {
        selectionListeners.remove(selectionListener);
    }

    void Menu::distributeSelectionChangedEvent() {
        auto iter = std::begin(selectionListeners);
        auto end = std::end(selectionListeners);

        for(; iter != end; ++iter) {
            gcn::SelectionEvent event(this);
            (*iter)->valueChanged(event);
        }
    }

    void Menu::processInputState() {
        if(auto inputPtr = input.lock()) {
            if(inputPtr->wasPressed(Input::BUTTON_UP)) {
                selectPreviousItem();
            } else if(inputPtr->wasPressed(Input::BUTTON_DOWN)) {
                selectNextItem();
            }

            if(inputPtr->wasPressed(Input::BUTTON_SHOOT) || inputPtr->wasPressed(Input::BUTTON_START)) {
                distributeActionEvent();
            }
        }
    }

} // hikari::gui
} // hikari
