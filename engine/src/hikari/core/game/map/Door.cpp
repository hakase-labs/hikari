#include "hikari/core/game/map/Door.hpp"
#include "hikari/core/math/Vector2.hpp"
#include "hikari/core/util/Log.hpp"

namespace hikari {

    Door::Door(int x, int y, int width, int height)
        : openFlag(false) // Doors always start closed
        , closedFlag(true) // Doors always start closed
        , bounds(x, y, width, height)
    {

    }

    Door::~Door() {

    }

    int Door::getX() const {
        return bounds.getPosition().getX();
    }

    int Door::getY() const {
        return bounds.getPosition().getY();
    }

    int Door::getWidth() const {
        return bounds.getWidth();
    }

    int Door::getHeight() const {
        return bounds.getHeight();
    }

    void Door::open() {
        HIKARI_LOG(debug4) << "Door opening!";
    }

    void Door::close() {
        HIKARI_LOG(debug4) << "Door closing!";
    }

    void Door::setOpen() {
        HIKARI_LOG(debug4) << "Door opened immediately!";
        closedFlag = false;
        openFlag = true;
    }

    void Door::setClosed() {
        HIKARI_LOG(debug4) << "Door closed immediately!";
        closedFlag = true;
        openFlag = false;
    }

    void Door::update(float dt) {

    }

} // hikari
