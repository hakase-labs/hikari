#ifndef HIKARI_CLIENT_GUI_MENU
#define HIKARI_CLIENT_GUI_MENU

#include <list>
#include <memory>
#include <string>
#include <guichan/widgets/container.hpp>
#include <guichan/keylistener.hpp>

namespace gcn {
    class Image;
    class KeyEvent;
}

namespace hikari {
namespace gui {

    class MenuItem;

    class Menu : public gcn::Container, public gcn::KeyListener {
    private:
        std::list<std::shared_ptr<MenuItem>> items;

        bool enableSelectionWrap;
        int selectedIndex;

    protected:
        std::list<std::shared_ptr<MenuItem>> & getItems();

    public:
        Menu();
        explicit Menu(const std::list<std::shared_ptr<MenuItem>> & items);
        virtual ~Menu();

        int getSelectedIndex() const;
        void setSelectedIndex(int index);

        void addItem(const std::shared_ptr<MenuItem> & item);
        void removeItem(const std::shared_ptr<MenuItem> & item);

        // Inherited from Widget
        virtual void draw(gcn::Graphics* graphics);

        // Inherited from KeyListener
        virtual void keyPressed(gcn::KeyEvent& keyEvent);
    };

} // hikari::gui
} // hikari

#endif // HIKARI_CLIENT_GUI_MENU