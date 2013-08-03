#include "hikari/client/game/Weapon.hpp"
#include "hikari/client/game/WeaponAction.hpp"
#include "hikari/client/game/events/WeaponFireEventData.hpp"
#include "hikari/client/game/Shot.hpp"
#include "hikari/client/game/objects/GameObject.hpp"

#include "hikari/core/util/Log.hpp"

#include <algorithm>

namespace hikari {

  const float Weapon::DEFAULT_USAGE_COST = 0.0f;
  const char * Weapon::DEFAULT_PROJECTILE_TYPE = "None";
  const char * Weapon::DEFAULT_USAGE_SOUND = "None";

  Weapon::Weapon(const std::string & name, unsigned int limit, int damageId, float usageCost)
    : usageCost(usageCost)
    , limit(limit)
    , damageId(damageId)
    , name(name)
    , projectileType()
    , usageSound()
    , actions()
  {

  }

  Weapon::~Weapon() {
    // dtor
  }

  float Weapon::getUsageCost() const {
    return usageCost;
  }

  unsigned int Weapon::getLimit() const {
    return limit;
  }

  int Weapon::getDamageId() const {
    return damageId;
  }

  const std::string & Weapon::getName() const {
    return name;
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

  void Weapon::setActions(const std::vector<std::shared_ptr<WeaponAction>> & actions) {
    this->actions = actions;
  }
  
  Shot Weapon::fire(GameWorld & world, WeaponFireEventData & eventData) const {
    HIKARI_LOG(debug4) << "Weapon::fire executed. name=" << getName();
    
    std::list<std::weak_ptr<GameObject>> spawnedObjects;

    std::for_each(std::begin(actions), std::end(actions), [this, &world, &eventData, &spawnedObjects](const std::shared_ptr<WeaponAction> & action) {
      if(action) {
        spawnedObjects.push_back(action->apply(world, *this, eventData));
      }
    });

    return Shot(spawnedObjects);
  }
} // hikari
