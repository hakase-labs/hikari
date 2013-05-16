#ifndef HIKARI_CLIENT_GUI_ENERGYGAGUE
#define HIKARI_CLIENT_GUI_ENERGYGAGUE

#include <memory>
#include <guichan/widget.hpp>
#include <guichan/rectangle.hpp>

namespace gcn {
    class Image;
}

namespace hikari {

    class EnergyGague : public gcn::Widget {
    private:
        static const unsigned int DEFAULT_WIDTH;
        static const unsigned int DEFAULT_HEIGHT;
        static const float DEFAULT_VALUE;
        static const float DEFAULT_MAXIMUM_VALUE;

        float value;
        float maximumValue;

        gcn::Rectangle capacityOverlay;

    public:
        EnergyGague();
        explicit EnergyGague(float maximumValue);
        virtual ~EnergyGague();

        float getValue() const;
        float getMaximumValue() const;

        void setValue(float value);
        void setMaximumValue(float maximumValue);

        //Inherited from Widget
        virtual void draw(gcn::Graphics* graphics);
    };

} // hikari

#endif // HIKARI_CLIENT_GUI_ENERGYGAGUE