#include "hikari/client/game/events/WeaponFireEventData.hpp"
#include "hikari/core/util/HashedString.hpp"

namespace hikari {

    const EventType WeaponFireEventData::Type = HashedString("WeaponFireEventData").getHash();;

    WeaponFireEventData::WeaponFireEventData(int weaponId, int shooterId, Faction faction, Direction direction, const Vector2<float> & position)
        : BaseEventData(0.0f)
        , weaponId(weaponId)
        , shooterId(shooterId)
        , faction(faction)
        , direction(direction)
        , position(position)
    {

    }

    int WeaponFireEventData::getWeaponId() const {
        return weaponId;
    }

    int WeaponFireEventData::getShooterId() const {
        return shooterId;
    }

    Faction WeaponFireEventData::getFaction() const {
        return faction;
    }

    Direction WeaponFireEventData::getDirection() const {
        return direction;
    }

    const Vector2<float>& WeaponFireEventData::getPosition() const {
        return position;
    }

    WeaponFireEventData::~WeaponFireEventData() {
        // Do nothing!
    }

    const EventType & WeaponFireEventData::getEventType() const {
        return WeaponFireEventData::Type;
    }

    EventDataPtr WeaponFireEventData::copy() const {
        return EventDataPtr(
            new WeaponFireEventData(
                getWeaponId(),
                getShooterId(),
                getFaction(),
                getDirection(),
                getPosition()
            )
        );
    }

    const char * WeaponFireEventData::getName() const {
        return "WeaponFireEventData";
    }

} // hikari