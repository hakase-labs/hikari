#include "hikari/client/game/SpawnProjectileWeaponAction.hpp"
#include "hikari/client/game/GameWorld.hpp"
#include "hikari/client/game/Weapon.hpp"
#include "hikari/client/game/events/WeaponFireEventData.hpp"
#include "hikari/client/game/objects/Projectile.hpp"
#include "hikari/client/game/objects/Motion.hpp"
#include "hikari/core/util/Log.hpp"

#include <memory>

namespace hikari {

    SpawnProjectileWeaponAction::SpawnProjectileWeaponAction(const std::string & projectileType, const std::shared_ptr<Motion> & motion)
        : WeaponAction()
        , projectileType(projectileType)
        , motion(motion)
    {

    }

    SpawnProjectileWeaponAction::~SpawnProjectileWeaponAction() {
        // dtor
    }

    std::weak_ptr<GameObject> SpawnProjectileWeaponAction::apply(GameWorld & world, const Weapon & weapon, WeaponFireEventData & eventData) const {
        HIKARI_LOG(debug4) << "SpawnProjectileWeaponAction::apply executed.";

        auto projectile = world.spawnProjectile(projectileType);
        std::weak_ptr<GameObject> trackedProjectile;

        if(projectile) {
            projectile->reset();
            projectile->setDirection(eventData.getDirection());
            projectile->setFaction(eventData.getFaction());
            projectile->setPosition(eventData.getPosition());
            projectile->setParentId(eventData.getShooterId());
            projectile->setDamageId(weapon.getDamageId());
            projectile->setActive(true);

            if(motion) {
                projectile->setMotion(motion);
            }

            std::shared_ptr<Projectile> projectilePtr(std::move(projectile));

            world.queueObjectAddition(projectilePtr);
            trackedProjectile = std::weak_ptr<GameObject>(projectilePtr);
        }

        // Return an empty pointer
        return trackedProjectile;
    }
} // hikari
