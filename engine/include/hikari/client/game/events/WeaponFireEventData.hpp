#ifndef HIKARI_CLIENT_WEAPONFIREEVENTDATA
#define HIKARI_CLIENT_WEAPONFIREEVENTDATA

#include "hikari/client/game/events/BaseEventData.hpp"

namespace hikari {

    class Entity;

    class WeaponFireEventData : public BaseEventData {
    public:
        static const EventType Type;

    private:
        int weaponId;  // id of the weapon to be fired
        int shooterId; // id of the entity that shot this weapon
        
    public:
        WeaponFireEventData(int weaponId, int shooterId);
        virtual ~WeaponFireEventData();

        int getWeaponId() const;
        int getShooterId() const;

        virtual const EventType & getEventType() const;

        virtual EventDataPtr copy() const;
        virtual const char * getName() const;
    };

} // hikari

#endif // HIKARI_CLIENT_WEAPONFIREEVENTDATA