#include "hikari/client/game/objects/EnemyBrain.hpp"
#include "hikari/core/game/AnimationSet.hpp"
#include "hikari/core/math/NESNumber.hpp"
#include "hikari/core/util/Log.hpp"

namespace hikari {

    EnemyBrain::EnemyBrain()
        :host(nullptr)
    {
        
    }

    EnemyBrain::EnemyBrain(const EnemyBrain & proto)
        : host(nullptr)
    {

    }

    EnemyBrain::~EnemyBrain() {

    }

    std::unique_ptr<EnemyBrain> EnemyBrain::clone() const {
        return std::unique_ptr<EnemyBrain>(new EnemyBrain(*this));
    }

    Enemy* const EnemyBrain::getHost() {
        return host;
    }

    void EnemyBrain::attach(Enemy* host) {
        this->host = host;
    }

    void EnemyBrain::detach() {
        host = nullptr;
    }

    void EnemyBrain::onActivated() {
        // Does nothing
    }

    void EnemyBrain::onDeactivated() {
        // Does nothing
    }

    void EnemyBrain::update(float dt) {
        // Does nothing
    }

    void EnemyBrain::handleCollision(Movable& body, CollisionInfo& info) {
        // Does nothing
    }

    void EnemyBrain::handleObjectTouch(int otherId) {
        // Does nothing
    }

    void EnemyBrain::applyConfig(const Sqrat::Table & instanceConfig) {
        // Does nothing
    }

} // hikari