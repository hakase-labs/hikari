#ifndef HIKARI_CLIENT_GAME_WEAPONACTION
#define HIKARI_CLIENT_GAME_WEAPONACTION

#include <memory>
#include <string>

namespace hikari {

  class GameWorld;
  class GameObject;
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
    virtual std::weak_ptr<GameObject> apply(GameWorld & world, const Weapon & weapon, WeaponFireEventData & eventData) const;
  };

} // hikari

#endif // HIKARI_CLIENT_GAME_WEAPONACTION