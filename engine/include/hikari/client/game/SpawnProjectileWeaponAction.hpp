#ifndef HIKARI_CLIENT_GAME_SPAWNPROJECTILEWEAPONACTION
#define HIKARI_CLIENT_GAME_SPAWNPROJECTILEWEAPONACTION

#include "hikari/client/game/WeaponAction.hpp"

#include <memory>
#include <string>

namespace hikari {

  class Motion;

  /**
   * A WeaponAction is something that a Weapon uses to perform its action.
   */
  class SpawnProjectileWeaponAction : public WeaponAction {
  private:
    std::string projectileType;
    std::shared_ptr<Motion> motion;

  public:
    SpawnProjectileWeaponAction(const std::string & projectileType, const std::shared_ptr<Motion> & motion = std::shared_ptr<Motion>(nullptr));
    virtual ~SpawnProjectileWeaponAction();
    virtual void apply(GameWorld & world, const Weapon & weapon, WeaponFireEventData & eventData) const;
  };

} // hikari

#endif // HIKARI_CLIENT_GAME_SPAWNPROJECTILEWEAPONACTION