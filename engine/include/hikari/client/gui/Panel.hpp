#ifndef HIKARI_CLIENT_GUI_PANEL
#define HIKARI_CLIENT_GUI_PANEL

#include <memory>
#include <guichan/widgets/container.hpp>
#include <guichan/color.hpp>

namespace hikari {
namespace gui {

    class Panel : public gcn::Container {
    private:
        gcn::Color innerBorderColor;
        gcn::Color shadowColor;

        bool drawShadow;

        void drawInnerBorder(gcn::Graphics* graphics) const;

    public:
        Panel();
        virtual ~Panel();

        //Inherited from Widget
        virtual void draw(gcn::Graphics* graphics);
    };

} // hikari::gui
} // hikari

#endif // HIKARI_CLIENT_GUI_PANEL