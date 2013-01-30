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
        //brain = [&](GameObject& go, const float &dt) {
        //    HIKARI_LOG(debug) << "GameObject::Brain::(); {id=" << getId() << ", dt=" << dt << "}";
        //};
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
        //HIKARI_LOG(debug) << "GameObject::update; {id=" << getId() << ", dt=" << dt << "}";
        //if(brain) {
        //    brain(*this, dt);
        //}
    }

} // hikari

//
// Script binding exports
//
// namespace OOLUA {
//    EXPORT_OOLUA_FUNCTIONS_NON_CONST(GameObject)
//    EXPORT_OOLUA_FUNCTIONS_CONST(GameObject, getId)
// }

/*
#include "hikari/client/game/objects/GameObject.hpp"
#include <SFML/Graphics.hpp>
#include <oolua.h>

namespace hikari {
    GameObject::GameObject(const int &id) 
        : id(id)
        , state(0)
        , health(0)
        , maxHealth(0)
        , age(0.0f)
        , active(false)
        , affectedByGravity(false)
        , position(0.0f, 0.0f)
        , spawnPosition(0.0f, 0.0f)
        , velocity(0.0f, 0.0f)
        , direction(Directions::LEFT)
        , boundingBox(position.getX(), position.getY(), 0.0f, 0.0f)
        , brain(DefaultBrain())
    {

    }

    GameObject::GameObject(const int &id, const GOCreationParameters &params)
        : id(id)
        , creationParams(params)
    {
        
    }

    void GameObject::DefaultBrain::operator() (GameObject &object, const float &dt) {
        // do nothing
        std::cout << "Default brain... age = " << object.getAge() << std::endl;
    }

    void GameObject::setState(const int &state) {
        this->state = state;
    }

    void GameObject::setHealth(const int &health) {
        this->health = health;
    }

    void GameObject::setMaxHealth(const int &maxHealth) {
        this->maxHealth = maxHealth;
    }

    void GameObject::setActive(const bool &active) {
        this->active = active;
    }

    void GameObject::setGravitated(const bool &affectedByGravity) {
        this->affectedByGravity = affectedByGravity;
    }

    void GameObject::setDirection(const Directions::Enum &direction) {
        this->direction = direction;
    }

    void GameObject::setBrain(const Brain &brain) {
        this->brain = brain;
    }

    int GameObject::getId() const {
        return id;
    }

    const int& GameObject::getState() const {
        return state;
    }

    const int& GameObject::getHealth() const {
        return health;
    }

    const int& GameObject::getMaxHealth() const {
        return maxHealth;
    }

    const float& GameObject::getAge() const {
        return age;
    }

    const bool& GameObject::isActive() const {
        return active;
    }

    const bool& GameObject::isGravitated() const {
        return affectedByGravity;
    }

    const Point2D<float>& GameObject::getPosition() const {
        return position;
    }

    const Point2D<float>& GameObject::getSpawnPosition() const {
        return spawnPosition;
    }

    const Point2D<float>& GameObject::getVelocity() const {
        return velocity;
    }

    const Rectangle2D<float>& GameObject::getBoundingBox() const {
        return boundingBox;
    }

    const Directions::Enum& GameObject::getDirection() const {
        return direction;
    }

    const GameObject::Brain& GameObject::getBrain() const {
        return brain;
    }

    void GameObject::update(const float &dt) {
        age += dt;
        brain(*this, dt);
    }

    void GameObject::render(sf::RenderTarget &target) {

    }
}

//
// Script binding exports
//
namespace OOLUA {
    EXPORT_OOLUA_FUNCTIONS_NON_CONST(GameObject)
    EXPORT_OOLUA_FUNCTIONS_CONST(GameObject, getId)
}
*/