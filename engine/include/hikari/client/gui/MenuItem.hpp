#ifndef HIKARI_CLIENT_GUI_MENUITEM
#define HIKARI_CLIENT_GUI_MENUITEM

#include <string>
#include <guichan/widget.hpp>

namespace hikari {
namespace gui {

    class MenuItem : public gcn::Widget {
    private:
        std::string name;
        bool selected;

    protected:
        virtual void adjustSize();

    public:
        MenuItem();
        explicit MenuItem(const std::string & name);
        virtual ~MenuItem();

        const std::string & getName() const;
        void setName(const std::string & name);

        bool isSelected() const;
        void select();
        void deselect();

        // Inherited from Widget
        virtual void draw(gcn::Graphics* graphics);
        gcn::Rectangle getChildrenArea();
    };

} // hikari::gui
} // hikari

#endif // HIKARI_CLIENT_GUI_MENUITEM