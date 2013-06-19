#include "hikari/client/game/WeaponTable.hpp"
#include "hikari/client/game/Weapon.hpp"
#include <json/value.h>

namespace hikari {

    WeaponTable::WeaponTable(/* const Json::Value &configuration */) {
        
    }

    WeaponTable::~WeaponTable() {
        
    }

    int WeaponTable::addWeapon(const std::shared_ptr<Weapon> & weapon) {
        weapons.push_back(weapon);
        return weapons.size() - 1;
    }

    std::weak_ptr<Weapon> WeaponTable::getWeaponById(int weaponId) const {
        if(weaponId >= 0 && weaponId < weapons.size()) {
            return std::weak_ptr<Weapon>(weapons.at(weaponId));
        }

        // TODO: Add else conditions here for special/default weapons?

        return std::weak_ptr<Weapon>();
    }
} // hikari