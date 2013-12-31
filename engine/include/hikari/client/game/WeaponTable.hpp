#ifndef HIKARI_CLIENT_GAME_WEAPONTABLE
#define HIKARI_CLIENT_GAME_WEAPONTABLE

#include "hikari/core/util/Service.hpp"

#include <memory>
#include <vector>

namespace hikari {

  class Weapon;

  class WeaponTable : public Service {
  private:
    std::vector<std::shared_ptr<Weapon>> weapons;

  public:
    WeaponTable(/* const Json::Value &configuration */);
    virtual ~WeaponTable();

    /**
     * Adds a weapon to the table and returns the assigned weapon ID.
     *
     * @param  weapon the Weapon instance to add
     * @return        integer ID of the added Weapon
     */
    int addWeapon(const std::shared_ptr<Weapon> & weapon);

    /**
     * Returns a Weapon by looking it up using its ID. If no Weapon is found
     * for a given ID then a null pointer is returned.
     */
    std::weak_ptr<Weapon> getWeaponById(int weaponId) const;

    /**
     * Returns a Weapon by looking it up using its name. If no Weapon is found
     * for a given name then a null pointer is returned.
     */
    std::weak_ptr<Weapon> getWeaponByName(const std::string & name) const;

    /**
     * Returns the ID of a weapon by looking it up by name. If the weapon cannot
     * be found, then -1 is returned.
     */
    int getWeaponIdByName(const std::string & name) const;
  };

} // hikari

#endif // HIKARI_CLIENT_GAME_WEAPONTABLE