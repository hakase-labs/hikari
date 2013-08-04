#include "hikari/client/game/objects/motions/LinearMotion.hpp"

namespace hikari {

    LinearMotion::LinearMotion()
        : angle(0.0f)
        , speed(0.0f)
        , velocity()
        , applyXVelocity(true)
        , applyYVelocity(true)
    {
        
    }

    LinearMotion::LinearMotion(float angle, float speed)
        : angle(angle)
        , speed(speed)
        , velocity()
        , applyXVelocity(true)
        , applyYVelocity(true)
    {
        calculateVelocity();
    }
       

    LinearMotion::LinearMotion(const Vector2<float> & velocity)
        : angle(0.0f)
        , speed(0.0f)
        , velocity(velocity)
        , applyXVelocity(true)
        , applyYVelocity(true)
    {
        calculateAngle();
        calculateSpeed();
    }

    LinearMotion::LinearMotion(const LinearMotion & proto)
        : angle(proto.angle)
        , speed(proto.speed)
        , velocity(proto.velocity)
        , applyXVelocity(proto.applyXVelocity)
        , applyYVelocity(proto.applyYVelocity)
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
        Vector2<float> vel = velocity;

        if(!applyYVelocity) {
            vel.setY(previousVelocity.getY());
        }

        if(!applyXVelocity) {
            vel.setX(previousVelocity.getX());
        }

        return vel;
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

    void LinearMotion::setApplyHorizontalVelocity(bool applyVelocity) {
        applyXVelocity = applyVelocity;
    }

    void LinearMotion::setApplyVerticalVelocity(bool applyVelocity) {
        applyYVelocity = applyVelocity;
    }

} // hikari