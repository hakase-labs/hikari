#include "hikari/client/game/Weapon.hpp"
#include "hikari/client/game/WeaponAction.hpp"
#include "hikari/client/game/events/WeaponFireEventData.hpp"
#include "hikari/client/game/Shot.hpp"
#include "hikari/client/game/objects/GameObject.hpp"

#include "hikari/core/util/Log.hpp"

#include <algorithm>

namespace hikari {

  const int Weapon::DEFAULT_PALETTE_ID = -1;
  const float Weapon::DEFAULT_USAGE_COST = 0.0f;
  const char * Weapon::DEFAULT_PROJECTILE_TYPE = "None";
  const char * Weapon::DEFAULT_USAGE_SOUND = "None";

  Weapon::Weapon(const std::string & name, unsigned int limit, int damageId, int paletteId, float usageCost)
    : usageCost(usageCost)
    , limit(limit)
    , damageId(damageId)
    , paletteId(paletteId)
    , name(name)
    , label("WEAPON")
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

  int Weapon::getPaletteId() const {
    return paletteId;
  }

  const std::string & Weapon::getName() const {
    return name;
  }

  const std::string & Weapon::getLabel() const {
    return label;
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

  void Weapon::setLabel(const std::string & label) {
    this->label = label;
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
