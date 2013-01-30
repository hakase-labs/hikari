#include "hikari/client/game/objects/Spawner.hpp"

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

    const Vector2D& Spawner::getPosition() const {
        return position;
    }

    void Spawner::setPosition(const Vector2D& newPosition) {
        this->position = newPosition;
    }

} // hikari