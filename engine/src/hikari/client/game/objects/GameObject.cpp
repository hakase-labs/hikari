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
        , active(false)
    {

    }

    GameObject::~GameObject() {

    }

    int GameObject::getId() const {
        return id;
    }

    bool GameObject::isActive() const {
        return active;
    }

    void GameObject::setActive(const bool &active) {
        if(this->active != active) {
            this->active = active;

            if(this->active) {
                onActivated();
            } else {
                onDeactivated();
            }
        }
    }

    void GameObject::onActivated() {

    }

    void GameObject::onDeactivated() {

    }

    void GameObject::update(const float &dt) {

    }

} // hikari
