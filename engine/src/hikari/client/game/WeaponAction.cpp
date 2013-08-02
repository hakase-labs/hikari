#include "hikari/client/game/WeaponAction.hpp"
#include "hikari/client/game/GameWorld.hpp"
#include "hikari/client/game/Weapon.hpp"
#include "hikari/client/game/events/WeaponFireEventData.hpp"
#include "hikari/client/game/objects/GameObject.hpp"

#include "hikari/core/util/Log.hpp"

namespace hikari {

  WeaponAction::WeaponAction() {

  }

  WeaponAction::~WeaponAction() {
    // dtor
  }
  
  std::weak_ptr<GameObject> WeaponAction::apply(GameWorld & world, const Weapon & weapon, WeaponFireEventData & eventData) const {
    HIKARI_LOG(debug4) << "WeaponAction::apply executed.";

    return std::weak_ptr<GameObject>();
  }
} // hikari
