#include "hikari/client/game/Weapon.hpp"

namespace hikari {

  const float DEFAULT_USAGE_COST = 0.0f;
  const char * DEFAULT_PROJECTILE_TYPE = "None";
  const char * DEFAULT_USAGE_SOUND = "None";

  Weapon::Weapon(const std::string & name, float usageCost)
    : name(name)
    , usageCost(usageCost)
  {

  }

  Weapon::~Weapon() {
    // dtor
  }

  float Weapon::getUsageCost() const {
    return usageCost;
  }

  void Weapon::setName(const std::string & name) {
    this->name = name;
  }

  void Weapon::setProjectileType(const std::string & type) {
    this->projectileType = type;
  }

  void Weapon::setSound(const std::string & sound) {
    this->usageSound = sound;
  }
} // hikari




/// Things to consider:
/*
  * If a Weapon fires projectiles it needs to do so from a specific position, and that position is relative to the Weapon user.
  * For weapons like shields, they actually spawn around the centroid of the Weapon user.
  * For "universal" weapons, they don't really care about the position of the Weapon user.
  * What if multiple particles are spawned? How does that work?

  * Projectiles can habe different movement behaviors:
    ** Linear movement
    ** Sinusidal (sine-wave-like) movement
    ** Heat-seeking movement
    ** "Sticky" movement (think Leaf Shield, Skull Sheild, anything that sticks to its creator)
    ** Static movement (no movement at all)
    ** Something requiring a script, probably not common but possible
*/