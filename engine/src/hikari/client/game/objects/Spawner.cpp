#include "hikari/client/game/objects/Spawner.hpp"

#include "hikari/core/util/Log.hpp"

namespace hikari {

    Spawner::Spawner()
        : GameObject(GameObject::generateObjectId())
        , direction(Directions::None)
        , position(0.0f, 0.0f)
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

    void Spawner::attachToInstance(const std::shared_ptr<GameObject> & instance) {

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