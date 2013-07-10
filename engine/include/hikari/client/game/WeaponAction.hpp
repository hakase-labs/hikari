#ifndef HIKARI_CLIENT_GAME_WEAPONACTION
#define HIKARI_CLIENT_GAME_WEAPONACTION

#include <string>

namespace hikari {

  class GameWorld;
  class Weapon;
  class WeaponFireEventData;

  /**
   * A WeaponAction is something that a Weapon uses to perform its action.
   */
  class WeaponAction {
  public:
    WeaponAction();
    virtual ~WeaponAction();

    /**
     * Applied the effects of the WeaponAction to a GameWorld.
     *
     * @param world the GameWorld instance where the actions take effect
     */
    virtual void apply(GameWorld & world, const Weapon & weapon, WeaponFireEventData & eventData) const;
  };

} // hikari

#endif // HIKARI_CLIENT_GAME_WEAPONACTION