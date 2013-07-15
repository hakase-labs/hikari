#include "hikari/client/game/objects/HeroDamagedMobilityState.hpp"
// #include "hikari/client/game/objects/HeroWalkingMobilityState.hpp"
#include "hikari/client/game/objects/HeroIdleMobilityState.hpp"
#include "hikari/client/game/events/EventManager.hpp"
#include "hikari/client/game/events/EntityDamageEventData.hpp"
#include "hikari/core/game/SpriteAnimator.hpp"
#include "hikari/core/game/Direction.hpp"
#include "hikari/core/math/NESNumber.hpp"
#include "hikari/core/util/Log.hpp"

namespace hikari {

    Hero::DamagedMobilityState::DamagedMobilityState(Hero & hero)
        : MobilityState(hero)
        , stunnedTimer(0.6667f)
        , horizontalVelocity()
    {

    }

    Hero::DamagedMobilityState::~DamagedMobilityState() {

    }

    void Hero::DamagedMobilityState::enter() {
        hero.isStunned = true;
        hero.invincibilityTimer = 1.4667f;
        hero.stopShooting();
        hero.chooseAnimation();

        // If we're facing left, move right, if right, move left.
        if(hero.getDirection() == Directions::Left) {
            horizontalVelocity.setX(NESNumber(0x00, 0x80).toFloat());
        } else {
            horizontalVelocity.setX(-(NESNumber(0x00, 0x80).toFloat()));
        }

        // Special case for sliding and climbing:
        // * If damaged mid-slide there is no knockback
        // * If damaged while climbing there is no knockback
        if(hero.isSliding || hero.isOnLadder) {
            horizontalVelocity.setX(0.0f);
        }

        // Ensure we get knocked back
        hero.body.setGravitated(true);
        hero.body.setApplyHorizontalVelocity(true);

        // Make sure we start falling immediately
        hero.setVelocityY(0.0f);

        // Make sure animations will play (like when you're on a ladder)
        hero.getAnimationPlayer()->unpause();

        if(auto events = hero.getEventManager().lock()) {
            events->triggerEvent(EventDataPtr(new EntityDamageEventData(hero.getId(), 0.0f)));
        }
    }

    void Hero::DamagedMobilityState::exit() {
        hero.isStunned = false;
        hero.isBlinking = true;
        hero.setVelocityX(0.0f);
    }

    Hero::MobilityState::StateChangeAction Hero::DamagedMobilityState::update(const float & dt) {
        stunnedTimer -= dt;

        if(stunnedTimer <= 0.0f) {
            // hero.requestMobilityStateChange(std::unique_ptr<MobilityState>(new IdleMobilityState(hero)));
            return MobilityState::NEXT;
        }

        hero.setVelocityX(horizontalVelocity.getX());

        return MobilityState::CONTINUE;
    }

} // hikari