#ifndef HIKARI_CLIENT_GAME_OBJECTS_COLLECTABLEITEM
#define HIKARI_CLIENT_GAME_OBJECTS_COLLECTABLEITEM

#include "hikari/client/game/objects/Entity.hpp"
#include "hikari/core/util/Cloneable.hpp"
#include <memory>
// #include <oolua.h>

namespace hikari {

    class AnimationSet;
    class Effect;
    class ItemSpawner;

    class CollectableItem : public Entity, public Cloneable<CollectableItem> {
    private:
        static const float DEFAULT_AGE_IN_M_SECONDS;
        static const float DEFAULT_MAXIMUM_AGE_IN_M_SECONDS;

        std::shared_ptr<Effect> effect;
        bool ageless;
        float age;
        float maximumAge;

    protected:
        void setAge(float newAge);

    public:
        CollectableItem(int id, std::shared_ptr<Room> room, std::shared_ptr<Effect> effect);
        CollectableItem(const CollectableItem &proto);
        virtual ~CollectableItem();

        virtual std::unique_ptr<CollectableItem> clone() const;

        float getAge() const;
        float getMaximumAge() const;
        void setMaximumAge(float newMaximumAge);

        bool isAgeless() const;
        void setAgeless(bool isAgeless);

        void setEffect(std::shared_ptr<Effect> newEffect);
        std::shared_ptr<Effect> getEffect() const;

        virtual void onBirth();
        virtual void onDeath();
        virtual void onWake();
        virtual void onSleep();

        virtual void update(float dt);
        virtual void render(sf::RenderTarget &target);
        virtual void reset();
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_COLLECTABLEITEM