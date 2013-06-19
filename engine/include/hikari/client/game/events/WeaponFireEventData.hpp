#ifndef HIKARI_CLIENT_WEAPONFIREEVENTDATA
#define HIKARI_CLIENT_WEAPONFIREEVENTDATA

#include "hikari/client/game/events/BaseEventData.hpp"
#include "hikari/client/game/objects/Faction.hpp"
#include "hikari/core/game/Direction.hpp"
#include "hikari/core/math/Vector2.hpp"

namespace hikari {

    class Entity;

    class WeaponFireEventData : public BaseEventData {
    public:
        static const EventType Type;

    private:
        int weaponId;  // id of the weapon to be fired
        int shooterId; // id of the entity that shot this weapon
        Faction::Type faction;
        Direction direction; //weaponId, ownerId, faction, position, direction
        Vector2<float> position; // position where this weapon was fired
    public:
        WeaponFireEventData(int weaponId, int shooterId, Faction::Type faction, Direction direction, const Vector2<float>& position);
        virtual ~WeaponFireEventData();

        int getWeaponId() const;
        int getShooterId() const;
        Faction::Type getFaction() const;
        Direction getDirection() const;
        const Vector2<float>& getPosition() const;

        virtual const EventType & getEventType() const;

        virtual EventDataPtr copy() const;
        virtual const char * getName() const;
    };

} // hikari

#endif // HIKARI_CLIENT_WEAPONFIREEVENTDATA