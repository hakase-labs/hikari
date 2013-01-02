#ifndef HIKARI_CLIENT_GAME_OBJECTS_MOVABLEOBJECT
#define HIKARI_CLIENT_GAME_OBJECTS_MOVABLEOBJECT

#include <hikari/client/game/objects/GameObject.hpp>
#include <hikari/core/geom/AABB.hpp>
#include <hikari/core/geom/Vector2D.hpp>
#include <hikari/core/geom/AABBCollisionInfo.hpp>
#include <memory>

namespace hikari {

    //class CollisionResolver;
    class Room;

    class MovableObject : public GameObject, public AABB {
    private:
        static float GRAVITY;
        bool onGroundNow;
        bool onGroundLastFrame;
        bool affectedByGravity;
        bool collidesWithWorld;

    protected:
        Vector2D position;
        Vector2D positionCorrection;
        Vector2D halfExtents;
        Vector2D velocity;
        
        geom::AABBCollisionInfo collisionInfo;
        //std::shared_ptr<CollisionResolver> collisionResolver;
        std::shared_ptr<Room> room;

        bool isOnGround();
        bool wasOnGroundLast();

        void setGravitated(const bool& isAffected);
        void setHasWorldCollision(const bool& hasCollision);

        virtual void onPreCollision();
        virtual void onCollision(const float& dt);
        virtual void onPostCollision();
        virtual void innerCollide(const AABB& tileAabb, const int& tileAttrs, const float& dt, const int& x, const int& y);
        virtual void collisionResponse(const Vector2D& normal, const float& distance, const float& dt);

        virtual void onLanding();

    public:
        MovableObject(const int& id, std::shared_ptr<Room> room);
        virtual ~MovableObject();
        const Vector2D& getCenter() const;
        const Vector2D& getHalfExtents() const;
        std::shared_ptr<Room> getRoom() const;

        void setCenter(const Vector2D& newCenter);
        void setHalfExtents(const Vector2D& newHalfExtents);
        void setRoom(std::shared_ptr<Room> newRoom);

        bool isGravitated() const;
        bool hasWorldCollision() const;

        virtual void update(const float& dt);
        virtual void reset();

        static void setGravity(const float& gravity);
        static const float getGravity();

        void setVelocityX(const float& newX);
        void setVelocityY(const float& newX);
    };

}

#endif // HIKARI_CLIENT_GAME_OBJECTS_MOVABLEOBJECT