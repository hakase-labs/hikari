#include "hikari/core/game/Movable.hpp"
#include "hikari/core/game/CollisionResolver.hpp"
#include "hikari/core/geom/Point2D.hpp"
#include "hikari/core/util/Log.hpp"
#include "hikari/core/math/MathUtils.hpp"

namespace hikari {

    // Static
    float Movable::maxYVelocity = 7.0f;
    float Movable::maxXVelocity = 16.0f;
    float Movable::minYVelocity = -maxYVelocity;
    float Movable::minXVelocity = -maxXVelocity;

    std::shared_ptr<CollisionResolver> Movable::collisionResolver = nullptr;
    float Movable::gravity = 0.0f;
    int Movable::gravityApplicationCounter = 0;
    int Movable::gravityApplicationThreshold = 1;

    void Movable::setGravity(const float& gravity) {
        Movable::gravity = gravity;
    }

    const float Movable::getGravity() {
        return Movable::gravity;
    }
/*
    , onGroundNow(false)
    , onGroundLastFrame(false)
    , affectedByGravity(true)
    , collidesWithWorld(true)
    , treatLadderTopAsGround(true)
    , applyHorizontalVelocity(true)
    , applyVerticalVelocity(true)
    , velocity(0.0f, 0.0f)
    , boundingBox(0.0f, 0.0f, 0.0f, 0.0f)
    , collisionInfo()
    , landingCallback()
    , collisionCallback()
*/
    Movable::Movable()
        : onGroundNow(false)
        , onGroundLastFrame(false)
        , affectedByGravity(true)
        , collidesWithWorld(true)
        , treatLadderTopAsGround(true)
        , applyHorizontalVelocity(true)
        , applyVerticalVelocity(true)
        , velocity(0.0f, 0.0f)
        , boundingBox(0.0f, 0.0f, 0.0f, 0.0f)
        , collisionInfo()
        , landingCallback()
        , collisionCallback()
    {

    }

    Movable::Movable(float width, float height)
        : onGroundNow(false)
        , onGroundLastFrame(false)
        , affectedByGravity(true)
        , collidesWithWorld(true)
        , treatLadderTopAsGround(true)
        , applyHorizontalVelocity(true)
        , applyVerticalVelocity(true)
        , velocity(0.0f, 0.0f)
        , boundingBox(0.0f, 0.0f, width, height)
        , collisionInfo()
        , landingCallback()
        , collisionCallback()
    {

    }

    Movable::Movable(const Movable& proto) 
        : onGroundNow(proto.onGroundNow)
        , onGroundLastFrame(proto.onGroundLastFrame)
        , affectedByGravity(proto.affectedByGravity)
        , collidesWithWorld(proto.collidesWithWorld)
        , treatLadderTopAsGround(proto.treatLadderTopAsGround)
        , applyHorizontalVelocity(proto.applyHorizontalVelocity)
        , applyVerticalVelocity(proto.applyVerticalVelocity)
        , velocity(proto.velocity)
        , boundingBox(proto.boundingBox)
        , collisionInfo(proto.collisionInfo)
        , landingCallback(proto.landingCallback)        // TODO: Cloning the callbacks is almost 100% wrong
        , collisionCallback(proto.collisionCallback)    // You should create new ones instead
    {

    }

    Movable::~Movable() {

    }

    std::shared_ptr<CollisionResolver> Movable::getCollisionResolver() {
        return collisionResolver;
    }

    void Movable::setCollisionResolver(std::shared_ptr<CollisionResolver> resolver) {
        collisionResolver = resolver;
    }

    const bool& Movable::isOnGround() const {
        return isOnGroundNow();
    }

    const Vector2<float>& Movable::getPosition() const {
        return boundingBox.getPosition();
    }

    const Vector2<float>& Movable::getVelocity() const {
        return velocity;
    }

    const BoundingBoxF& Movable::getBoundingBox() const {
        return boundingBox;
    }

    const bool& Movable::isOnGroundNow() const {
        return onGroundNow;
    }

    const bool& Movable::wasOnGroundLastFrame() const {
        return onGroundLastFrame;
    }

    const bool& Movable::isGravitated() const {
        return affectedByGravity;
    }

    const bool& Movable::doesCollideWithWorld() const {
        return collidesWithWorld;
    }

    void Movable::setOnGround(const bool & bypassCallback = false) {
        onGroundNow = true;

        if(bypassCallback) {
            onGroundLastFrame = true;
        }
    }

    void Movable::setGravitated(const bool& isAffected) {
        affectedByGravity = isAffected;
    }

    void Movable::setHasWorldCollision(const bool& hasCollision) {
        collidesWithWorld = hasCollision;
    }

    void Movable::preCheckCollision() {
        onGroundLastFrame = onGroundNow;
        onGroundNow = false;
    }

    void Movable::checkCollision(const float& dt) {
        collisionInfo.clear();
        collisionInfo.treatLadderTopAsGround = this->treatLadderTopAsGround;

        preCheckCollision();

        // Check horizontal directions first
        if(velocity.getX() < 0) {
            // Moving left

            // We subtract 1 here because getBottom() represents the first pixel outside of the bounding box.
            getCollisionResolver()->checkHorizontalEdge(
                static_cast<int>(boundingBox.getLeft() + velocity.getX()/* * dt */),
                static_cast<int>(boundingBox.getTop()),
                static_cast<int>(boundingBox.getBottom() - 1),
                Directions::Left,
                collisionInfo
            );

            if(collisionInfo.isCollisionX) {
                boundingBox.setLeft(static_cast<float>(collisionInfo.correctedX));
                velocity.setX(0.0f);

                if(collisionCallback) {
                    collisionCallback(*this, collisionInfo);
                }
            }

        } else if(velocity.getX() > 0) {
            // Moving right

            // We subtract 1 here because getBottom() represents the first pixel outside of the bounding box.
            getCollisionResolver()->checkHorizontalEdge(
                static_cast<int>(boundingBox.getRight() + velocity.getX()/* * dt */),
                static_cast<int>(boundingBox.getTop()),
                static_cast<int>(boundingBox.getBottom() - 1),
                Directions::Right,
                collisionInfo
            );

            if(collisionInfo.isCollisionX) {
                boundingBox.setRight(static_cast<float>(collisionInfo.correctedX));
                velocity.setX(0.0f);

                if(collisionCallback) {
                    collisionCallback(*this, collisionInfo);
                }
            }
        }

        // Check vertical directions second
        if(velocity.getY() < 0) {
            // Moving up

            // We subtract 1 here because getRight() represents the first pixel outside of the bounding box.
            getCollisionResolver()->checkVerticalEdge(
                static_cast<int>(boundingBox.getTop() + velocity.getY()/* * dt */),
                static_cast<int>(boundingBox.getLeft()),
                static_cast<int>(boundingBox.getRight() - 1),
                Directions::Up,
                collisionInfo
            );

            if(collisionInfo.isCollisionY) {
                boundingBox.setTop(static_cast<float>(collisionInfo.correctedY));
                velocity.setY(0.0f);

                if(collisionCallback) {
                    collisionCallback(*this, collisionInfo);
                }
            }

        } else if(velocity.getY() > 0) {
            // Moving down

            // We subtract 1 here because getRight() represents the first pixel outside of the bounding box.
            getCollisionResolver()->checkVerticalEdge(
                static_cast<int>(std::ceil(boundingBox.getBottom() + velocity.getY()/* * dt */)),
                static_cast<int>(boundingBox.getLeft()),
                static_cast<int>(boundingBox.getRight() - 1),
                Directions::Down,
                collisionInfo
            );

            if(collisionInfo.isCollisionY) {
                boundingBox.setBottom(static_cast<float>(collisionInfo.correctedY));
                velocity.setY(velocity.getY() - std::floor(velocity.getY()));
                onGroundNow = true;

                if(collisionCallback) {
                    collisionCallback(*this, collisionInfo);
                }

                if(isGravitated() && !wasOnGroundLastFrame()) {
                    onLanding();
                }
            }
        }

        postCheckCollision();
    }

    void Movable::postCheckCollision() {

    }

    void Movable::onLanding() {
        if(landingCallback) {
            landingCallback(*this, collisionInfo);
        }
    }

    void Movable::setPosition(const Vector2<float>& position) {
        boundingBox.setPosition(position);
    }

    void Movable::setPosition(const float& x, const float& y) {
        boundingBox.setPosition(x, y);
    }

    void Movable::setBottom(float newBottom) {
        boundingBox.setBottom(newBottom);
    }

    void Movable::setVelocity(const Vector2<float>& velocity) {
        this->velocity = velocity;
    }

    void Movable::setVelocity(const float& x, const float& y) {
        this->velocity.setX(x).setY(y);
    }

    void Movable::setBoundingBox(const BoundingBoxF& boundingBox) {
        this->boundingBox = boundingBox;
    }

    void Movable::setTreatLadderTopAsGround(const bool& treatAsGround) {
        this->treatLadderTopAsGround = treatAsGround;
    }

    void Movable::setApplyHorizontalVelocity(const bool& applyVelocity) {
        this->applyHorizontalVelocity = applyVelocity;
    }

    void Movable::setApplyVerticalVelocity(const bool& applyVelocity) {
        this->applyVerticalVelocity = applyVelocity;
    }

    void Movable::move(const Vector2<float>& position) {
        setPosition(getPosition() + position);
    }

    void Movable::move(const float& x, const float& y) {
        const Vector2<float>& position = getPosition();
        setPosition(position.getX() + x, position.getY() + y);
    }

    void Movable::setLandingCallback(const CollisionCallback& callback) {
        this->landingCallback = callback;
    }

    void Movable::setCollisionCallback(const CollisionCallback& callback) {
        this->collisionCallback = callback;
    }

    void Movable::update(float dt) {
        if(isGravitated()) {
            velocity.setY(velocity.getY() + getGravity());
            velocity.setY(math::clamp(velocity.getY(), minYVelocity, maxYVelocity));
        }

        if(doesCollideWithWorld()) {
            checkCollision(dt);
        }

        // Integrate the position
        setPosition(
            getPosition().getX() + (applyHorizontalVelocity ? velocity.getX() : 0)/* * dt */, 
            getPosition().getY() + (applyVerticalVelocity   ? velocity.getY() : 0)/* * dt */);
    }

} // hikari