#include "hikari/core/game/map/Door.hpp"
#include "hikari/core/math/Vector2.hpp"
#include "hikari/core/util/Log.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace hikari {

    const float Door::DOOR_SECTION_DELAY_SECONDS = (1.0f / 60.0f) * 4.0f; // 4 frames
    const int Door::DOOR_SECTION_COUNT = 4;                               // 4 sections in a door

    Door::Door(int x, int y, int width, int height)
        : openFlag(false)  // Doors always start closed
        , closedFlag(true) // Doors always start closed
        , timer(0.0f)
        , bounds(x, y, width, height)
        , animatedSprite()
        , doorState(DOOR_CLOSED)
    {
        animatedSprite.setPosition(Vector2<float>(x * 16.0f, y * 16.0f));
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

    bool Door::isOpen() const {
        return DOOR_OPEN == doorState;
    }

    bool Door::isClosed() const {
        return DOOR_CLOSED == doorState;
    }

    void Door::setAnimationSet(const std::shared_ptr<AnimationSet> & newAnimationSet) {
        animatedSprite.setAnimationSet(newAnimationSet);
        changeAnimation("idle");
    }

    void Door::changeAnimation(const std::string& animationName) {
        animatedSprite.setAnimation(animationName);
    }

    void Door::open() {
        HIKARI_LOG(debug4) << "Door opening!";
        changeAnimation("jumping");

        doorState = DOOR_OPENING;
    }

    void Door::close() {
        HIKARI_LOG(debug4) << "Door closing!";
        changeAnimation("idle");

        doorState = DOOR_CLOSING;
    }

    void Door::setOpen() {
        HIKARI_LOG(debug4) << "Door opened immediately!";
        doorState = DOOR_OPEN;
    }

    void Door::setClosed() {
        HIKARI_LOG(debug4) << "Door closed immediately!";
        doorState = DOOR_CLOSED;
    }

    void Door::update(float dt) {
        if(timer > 0.0f) {
            timer -= dt;
        } else {
            switch(doorState) {
                case DOOR_OPENING:
                    setOpen();
                    break;

                case DOOR_CLOSING:
                    setClosed();
                    break;

                default:
                    break;
            }
        }

        animatedSprite.update(dt);
    }

    void Door::render(sf::RenderTarget & target) const {
        animatedSprite.render(target);
    }

} // hikari
