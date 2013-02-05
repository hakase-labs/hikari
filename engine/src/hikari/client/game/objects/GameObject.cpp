#include "hikari/client/game/objects/GameObject.hpp"
#include "hikari/core/util/Log.hpp"
// #include <oolua.h>

namespace hikari {

    /* static */
    int GameObject::nextId = 1000;

    /* static */ 
    const int  GameObject::generateObjectId() {
        return nextId++;
    }

    GameObject::GameObject(const int& id)
        : id(id)
    {

    }

    GameObject::~GameObject() {

    }

    int GameObject::getId() const {
        return id;
    }

    const bool GameObject::isActive() const {
        return active;
    }

    void GameObject::setActive(const bool &active) {
        this->active = active;
    }

    void GameObject::update(const float &dt) {

    }

} // hikari
