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
     * @param  weapon [description]
     * @return        [description]
     */
    int addWeapon(const std::shared_ptr<Weapon> & weapon);

    /**
     * Returns a Weapon by looking it up using its ID. If no Weapon is found
     * for a given ID then a null pointer is returned.
     */
    std::weak_ptr<Weapon> getWeaponById(int weaponId) const;
  };

} // hikari

#endif // HIKARI_CLIENT_GAME_WEAPONTABLE