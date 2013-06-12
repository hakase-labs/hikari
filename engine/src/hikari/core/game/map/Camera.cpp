#include "hikari/core/game/map/Camera.hpp"
#include <cmath>

namespace hikari {

    Camera::Camera(const Rectangle2D<float>& view, const Rectangle2D<int>& boundary)
        : view(view)
        , boundary(boundary) 
        , lockHorizontalMovement(false)
        , lockVerticalMovement(false)
    {
        lookAt(0.0f, 0.0f);
    }

    const Rectangle2D<int>& Camera::getBoundary() const {
        return boundary;
    }

    const Rectangle2D<float>& Camera::getView() const {
        return view;
    }

    sf::View Camera::getPixelAlignedView() const {
        sf::Vector2f pixelAlignedCenter(view.getX() + (view.getWidth() / 2), view.getY() + (view.getHeight() / 2));
        pixelAlignedCenter.x = std::floor(pixelAlignedCenter.x);
        pixelAlignedCenter.y = std::floor(pixelAlignedCenter.y);

        sf::View pixelAlignedView(pixelAlignedCenter, sf::Vector2f(view.getWidth(), view.getHeight()));

        return pixelAlignedView;
    }

    void Camera::setBoundary(const Rectangle2D<int>& boundary) {
        this->boundary = boundary;
        updateView();
    }

    void Camera::setView(const Rectangle2D<float>& view) {
        this->view = view;
    }

    float Camera::getX() const {
        return view.getX();
    }

    float Camera::getY() const {
        return view.getY();
    }

    void Camera::setX(float x) {
        view.setX(x);
        updateView();
    }

    void Camera::setY(float y) {
        view.setY(y);
        updateView();
    }

    bool Camera::canMoveHorizontal() const {
        return !lockHorizontalMovement;
    }

    bool Camera::canMoveVertical() const {
        return !lockVerticalMovement;
    }

    void Camera::lockHorizontal(bool lock) {
        lockHorizontalMovement = lock;
        updateView();
    }

    void Camera::lockVertical(bool lock) {
        lockVerticalMovement = lock;
        updateView();
    }

    void Camera::lookAt(const Vector2<float>& location) {
        lookAt(location.getX(), location.getY());
    }

    void Camera::lookAt(float x, float y) {
        view.setX(x - (view.getWidth() / 2));
        view.setY(y - (view.getHeight() / 2));
        updateView();
    }

    void Camera::move(const Vector2<float> &offset) {
        move(offset.getX(), offset.getY());
    }

    void Camera::move(float offsetX, float offsetY) {
        view.setX(view.getX() + offsetX);
        view.setY(view.getY() + offsetY);
        updateView();
    }

    void Camera::updateView() {
        if(!canMoveVertical()) {
            // Set the view's top to the top of the boundary.
            if(view.getTop() < boundary.getTop()) {
                view.setY(static_cast<float>(boundary.getTop()));
            }

            // Set the "bottom" of the view to the bottom of the boundary.
            if(view.getBottom() > boundary.getBottom()) {
                view.setY(static_cast<float>(boundary.getBottom()) - view.getHeight());
            }
        }

        if(!canMoveHorizontal()) {
            // Set the view's left to the left of the boundary.
            if(view.getLeft() < boundary.getLeft()) {
                view.setX(static_cast<float>(boundary.getLeft()));
            }

            // Set the "right" of the view to the right of the boundary.
            if(view.getRight() > boundary.getRight()) {
                view.setX(static_cast<float>(boundary.getRight()) - view.getWidth());
            }
        }
    }
}