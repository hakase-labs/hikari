#include "hikari/client/game/objects/Spawner.hpp"

#include "hikari/core/util/Log.hpp"

namespace hikari {

    Spawner::Spawner()
        : GameObject(GameObject::generateObjectId())
        , direction(Directions::None)
        , position(0.0f, 0.0f)
        , awake(false)
    {

    }

    const Direction& Spawner::getDirection() const {
        return direction;
    }

    void Spawner::setDirection(const Direction& direction) {
        this->direction = direction;
    }

    const Vector2<float>& Spawner::getPosition() const {
        return position;
    }

    void Spawner::setPosition(const Vector2<float>& newPosition) {
        this->position = newPosition;
    }

    void Spawner::performAction(GameWorld & world) {

    }

    void Spawner::attachEventListeners(EventBus & EventBus) {

    }

    void Spawner::detachEventListeners(EventBus & EventBus) {

    }

    void Spawner::attachToInstance(const std::shared_ptr<GameObject> & instance) {

    }

    bool Spawner::isAwake() const {
        return awake;
    }

    void Spawner::setAwake(bool awake) {
        if(this->awake != awake) {
            this->awake = awake;

            if(this->awake) {
                onWake();
            } else {
                onSleep();
            }
        }
    }

    bool Spawner::canSpawn() const {
        return !isAwake();
    }

    void Spawner::onWake() {
        HIKARI_LOG(debug3) << "Spawner::onWake(), id = " << getId();
    }

    void Spawner::onSleep() {
        HIKARI_LOG(debug3) << "Spawner::onSleep(), id = " << getId();
    }

    void Spawner::onActivated() {
        GameObject::onActivated();

        HIKARI_LOG(debug3) << "Spawner::onActivated(), id = " << getId();
    }

    void Spawner::onDeactivated() {
        GameObject::onDeactivated();

        HIKARI_LOG(debug3) << "Spawner::onDeactivated(), id = " << getId();
    }
} // hikari