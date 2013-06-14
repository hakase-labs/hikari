#include "hikari/client/game/events/WeaponFireEventData.hpp"

namespace hikari {

    const EventType WeaponFireEventData::Type = 0x13373da4;

    WeaponFireEventData::WeaponFireEventData(int weaponId, int shooterId, Faction::Type faction)
        : BaseEventData(0.0f)
        , weaponId(weaponId)
        , shooterId(shooterId)
        , faction(faction)
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

    WeaponFireEventData::~WeaponFireEventData() {
        // Do nothing!
    }

    const EventType & WeaponFireEventData::getEventType() const {
        return WeaponFireEventData::Type;
    }

    EventDataPtr WeaponFireEventData::copy() const {
        return EventDataPtr(new WeaponFireEventData(getWeaponId(), getShooterId(), getFaction()));
    }

    const char * WeaponFireEventData::getName() const {
        return "WeaponFireEventData";
    }

} // hikari