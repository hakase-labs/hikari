#ifndef HIKARI_CORE_GAME_MAP_CAMERA
#define HIKARI_CORE_GAME_MAP_CAMERA

#include "hikari/core/Platform.hpp"
#include "hikari/core/geom/Rectangle2D.hpp"
#include "hikari/core/geom/Vector2D.hpp"
#include <SFML/Graphics/View.hpp>

namespace hikari {

    class HIKARI_API Camera {
    public:
        Camera(const Rectangle2D<float>& view, const Rectangle2D<int>& boundary = Rectangle2D<int>());

        const Rectangle2D<int>& getBoundary() const;
        const Rectangle2D<float>& getView() const;
        const sf::View getPixelAlignedView() const;

        void setBoundary(const Rectangle2D<int>& boundary);
        void setView(const Rectangle2D<float>& view);

        const float getX() const;
        const float getY() const;

        void setX(float x);
        void setY(float y);

        const bool canMoveHorizontal() const;
        const bool canMoveVertical() const;

        void lockHorizontal(bool lock);
        void lockVertical(bool lock);

        void lookAt(const Vector2D& location);
        void lookAt(float x, float y);

        void move(const Vector2D &offset);
        void move(float offsetX, float offsetY);

    private:
        Rectangle2D<int> boundary;
        Rectangle2D<float> view;
        //sf::View view;
        bool lockHorizontalMovement;
        bool lockVerticalMovement;

        void updateView();
    };

} // hikari

#endif // HIKARI_CORE_GAME_MAP_CAMERA
