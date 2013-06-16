#include "hikari/client/game/events/WeaponFireEventData.hpp"
#include "hikari/core/util/HashedString.hpp"

namespace hikari {

    const EventType WeaponFireEventData::Type = HashedString("WeaponFireEventData").getHash();;

    WeaponFireEventData::WeaponFireEventData(int weaponId, int shooterId, Faction::Type faction, Direction direction)
        : BaseEventData(0.0f)
        , weaponId(weaponId)
        , shooterId(shooterId)
        , faction(faction)
        , direction(direction)
    {

    }

    int WeaponFireEventData::getWeaponId() const {
        return weaponId;
    }

    int WeaponFireEventData::getShooterId() const {
        return shooterId;
    }

    Faction::Type WeaponFireEventData::getFaction() const {
        return faction;
    }

    Direction WeaponFireEventData::getDirection() const {
        return direction;
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
                getDirection()
            )
        );
    }

    const char * WeaponFireEventData::getName() const {
        return "WeaponFireEventData";
    }

} // hikari