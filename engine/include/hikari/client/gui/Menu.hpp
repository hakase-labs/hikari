#ifndef HIKARI_CLIENT_GUI_MENU
#define HIKARI_CLIENT_GUI_MENU

#include <vector>
#include <memory>
#include <string>
#include <guichan/widgets/container.hpp>
#include <guichan/keylistener.hpp>

namespace gcn {
    class KeyEvent;
    class SelectionListener;
}

namespace hikari {

    class Input;

namespace gui {

    class MenuItem;
    class InputListener;

    class Menu : public gcn::Container, public gcn::KeyListener {
    private:
        std::vector<std::shared_ptr<MenuItem>> items;
        std::list<gcn::SelectionListener*> selectionListeners;

        bool enableSelectionWrap;
        bool keyPressIgnoreFlag;
        int selectedIndex;

    protected:
        std::vector<std::shared_ptr<MenuItem>> & getItems();
        int getNextIndex(int start);
        int getPreviousIndex(int start);

        void selectMenuItem(int itemIndex);

        /**
         * Distributes a value changed event to all selection listeners
         * of the menu.
         */
        void distributeSelectionChangedEvent();

    public:
        Menu();
        explicit Menu(const std::vector<std::shared_ptr<MenuItem>> & items);
        virtual ~Menu();

        int getSelectedIndex() const;
        void setSelectedIndex(int index);

        void selectNextItem();
        void selectPreviousItem();

        void addItem(const std::shared_ptr<MenuItem> & item);
        void removeItem(const std::shared_ptr<MenuItem> & item);
        int getItemCount() const;
        std::shared_ptr<MenuItem> getMenuItemAt(unsigned int index) const;

        bool isWrappingEnabled() const;
        void enableWrapping();
        void disableWrapping();

        void enableKeyPressIgnore();
        void disableKeyPressIgnore();

        /**
         * Adds a selection listener to the menu. When the selection
         * changes an event will be sent to all selection listeners of the
         * list box.
         *
         * If you delete your selection listener, be sure to also remove it
         * using removeSelectionListener().
         *
         * @param selectionListener The selection listener to add.
         */
        void addSelectionListener(gcn::SelectionListener* selectionListener);

        /**
         * Removes a selection listener from the menu.
         *
         * @param selectionListener The selection listener to remove.
         */
        void removeSelectionListener(gcn::SelectionListener* selectionListener);

        // Inherited from Widget
        virtual void draw(gcn::Graphics* graphics);
        virtual void logic();

        // Inherited from KeyListener
        virtual void keyPressed(gcn::KeyEvent& keyEvent);
    };

} // hikari::gui
} // hikari

#endif // HIKARI_CLIENT_GUI_MENU
