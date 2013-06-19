#ifndef HIKARI_CLIENT_GAME_WEAPON
#define HIKARI_CLIENT_GAME_WEAPON

#include <memory>
#include <string>
#include <vector>

namespace hikari {

  class GameWorld;
  class WeaponAction;
  class WeaponFireEventData;

  /**
   * A Weapon describes the properties of a "usable" weapon.
   */
  class Weapon {
  private:
    float usageCost;            /// How much ammunition one "usage" of this weapon is required for it to fire
    std::string name;           /// The unique name of this weapon.
    std::string projectileType; /// The identifier of a particular projectile.
    std::string usageSound;     /// The name of the sample to play when the weapon is used.
    std::vector<std::shared_ptr<WeaponAction>> actions;

  protected:
    void setName(const std::string & name);
    void setProjectileType(const std::string & type);
    void setSound(const std::string & sound);

  public:
    static const float DEFAULT_USAGE_COST;
    static const char * DEFAULT_PROJECTILE_TYPE;
    static const char * DEFAULT_USAGE_SOUND;

    Weapon(const std::string & name, float usageCost = DEFAULT_USAGE_COST);

    virtual ~Weapon();

    float getUsageCost() const;
    const std::string & getName() const;
    const std::string & getProjectileType() const;
    const std::string & getUsageSound() const;

    void setActions(const std::vector<std::shared_ptr<WeaponAction>> & actions);
    virtual void fire(GameWorld & world, WeaponFireEventData & eventData) const;
  };

} // hikari

#endif // HIKARI_CLIENT_GAME_WEAPON