#ifndef HIKARI_CLIENT_GUI_ENERGYGAUGE
#define HIKARI_CLIENT_GUI_ENERGYGAUGE

#include <memory>
#include <guichan/widget.hpp>
#include <guichan/rectangle.hpp>

#include "hikari/client/gui/Orientation.hpp"

namespace gcn {
    class Image;
}

namespace hikari {
namespace gui {

    class EnergyGauge : public gcn::Widget {
    private:
        static const unsigned int DEFAULT_WIDTH;
        static const unsigned int DEFAULT_HEIGHT;
        static const unsigned int DEFAULT_BASE_COLOR;
        static const unsigned int DEFAULT_FOREGROUND_COLOR;
        static const unsigned int DEFAULT_BACKGROUND_COLOR;
        static const float DEFAULT_VALUE;
        static const float DEFAULT_MAXIMUM_VALUE;

        float value;
        float maximumValue;

        Orientation::Type orientation;

        void initialize();

    public:
        explicit EnergyGauge(float maximumValue = DEFAULT_MAXIMUM_VALUE);
        virtual ~EnergyGauge();

        float getValue() const;
        float getMaximumValue() const;
        Orientation::Type getOrientation() const;

        void setValue(float value);
        void setMaximumValue(float maximumValue);
        void setOrientation(Orientation::Type orientation);

        //Inherited from Widget
        virtual void draw(gcn::Graphics* graphics);
    };

} // hikari::gui
} // hikari

#endif // HIKARI_CLIENT_GUI_ENERGYGAUGE