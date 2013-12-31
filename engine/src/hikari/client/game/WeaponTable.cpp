#include "hikari/client/game/WeaponTable.hpp"
#include "hikari/client/game/Weapon.hpp"

#include <json/value.h>

#include <algorithm>

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
        if(weaponId >= 0 && static_cast<std::size_t>(weaponId) < weapons.size()) {
            return std::weak_ptr<Weapon>(weapons.at(weaponId));
        }

        // TODO: Add else conditions here for special/default weapons?

        return std::weak_ptr<Weapon>();
    }

    std::weak_ptr<Weapon> WeaponTable::getWeaponByName(const std::string & name) const {
        auto itr = std::find_if(std::begin(weapons), std::end(weapons), [&](const std::weak_ptr<Weapon> & weapon) {
            if(const auto & weaponPtr = weapon.lock()) {
                return weaponPtr->getName() == name;
            }

            return false;
        });

        if(itr != std::end(weapons)) {
            return *itr;
        }

        return std::weak_ptr<Weapon>();
    }

    int WeaponTable::getWeaponIdByName(const std::string & name) const {
        int index = -1;

        for(unsigned int i = 0; i < weapons.size(); ++i) {
            if(const auto & weaponPtr = weapons.at(i)) {
                if(weaponPtr->getName() == name) {
                    index = i;
                    break;
                }
            }
        }

        return index;
    }
} // hikari
