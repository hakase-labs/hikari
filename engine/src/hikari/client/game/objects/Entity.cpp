#include "hikari/client/game/objects/Entity.hpp"
#include "hikari/client/game/objects/AnimatedSprite.hpp"
#include "hikari/client/game/Shot.hpp"
#include "hikari/client/game/events/EventManager.hpp"
#include "hikari/client/game/events/WeaponFireEventData.hpp"
#include "hikari/core/game/map/Room.hpp"
#include "hikari/core/game/map/Tileset.hpp"
#include "hikari/core/game/SpriteAnimator.hpp"
#include "hikari/core/util/Log.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace hikari {

    bool Entity::debug = false;

    void Entity::enableDebug(const bool& debug) {
        #ifdef HIKARI_DEBUG_ENTITIES
        Entity::debug = debug;
        #endif // HIKARI_DEBUG_ENTITIES
    }

    Entity::Entity(int id, std::shared_ptr<Room> room)
        : GameObject(id)
        , animatedSprite(new AnimatedSprite())
        , eventManager()
        , world()
        , room(room)
        , direction(Directions::None)
        , faction(Faction::World)
        , deathType(EntityDeathType::Nothing)
        , weaponId(0)
        , damageId(0)
        , obstacleFlag(false)
        , shieldFlag(false)
        , actionSpot(0.0f, 0.0f)
        , activeShots()
        , body()
    {
        reset();

        body.setCollisionCallback(
            std::bind(&Entity::handleCollision, this, std::placeholders::_1, std::placeholders::_2));

        #ifdef HIKARI_DEBUG_ENTITIES
        boxOutline.setFillColor(sf::Color(128, 64, 0, 128));
        boxOutline.setOutlineColor(sf::Color(255, 255, 255, 128));
        boxOutline.setOutlineThickness(1.0f);

        boxPosition.setFillColor(sf::Color(255, 0, 0, 196));
        boxPosition.setOutlineColor(sf::Color(255, 255, 255, 196));
        boxPosition.setOutlineThickness(1.0f);
        #endif // HIKARI_DEBUG_ENTITIES
    }

    Entity::Entity(const Entity& proto)
        : GameObject(GameObject::generateObjectId())
        , animatedSprite(nullptr)
        , eventManager(proto.eventManager)
        , world(proto.world)
        , room(proto.room)
        , direction(proto.direction)
        , faction(proto.faction)
        , deathType(proto.deathType)
        , weaponId(proto.weaponId)
        , damageId(proto.damageId)
        , obstacleFlag(proto.obstacleFlag)
        , shieldFlag(proto.shieldFlag)
        , actionSpot(proto.actionSpot)
        , activeShots()
        , body(proto.body)
    {
        setGravitated(proto.isGravitated());
        setPhasing(proto.isPhasing());
        setPosition(proto.getPosition());

        body.setCollisionCallback(
            std::bind(&Entity::handleCollision, this, std::placeholders::_1, std::placeholders::_2));

        // Clone the animation information if present
        animatedSprite.reset(proto.animatedSprite ? new AnimatedSprite(*proto.animatedSprite.get()) : new AnimatedSprite());

        #ifdef HIKARI_DEBUG_ENTITIES
        boxOutline = proto.boxOutline;
        boxPosition = proto.boxPosition;
        #endif // HIKARI_DEBUG_ENTITIES
    }

    Entity::~Entity() {
    }

    std::unique_ptr<AnimatedSprite> & Entity::getAnimatedSprite() {
        return animatedSprite;
    }

    void Entity::changeAnimation(const std::string& animationName) {
        if(animatedSprite) {
            animatedSprite->setAnimation(animationName);
        }
    }

    void Entity::setAnimationSet(const std::shared_ptr<AnimationSet> & newAnimationSet) {
        if(newAnimationSet && animatedSprite) {
            animatedSprite->setAnimationSet(newAnimationSet);
        }
    }

    const Vector2<float>& Entity::getPosition() const {
        return getBoundingBox().getPosition();
    }

    void Entity::setPosition(const Vector2<float>& newPosition) {
        body.setPosition(newPosition);
    }

    void Entity::setPosition(const float x, const float y) {
        body.setPosition(x, y);
    }

    const Vector2<float>& Entity::getActionSpot() const {
        return actionSpot;
    }

    void Entity::setActionSpot(const Vector2<float> & spot) {
        actionSpot = spot;
    }

    const BoundingBoxF& Entity::getBoundingBox() const {
        return body.getBoundingBox();
    }

    void Entity::setBoundingBox(const BoundingBoxF& box) {
        body.setBoundingBox(box);
    }

    void Entity::setDirection(const Direction& dir) {
        this->direction = dir;

        // Flip sprite and sprite offset if facing left

        if(getDirection() == Directions::Left) {
            if(animatedSprite) {
                animatedSprite->setXFlipped(true);
            }
        } else {
            if(animatedSprite) {
                animatedSprite->setXFlipped(false);
            }
        }
    }

    void Entity::setFaction(Faction::Type newFaction) {
        faction = newFaction;
    }

    Faction::Type Entity::getFaction() const {
        return faction;
    }

    void Entity::setDeathType(EntityDeathType::Type newDeathType) {
        deathType = newDeathType;
    }

    EntityDeathType::Type Entity::getDeathType() const {
        return deathType;
    }

    void Entity::setWeaponId(int weaponId) {
        this->weaponId = weaponId;
    }

    int Entity::getWeaponId() const {
        return weaponId;
    }

    void Entity::fireWeapon() {
        if(canFireWeapon()) {
            if(auto events = eventManager.lock()) {
                
                // Adjust offset for direction
                Vector2<float> offset = getActionSpot();

                if(getDirection() == Directions::Left) {
                    offset.setX(-offset.getX());
                }

                events->triggerEvent(
                    std::make_shared<WeaponFireEventData>(
                        getWeaponId(),
                        getId(),
                        getFaction(),
                        getDirection(),
                        getPosition() + offset
                    )
                );
            } else {
                HIKARI_LOG(debug4) << "Entity::fireWeapon failed; no EventManager. id = " << getId();
            }
        }
    }

    void Entity::setDamageId(int damageId) {
        this->damageId = damageId;
    }

    int Entity::getDamageId() const {
        return damageId;
    }

    void Entity::setRoom(const std::shared_ptr<Room>& newRoom) {
        room = newRoom;
    }

    const std::shared_ptr<Room>& Entity::getRoom() const {
        return room;
    }

    void Entity::setWorld(const std::weak_ptr<GameWorld>& worldRef) {
        world = worldRef;
    }

    const std::weak_ptr<GameWorld>& Entity::getWorld() const {
        return world;
    }

    void Entity::setEventManager(const std::weak_ptr<EventManager>& eventManager) {
        this->eventManager = eventManager;
    }

    const std::weak_ptr<EventManager>& Entity::getEventManager() const {
        return eventManager;
    }

    const Direction Entity::getDirection() const {
        return this->direction;
    }

    void Entity::setVelocityX(const float &vx) {
        body.setVelocity(vx, getVelocityY());
    }

    const float Entity::getVelocityX() const {
        return body.getVelocity().getX();
    }

    void Entity::setVelocityY(const float &vy) {
        body.setVelocity(getVelocityX(), vy);
    }

    const float Entity::getVelocityY() const {
        return body.getVelocity().getY();
    }

    void Entity::observeShot(const Shot & shot) {
        activeShots.push_back(shot);
    }

    unsigned int Entity::getActiveShotCount() const {
        return activeShots.size();
    }

    void Entity::setGravitated(bool affected) {
        body.setGravitated(affected);
    }

    bool Entity::isGravitated() const {
        return body.isGravitated();
    }

    void Entity::setObstacle(bool isObstacle) {
        this->obstacleFlag = isObstacle;
    }

    bool Entity::isObstacle() const {
        return obstacleFlag;
    }

    void Entity::setShielded(bool shielded) {
        this->shieldFlag = shielded;
    }

    bool Entity::isShielded() const {
        return shieldFlag;
    }

    void Entity::setPhasing(bool phasing) {
        body.setHasWorldCollision(!phasing);
    }

    bool Entity::isPhasing() const {
        return !body.doesCollideWithWorld();
    }

    void Entity::onBirth() {

    }

    void Entity::onDeath() {
        reset();
    }

    void Entity::onWake() {

    }

    void Entity::onSleep() {
        reset();
    }

    void Entity::handleCollision(Movable& body, CollisionInfo& info) {

    }

    void Entity::update(float dt) {
        body.update(dt);

        if(animatedSprite) {
            animatedSprite->update(dt);
        }

        #ifdef HIKARI_DEBUG_ENTITIES
        if(debug) {
            const BoundingBoxF& bb = getBoundingBox();

            boxOutline.setPosition(std::floor(bb.getLeft() ), std::floor(bb.getTop()));
            boxOutline.setSize(sf::Vector2f(std::floor(bb.getWidth() ), std::floor(bb.getHeight())));

            boxPosition.setPosition(std::floor(getPosition().getX()), std::floor(getPosition().getY()));
            boxPosition.setSize(sf::Vector2f(1.0f, 1.0f));
        }
        #endif // HIKARI_DEBUG_ENTITIES

        removeNonActiveShots();
    }

    void Entity::render(sf::RenderTarget &target) {
        #ifdef HIKARI_DEBUG_ENTITIES
        // Draw bounding box behind sprite
        if(debug) {
            target.draw(boxOutline);
        }
        #endif // HIKARI_DEBUG_ENTITIES

        renderEntity(target);

        #ifdef HIKARI_DEBUG_ENTITIES        
        // Draw position in front of sprite
        if(debug) {
            target.draw(boxPosition);
        }
        #endif // HIKARI_DEBUG_ENTITIES
    }

    void Entity::renderEntity(sf::RenderTarget &target) {
        if(animatedSprite) {
            animatedSprite->setPosition(
                getPosition().toFloor()
            );
            animatedSprite->render(target);
        }
    }

    void Entity::removeNonActiveShots() {
        if(getActiveShotCount() > 0) {
            activeShots.erase(
                std::remove_if(
                    std::begin(activeShots),
                    std::end(activeShots),
                    [](const Shot & shot) {
                        return !shot.isActive();
                    }
                ),
                std::end(activeShots)
            );
        }
    }

    bool Entity::canFireWeapon() const {
        return getActiveShotCount() < 3; // TODO: Change this to weapon-specified threshold
    }

    void Entity::reset() {
        if(animatedSprite) {
            animatedSprite->rewind();
        }
    }

    namespace EntityHelpers {

        float getX(const Entity* entity) {
            return entity->getPosition().getX();
        }

        float getY(const Entity* entity) {
            return entity->getPosition().getY();
        }

        void setX(Entity * entity, float x) {
            entity->setPosition(x, entity->getPosition().getY());
        }

        void setY(Entity * entity, float y) {
            entity->setPosition(entity->getPosition().getX(), y);
        }

        bool checkIfTileAtPositionHasAttribute(Entity * entity, int x, int y, int attribute) {
            if(const auto & room = entity->getRoom()) {
                int gridSize = room->getGridSize();
                return TileAttribute::hasAttribute(
                    room->getAttributeAt(x / gridSize, y / gridSize), 
                    static_cast<TileAttribute::TileAttribute>(attribute)
                );
            }

            return false;
        }

    }

} // hikari