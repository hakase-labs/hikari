#ifndef HIKARI_CORE_GAME_MECHANICS
#define HIKARI_CORE_GAME_MECHANICS

#include "hikari/core/geom/BoundingBox.hpp"
#include "hikari/core/math/Vector2.hpp"
#include "hikari/core/math/NESNumber.hpp"
#include "hikari/core/game/CollisionInfo.hpp"
#include "hikari/core/game/Direction.hpp"
#include <functional>
#include <memory>

namespace hikari {

    class CollisionResolver;

    /**
     * Movable is a class that handles physical movement. It cane be used with
     * a CollisionChecker to handle collision detection and response with the
     * world.
     */
    class Movable {
    public:
        typedef std::function<void (Movable&, CollisionInfo&)> CollisionCallback;
    
    private:
        static float maxYVelocity;
        static float maxXVelocity;
        static float minYVelocity;
        static float minXVelocity;

        static float gravity;
        unsigned int gravityApplicationCounter;   // Used to count how many frames since last gavity application
        unsigned int gravityApplicationThreshold; // Used to determine when to apply gravity

        static std::shared_ptr<CollisionResolver> collisionResolver;

        bool onGroundNow;
        bool onGroundLastFrame;
        bool topBlockedFlag;
        bool rightBlockedFlag;
        bool bottomBlockedFlag;
        bool leftBlockedFlag;
        bool affectedByGravity;
        bool collidesWithWorld;
        bool treatPlatformAsGround;
        bool applyHorizontalVelocity;
        bool applyVerticalVelocity;

    protected:
        Vector2<float> velocity;
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
        Movable(const Movable& proto);
        virtual ~Movable();

        /**
         * Indicates whether the Movable is on the ground right now or not.
         *
         * @return true if on ground, false otherwise
         * @see Movable::setOnGround
         */
        bool isOnGround() const;

        bool isTopBlocked() const;
        bool isRightBlocked() const;
        bool isBottomBlocked() const;
        bool isLeftBlocked() const;

        /**
         * Gets the position of the Movable. The position may not be the same
         * as the top-left corner depending on the Movable's bounding box
         * origin.
         *
         * @return a vector containing the position
         * @see Movable::setPosition
         * @see Movable::getBoundingBox
         */
        const Vector2<float>& getPosition() const;
        const Vector2<float>& getVelocity() const;
        const BoundingBoxF& getBoundingBox() const;

        void setOnGround(const bool & bypassCallback);
        void setPosition(const Vector2<float>& position);
        void setPosition(const float& x, const float& y);
        void setBottom(float newBottom);
        void setVelocity(const Vector2<float>& velocity);
        void setVelocity(const float& x, const float& y);
        void setBoundingBox(const BoundingBoxF& boundingBox);

        void setTreatPlatformAsGround(const bool& treatAsGround); 
        void setApplyHorizontalVelocity(const bool& applyVelocity);
        void setApplyVerticalVelocity(const bool& applyVelocity);

        void move(const Vector2<float>& position);
        void move(const float& x, const float& y);

        const bool& isGravitated() const;
        void setGravitated(const bool& isAffected);
        const bool& doesCollideWithWorld() const;
        void setHasWorldCollision(const bool& hasCollision);

        void setLandingCallback(const CollisionCallback& callback);
        void setCollisionCallback(const CollisionCallback& callback);

        virtual void update(float dt);

        unsigned int getGravityApplicationThreshold() const;
        void setGravityApplicationThreshold(unsigned int threshold);

        static std::shared_ptr<CollisionResolver> getCollisionResolver();
        static void setCollisionResolver(std::shared_ptr<CollisionResolver> resolver);

        static void setGravity(const float& gravity);
        static const float getGravity();
    };

}

#endif // HIKARI_CORE_GAME_MECHANICS