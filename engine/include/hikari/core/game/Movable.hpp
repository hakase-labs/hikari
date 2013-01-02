#ifndef HIKARI_CORE_GAME_MECHANICS
#define HIKARI_CORE_GAME_MECHANICS

#include <hikari/core/geom/BoundingBox.hpp>
#include <hikari/core/math/Vector2.hpp>
#include <hikari/core/math/RetroVector.hpp>
#include <hikari/core/game/CollisionInfo.hpp>
#include <hikari/core/game/Direction.hpp>
#include <functional>
#include <memory>

namespace hikari {

    class CollisionResolver;
    class CollisionDetector;

    class Movable {
    public:
        typedef std::function<void (Movable&, CollisionInfo&)> CollisionCallback;
    
    private:
        static float gravity;
        static std::shared_ptr<CollisionResolver> collisionResolver;

        bool onGroundNow;
        bool onGroundLastFrame;
        bool affectedByGravity;
        bool collidesWithWorld;
        bool applyHorizontalVelocity;
        bool applyVerticalVelocity;

    protected:
        Vector2<float> velocity;
        Vector2<RetroVector> retroVelocity;
        BoundingBoxF boundingBox;
        CollisionInfo collisionInfo;

        CollisionCallback landingCallback;
        CollisionCallback collisionCallback;

        virtual void preCheckCollision();
        virtual void checkCollision(const float& dt);
        virtual void postCheckCollision();
        virtual void onLanding();

        //
        // Accessors
        //
        const bool& isOnGroundNow() const;
        const bool& wasOnGroundLastFrame() const;

    public:
        Movable();
        Movable(float width, float height);
        virtual ~Movable();

        const bool& isOnGround() const;
        const Vector2<float>& getPosition() const;
        const Vector2<float>& getVelocity() const;
        const BoundingBoxF& getBoundingBox() const;

        void setOnGround(const bool & bypassCallback);
        void setPosition(const Vector2<float>& position);
        void setPosition(const float& x, const float& y);
        void setVelocity(const Vector2<float>& velocity);
        void setVelocity(const float& x, const float& y);
        void setBoundingBox(const BoundingBoxF& boundingBox);

        void setApplyHorizontalVelocity(const bool& applyVelocity);
        void setApplyVerticalVelocity(const bool& applyVelocity);

        //std::unique_ptr<CollisionDetector> getCollisionDetector() const;
        //std::unique_ptr<CollisionResolver> getCollisionResolver() const;

        void move(const Vector2<float>& position);
        void move(const float& x, const float& y);

        const bool& isGravitated() const;
        void setGravitated(const bool& isAffected);
        const bool& doesCollideWithWorld() const;
        void setHasWorldCollision(const bool& hasCollision);

        void setLandingCallback(const CollisionCallback& callback);
        void setCollisionCallback(const CollisionCallback& callback);

        virtual void update(const float& dt);

        static std::shared_ptr<CollisionResolver> getCollisionResolver();
        static void setCollisionResolver(std::shared_ptr<CollisionResolver> resolver);

        static void setGravity(const float& gravity);
        static const float getGravity();
    };

}

#endif // HIKARI_CORE_GAME_MECHANICS