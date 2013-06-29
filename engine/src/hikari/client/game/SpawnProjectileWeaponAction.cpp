#include "hikari/client/game/SpawnProjectileWeaponAction.hpp"
#include "hikari/client/game/GameWorld.hpp"
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

    void SpawnProjectileWeaponAction::apply(GameWorld & world, WeaponFireEventData & eventData) const {
        HIKARI_LOG(debug4) << "SpawnProjectileWeaponAction::apply executed.";

        auto projectile = world.spawnProjectile(projectileType);

        if(projectile) {
            projectile->reset();
            projectile->setDirection(eventData.getDirection());
            projectile->setFaction(eventData.getFaction());
            projectile->setPosition(eventData.getPosition());
            projectile->setActive(true);

            if(motion) {
                projectile->setMotion(motion);
            }

            world.queueObjectAddition(std::shared_ptr<Projectile>(std::move(projectile)));
        }
    }
} // hikari
