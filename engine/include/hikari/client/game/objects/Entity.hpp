#ifndef HIKARI_CLIENT_GAME_OBJECTS_ENTITY
#define HIKARI_CLIENT_GAME_OBJECTS_ENTITY

#include "hikari/client/game/objects/GameObject.hpp"
#include "hikari/client/game/objects/Faction.hpp"
#include "hikari/core/game/Movable.hpp"
#include "hikari/core/game/Direction.hpp"
#include "hikari/core/math/Vector2.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <SFML/Graphics.hpp>

#include <memory>

namespace sf {
    class RenderTarget;
} // sf

namespace hikari {

    class Animation;
    class AnimationSet;
    class Animator;
    class EventManager;
    class GameWorld; // Soon to replace reference to Room
    class Room;

    /**
        Base class for all in-game entities. 
        
        An Entity has a logical and a rendered component. Most objects are
        subclasses of Entity.
    */
    class Entity : public GameObject {
    private:
        static bool debug;

        std::shared_ptr<sf::Texture> spriteTexture;
        sf::Sprite sprite;

        std::shared_ptr<AnimationSet> animationSet;
        std::shared_ptr<Animation> currentAnimation;
        std::shared_ptr<Animator> animationPlayer;

        std::weak_ptr<EventManager> eventManager;
        std::weak_ptr<GameWorld> world;

        #ifdef HIKARI_DEBUG_ENTITIES
        sf::RectangleShape boxOutline;
        sf::RectangleShape boxPosition;
        #endif // HIKARI_DEBUG_ENTITIES

        Direction direction;
        Faction::Type faction;
        int weaponId;
        int damageId;

        bool obstacleFlag; // Does this object act like an obstacle?
        bool shieldFlag;   // Does this object deflect projectiles right now?

        std::string currentAnimationName;

        void move(const Vector2<float>& delta);
        void move(const float& dx, const float& dy);

    protected:
        Movable body;
        std::shared_ptr<Room> room;
        sf::Sprite& getSprite();

        virtual void renderEntity(sf::RenderTarget &target);

    public:
        static void enableDebug(const bool &debug);

        Entity(int id, std::shared_ptr<Room> room);
        Entity(const Entity& proto);
        virtual ~Entity();

        std::shared_ptr<sf::Texture> getSpriteTexture();
        std::shared_ptr<AnimationSet> getAnimationSet() const;
        std::shared_ptr<Animation> getCurrentAnimation() const;
        std::shared_ptr<Animator> getAnimationPlayer() const;

        void setSpriteTexture(const std::shared_ptr<sf::Texture>& newTexture);
        void setSprite(const sf::Sprite& newSprite);
        void setAnimationSet(std::shared_ptr<AnimationSet> newAnimationSet);
        void setCurrentAnimation(std::shared_ptr<Animation> newAnimation);
        void changeAnimation(const std::string& animationName);
        void setAnimationPlayer(std::shared_ptr<Animator> newAnimationPlayer);

        void setDirection(const Direction& dir);
        const Direction getDirection() const;

        void setFaction(Faction::Type newFaction);
        Faction::Type getFaction() const;

        void setWeaponId(int weaponId);
        int getWeaponId() const;
        void fireWeapon();

        void setDamageId(int damageId);
        int getDamageId() const;

        void setRoom(const std::shared_ptr<Room>& newRoom);
        const std::shared_ptr<Room>& getRoom() const;

        void setEventManager(const std::weak_ptr<EventManager>& eventManager);
        const std::weak_ptr<EventManager>& getEventManager() const;

        void setWorld(const std::weak_ptr<GameWorld>& worldRef);
        const std::weak_ptr<GameWorld>& getWorld() const;

        void setVelocityX(const float &vx);
        const float getVelocityX() const;
        void setVelocityY(const float &vy);
        const float getVelocityY() const;

        /**
            Sets whether this Entity should be affected by gravity or not. When
            an Entity is affected by gravity, its Y position is adjusted over
            time by a gravitational constant.

            @param affected whether this Entity should be affected by gravity or not
            @see Entity::isGravitated
        */
        void setGravitated(bool affected);

        /**
            Gets whether the Entity should be affected by gravity or not.
            @see Entity::setGravitated
        */
        bool isGravitated() const;

        /**
            Sets whether this Entity acts like an obstacle. When an Entity is
            an obstacle, it obstructs the movement of other Entities. It can 
            also be stood on (like solid ground).

            @param obstacle flag for whether this Entity should be an obstacle or not
            @see Entity::isObstacle
        */
        void setObstacle(bool obstacle);

        /**
            Gets the "obstacle status" of this Entity.

            @return whether this Entity is an obstacle or not
            @see Entity::setObstacle
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
            Sets whether this Entity should hpase through solid objects. If an
            object is phasing it can freely pass through solid objects like
            walls. It will not perform collision checks with the world.

            @param phasing flag indicating whether this Entity should phase
            @see Entity::isPhasing
        */
        void setPhasing(bool phasing);

        /**
            Gets whether this Entity is currently phasing or not.

            @return phasing status
            @see Entity::setPhasing
        */
        bool isPhasing() const;

        const Vector2<float>& getPosition() const;
        void setPosition(const Vector2<float>& newPosition);
        void setPosition(const float x, const float y);

        const BoundingBoxF& getBoundingBox() const;
        void setBoundingBox(const BoundingBoxF& box);

        /**
            Called when Entity is added to the game.
        */
        virtual void onBirth();

        /**
            Called when Entity is removed from the game.
        */
        virtual void onDeath();

        /**
            Called when Entity needs to "wake up". For example, it enters the screen area.
        */
        virtual void onWake();

        /**
            Called when Entity needs to go to sleep. For example, it's no longer on screen.
        */
        virtual void onSleep();

        virtual void handleCollision(Movable& body, CollisionInfo& info);

        virtual void update(float dt);
        virtual void render(sf::RenderTarget &target);
        virtual void reset();
    };

    namespace EntityHelpers {

        float getX(const Entity* entity);
        float getY(const Entity* entity);
        void setX(Entity * entity, const float & x);
        void setY(Entity * entity, const float & y);

        bool checkIfTileAtPositionHasAttribute(Entity * entity, const int & x, const int & y, const int & attribute);

    } // hikari.EntityHelpers

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_ENTITY