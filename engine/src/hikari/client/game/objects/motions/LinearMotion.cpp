#include "hikari/client/game/objects/motions/LinearMotion.hpp"

namespace hikari {

    LinearMotion::LinearMotion()
        : angle(0.0f)
        , speed(0.0f)
        , velocity()
    {
        
    }

    LinearMotion::LinearMotion(float angle, float speed)
        : angle(angle)
        , speed(speed)
        , velocity()
    {
        calculateVelocity();
    }
       

    LinearMotion::LinearMotion(const Vector2<float> & velocity)
        : angle(0.0f)
        , speed(0.0f)
        , velocity(velocity)
    {
        calculateAngle();
        calculateSpeed();
    }

    LinearMotion::LinearMotion(const LinearMotion & proto)
        : angle(proto.angle)
        , speed(proto.speed)
        , velocity(proto.velocity)
    {

    }

    LinearMotion::~LinearMotion() {

    }

    void LinearMotion::calculateAngle() {
        // TODO
    }

    void LinearMotion::calculateSpeed() {
        // TODO
    }

    void LinearMotion::calculateVelocity() {
        // TODO
    }

    Vector2<float> LinearMotion::calculate(float dt, const Vector2<float> & previousVelocity) {
        return velocity;
    }

    void LinearMotion::setAngle(float degrees) {
        this->angle = degrees;
        calculateVelocity();
    }

    void LinearMotion::setSpeed(float speed) {
        this->speed = speed;
        calculateVelocity();
    }

    void LinearMotion::setVelocity(const Vector2<float> & velocity) {
        this->velocity = velocity;
        calculateAngle();
        calculateSpeed();
    }

} // hikari