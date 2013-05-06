#include "hikari/client/game/events/WeaponFireEventData.hpp"

namespace hikari {

    const EventType WeaponFireEventData::Type = 0x13373da4;

    WeaponFireEventData::WeaponFireEventData(int weaponId, int shooterId)
        : BaseEventData(0.0f)
        , weaponId(weaponId)
        , shooterId(shooterId)
    {

    }

    int WeaponFireEventData::getWeaponId() const {
        return weaponId;
    }

    int WeaponFireEventData::getShooterId() const {
        return shooterId;
    }

    WeaponFireEventData::~WeaponFireEventData() {
        // Do nothing!
    }

    const EventType & WeaponFireEventData::getEventType() const {
        return WeaponFireEventData::Type;
    }

    EventDataPtr WeaponFireEventData::copy() const {
        return EventDataPtr(new WeaponFireEventData(getWeaponId(), getShooterId()));
    }

    const char * WeaponFireEventData::getName() const {
        return "WeaponFireEventData";
    }

} // hikari