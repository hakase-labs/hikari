#ifndef HIKARI_CLIENT_GAME_OBJECTS_ENTITY
#define HIKARI_CLIENT_GAME_OBJECTS_ENTITY

#include "hikari/client/game/objects/GameObject.hpp"
#include "hikari/client/game/objects/Faction.hpp"
#include "hikari/client/game/objects/EntityDeathType.hpp"
#include "hikari/core/game/Movable.hpp"
#include "hikari/core/game/Direction.hpp"
#include "hikari/core/math/Vector2.hpp"

#include <list>
#include <memory>

namespace sf {
    class RenderTarget;
} // sf

namespace hikari {

    class AnimationSet;
    class AnimatedSprite;
    class EventBus;
    class GameWorld; // Soon to replace reference to Room
    class Room;
    class Shot;

    /**
     * Base class for all in-game entities. 
     *  
     * An Entity has a logical and a rendered component. Most objects are
     * subclasses of Entity.
     */
    class Entity : public GameObject {
    private:
        static bool debug;
        static const float DEFAULT_AGE_IN_M_SECONDS;
        static const float DEFAULT_MAXIMUM_AGE_IN_M_SECONDS;

        std::unique_ptr<AnimatedSprite> animatedSprite;
        std::weak_ptr<EventBus> eventBus;
        std::weak_ptr<GameWorld> world;
        std::shared_ptr<Room> room;

        #ifdef HIKARI_DEBUG_ENTITIES
        sf::RectangleShape boxOutline;
        sf::RectangleShape boxPosition;
        #endif // HIKARI_DEBUG_ENTITIES

        Direction direction;
        Faction::Type faction;
        EntityDeathType::Type deathType;
        int weaponId;
        int damageId;

        bool obstacleFlag; // Does this object act like an obstacle?
        bool shieldFlag;   // Does this object deflect projectiles right now?
        bool agelessFlag;  // Does this object not experience aging?

        float age;
        float maximumAge;

        Vector2<float> actionSpot; // An offset from the position where actions "take place"

        std::list<Shot> activeShots;

        void move(const Vector2<float>& delta);
        void move(const float& dx, const float& dy);

    protected:
        Movable body;
        std::unique_ptr<AnimatedSprite> & getAnimatedSprite();
        //const std::shared_ptr<Room> & getRoom() const;

        virtual void renderEntity(sf::RenderTarget &target);

        /**
         * Removes any non-active shots that are currently being observed by the
         * Entity. This should be called by an Entity at least once every update.
         */
        void removeNonActiveShots();

        /**
         * Returns whether the Entity can fire its currently assigned weapon or 
         * not.
         * 
         * @return true if weapon can be fired, false otherwise
         */
        bool canFireWeapon() const;

        void setAge(float newAge);

    public:
        static void enableDebug(const bool &debug);

        Entity(int id, std::shared_ptr<Room> room);
        Entity(const Entity& proto);
        virtual ~Entity();

        void setAnimationSet(const std::shared_ptr<AnimationSet> & newAnimationSet);
        void changeAnimation(const std::string& animationName);
        
        void setDirection(const Direction& dir);
        const Direction getDirection() const;

        void setFaction(Faction::Type newFaction);
        Faction::Type getFaction() const;

        void setDeathType(EntityDeathType::Type newDeathType);
        EntityDeathType::Type getDeathType() const;

        void setWeaponId(int weaponId);
        int getWeaponId() const;
        void fireWeapon();

        void setDamageId(int damageId);
        int getDamageId() const;

        void setRoom(const std::shared_ptr<Room>& newRoom);
        const std::shared_ptr<Room>& getRoom() const;

        void setEventBus(const std::weak_ptr<EventBus> & eventBus);
        const std::weak_ptr<EventBus> & getEventBus() const;

        void setWorld(const std::weak_ptr<GameWorld>& worldRef);
        const std::weak_ptr<GameWorld>& getWorld() const;

        void setVelocityX(const float &vx);
        const float getVelocityX() const;

        void setVelocityY(const float &vy);
        const float getVelocityY() const;

        float getAge() const;
        float getMaximumAge() const;
        void setMaximumAge(float newMaximumAge);

        bool isAgeless() const;
        void setAgeless(bool isAgeless);

        /**
         * Causes the Entity to observe a Shot. Entities can observe many shots
         * at one time.
         * 
         * @param shot the Shot to observe
         */
        void observeShot(const Shot & shot);

        /**
         * Returns how many active shots the Entity is observing.
         * 
         * @return count onf active shots
         */
        unsigned int getActiveShotCount() const;

        /**
         * Sets whether this Entity should be affected by gravity or not. When
         * an Entity is affected by gravity, its Y position is adjusted over
         * time by a gravitational constant.
         * 
         * @param affected whether this Entity should be affected by gravity or not
         * @see Entity::isGravitated
         */
        void setGravitated(bool affected);

        /**
         * Gets whether the Entity should be affected by gravity or not.
         * @see Entity::setGravitated
         */
        bool isGravitated() const;

        /**
         * Sets whether this Entity acts like an obstacle. When an Entity is
         * an obstacle, it obstructs the movement of other Entities. It can 
         * also be stood on (like solid ground).
         * 
         * @param obstacle flag for whether this Entity should be an obstacle or not
         * @see Entity::isObstacle
         */
        void setObstacle(bool obstacle);

        /**
         * Gets the "obstacle status" of this Entity.
         * 
         * @return whether this Entity is an obstacle or not
         * @see Entity::setObstacle
         */
        bool isObstacle() const;

        /**
         * Sets whether this Entity deflects projectiles or not.
         *
         * @param shielded true to enable shielding, false to disable
         * @see Entity::isShielded
         */
        void setShielded(bool shielded);

        /**
         * Gets whether this Entity has its shields up or not.
         *
         * @return shield status
         * @see Entity::setShielded
         */
        bool isShielded() const;

        /**
         * Sets whether this Entity should phase through solid objects. If an
         * object is phasing it can freely pass through solid objects like
         * walls. It will not perform collision checks with the world.
         * 
         * @param phasing flag indicating whether this Entity should phase
         * @see Entity::isPhasing
         */
        void setPhasing(bool phasing);

        /**
         * Gets whether this Entity is currently phasing or not. Entities that
         * are phasing do not perform Object vs. World collision checks.
         * 
         * @return phasing status
         * @see Entity::setPhasing
         */
        bool isPhasing() const;

        /**
         * Gets the position of the Entity.
         *
         * @return Entity's position
         * @see setPosition
         */
        const Vector2<float>& getPosition() const;

        /**
         * Sets the Entity's position to newPosition.
         * 
         * @param newPosition a new position to give the Entity
         * @see getPosition
         */
        void setPosition(const Vector2<float>& newPosition);

        /**
         * Sets the Entity's position to (x, y).
         * 
         * @param x a new X coordinate to give the Entity
         * @param y a new Y coordinate to give the Entity
         * @see getPosition
         */
        void setPosition(const float x, const float y);

        /**
         * Gets the Entity's "action spot", which is a point relative to its
         * position where "actions" should take place. For example, when 
         * shooting a weapon the action spot would be used to calculate
         * where the projectiles should spawn from.
         *
         * @see setActionSpot
         * @see getPosition
         */
        const Vector2<float>& getActionSpot() const;

        /**
         * Sets the Entity's "action spot".
         * 
         * @param spot a point relative to the Entity's position to be used as
         *             the "action spot"
         */
        void setActionSpot(const Vector2<float> & spot);

        /**
         * Gets the Entity's bounding box.
         *
         * @return the BoundingBox of the Entity
         */
        const BoundingBoxF& getBoundingBox() const;

        /**
         * Sets the Entity's bounding box.
         * 
         * @param box the bounding box to use
         */
        void setBoundingBox(const BoundingBoxF& box);

        /**
         * Called when Entity is added to the game.
         */
        virtual void onBirth();

        /**
         * Called when Entity is removed from the game.
         */
        virtual void onDeath();

        /**
         * Called when Entity needs to "wake up". For example, it enters the screen area.
         */
        virtual void onWake();

        /**
         * Called when Entity needs to go to sleep. For example, it's no longer on screen.
         */
        virtual void onSleep();

        /**
         * A function that is called when this object's body collides with 
         * different kinds of tiles.
         *
         * This method it used to handle Object vs. World collisions -- tile
         * collision resolution. Different objects can handle this in different
         * ways so it can be overridden by subclasses for their own use.
         *
         * @param body the Movable involved in the collision
         * @param info information about the collision event
         */
        virtual void handleCollision(Movable& body, CollisionInfo& info);

        /**
         * Updates the Entity allowing it to make changes to its internal state.
         *
         * @param dt the amount of time that should elapse for the Entity
         */
        virtual void update(float dt);

        /**
         * Renders the entity to a specific RenderTarget. This draws the Entity
         * to the target and, optionally, debug information (hitboxes, etc.) if
         * those features are enabled.
         *
         * @param target a sf::RenderTarget to draw to
         */
        virtual void render(sf::RenderTarget &target);

        /**
         * A function that can be used to "reset" an Entity to its original
         * state.
         *
         * This should no longer be used and will be removed eventually.
         * 
         * @deprecated
         */
        virtual void reset();
    };

    /**
     * EntityHelpers contains static helper functions related to the Entity
     * class. These are mostly used by the scripting layer to manipulate
     * Entity instances without moving them between layers in the appication.
     */
    namespace EntityHelpers {

        /**
         * Gets the X position of a given Entity.
         *
         * @param  entity an Entity to get the position from
         * @return        the X position of the specified Entity
         */
        float getX(const Entity* entity);

        /**
         * Gets the Y position of a given Entity.
         *
         * @param  entity an Entity to get the position from
         * @return        the Y position of the specified Entity
         */
        float getY(const Entity* entity);

        /**
         * Sets the X position of a given Entity.
         * @param entity an Entity to set the position of
         * @param x      the new X position to set for the Entity
         */
        void setX(Entity * entity, float x);

        /**
         * Sets the Y position of a given Entity.
         * @param entity an Entity to set the position of
         * @param y      the new Y position to set for the Entity
         */
        void setY(Entity * entity, float y);

        /**
         * Performs a check to see if the tile at a specified position of the
         * room that an Entity is in has some attribute.
         *
         * In other words, it can be used to see if an entity is touching
         * spikes, water, solid walls, etc.
         * 
         * @param  entity    the Entity to use as a Room reference
         * @param  x         the x position (in pixels) of a tile
         * @param  y         the y position (in pixels) of a tile
         * @param  attribute the attribute to look for
         * @return           true if the attribute was found in the tile at the
         *                   specified position, false otherwise
         */
        bool checkIfTileAtPositionHasAttribute(Entity * entity, int x, int y, int attribute);

    } // hikari.EntityHelpers

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_ENTITY
