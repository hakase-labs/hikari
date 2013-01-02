#include <hikari/client/game/objects/MovableObject.hpp>
#include <hikari/core/game/AABBCollision.hpp>
#include <hikari/core/game/map/Room.hpp>
#include <hikari/core/game/map/TileCollisionResolver.hpp>
#include <hikari/core/game/map/Tileset.hpp>
#include <hikari/core/util/Log.hpp>

namespace hikari {

    float MovableObject::GRAVITY = 0.0f;

    void MovableObject::setGravity(const float& gravity) {
        GRAVITY = gravity;
    }

    const float MovableObject::getGravity() {
        return GRAVITY;
    }

    MovableObject::MovableObject(const int& id, std::shared_ptr<Room> room) 
        : GameObject(id)
        , room(room)
        , onGroundNow(false)
        , onGroundLastFrame(false)
        , position()
        , positionCorrection()
        , halfExtents()
        , velocity(0.0f, 0.0f)
        , collisionInfo(Vector2D(), 0.0f, Vector2D())
        , affectedByGravity(true)
        , collidesWithWorld(true)
    {

    }

    MovableObject::~MovableObject() {

    }

    const Vector2D& MovableObject::getCenter() const {
        return position;
    }

    const Vector2D& MovableObject::getHalfExtents() const {
        return halfExtents;
    }

    std::shared_ptr<Room> MovableObject::getRoom() const {
        return room;
    }

    void MovableObject::setCenter(const Vector2D& newCenter) {
        position = newCenter;
    }

    void MovableObject::setHalfExtents(const Vector2D& newHalfExtents) {
        halfExtents = newHalfExtents;
    }

    void MovableObject::setRoom(std::shared_ptr<Room> newRoom) {
        room = newRoom;
    }

    bool MovableObject::hasWorldCollision() const {
        return collidesWithWorld;
    }
    
    bool MovableObject::isOnGround() {
        return onGroundNow;
    }

    bool MovableObject::wasOnGroundLast() {
        return onGroundLastFrame;
    }

    void MovableObject::setGravitated(const bool& isAffected) {
        affectedByGravity = isAffected;
    }

    void MovableObject::setHasWorldCollision(const bool& hasCollision) {
        collidesWithWorld = hasCollision;
    }

    bool MovableObject::isGravitated() const {
        return affectedByGravity;
    }

    void MovableObject::onPreCollision() {
        onGroundLastFrame = onGroundNow;
        onGroundNow = false;
    }

    void MovableObject::onCollision(const float& dt) {
        Vector2D predictedPosition = position + velocity * dt;
        
        // Find min/max moves
        Vector2D min = position.min(predictedPosition);
        Vector2D max = position.max(predictedPosition);

        // Extend moves by BB radius
        min -= getHalfExtents();
        max += getHalfExtents();

        // NOTE: May need to fudge the numbers slightly here.
        // Expand or contract them to get it right.

        onPreCollision();

        // Do actual collision detection here.
        if(room) {
            // collisionResolver->resolve(min, max, &innerCollide, dt);
            TileCollisionResolver::TileCollisionHandler handler =
                std::bind(&MovableObject::innerCollide, this, std::placeholders::_1, std::placeholders::_2, 
                    std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);

            TileCollisionResolver::checkTilesWithinAabb(*room, min, max, handler, dt);
        }

        onPostCollision();
    }

    void MovableObject::onPostCollision() {

    }

    void MovableObject::innerCollide(const AABB& tileAabb, const int& tileAttrs, const float& dt, const int& x, const int& y) {
        //HIKARI_LOG(info) << "Inner Collide; tile = " << tileAttrs << ", x = " << x << ", y = " << y;

        // Check if the tile is solid
        if(tileAttrs != Room::NO_TILE) {
            if((tileAttrs & TileAttribute::SOLID) == TileAttribute::SOLID) {
                
                bool collided = AABBCollision::aabbVsAabb(*this, tileAabb, collisionInfo, x, y, *room);
                
                if(collided) {
                    collisionResponse(collisionInfo.getNormal(), collisionInfo.getDistance(), dt);
                }
            }
        }
    }

    void MovableObject::collisionResponse(const Vector2D& normal, const float& distance, const float& dt) {
        //HIKARI_LOG(info) << "MovableObject::collisionResponse; normal = " << normal << ", distance = " << distance;

        float separation = std::max(distance, 0.0f);
        float penetration = std::min(distance, 0.0f);
        float relativeNormalVelocity = velocity.dot(normal) + separation / dt;

        positionCorrection -= (normal * (penetration / dt));

        //HIKARI_LOG(info) << "positionCorrection = " << positionCorrection;

        if(relativeNormalVelocity < 0) {
            velocity -= normal * relativeNormalVelocity;

            // On the ground?
            if(normal.getY() < 0) {
                onGroundNow = true;

                // Code for friction would go here
                // and here
                // and here
                // that's enough.

                if(!onGroundLastFrame) {
                    onLanding();
                }
            }
        }
    }

    void MovableObject::onLanding() {
        //HIKARI_LOG(info) << "MovableObject::onLanding()";
    }

    void MovableObject::update(const float& dt) {
        if(isGravitated()) {
            velocity.setY(velocity.getY() + MovableObject::getGravity());
            // May want to clamp velocity at some speed
            if(velocity.getY() > (7.0f * 60.0f)) {
                velocity.setY((7.0f * 60.0f) - 1.0f);
            }
        }

        if(hasWorldCollision()) {
            onCollision(dt);
        }

        position += (velocity + positionCorrection) * dt;

        positionCorrection.clear();
    }

    void MovableObject::reset() {
        // Nothing to do right now.
    }

    void MovableObject::setVelocityX(const float& newX) {
        velocity.setX(newX);
    }

    void MovableObject::setVelocityY(const float& newY) {
        velocity.setY(newY);
    }
}