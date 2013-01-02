#include <hikari/client/game/objects/EnemyBrain.hpp>
#include <hikari/core/game/AnimationSet.hpp>
#include <hikari/core/math/RetroVector.hpp>
#include <hikari/core/util/Log.hpp>

namespace hikari {

    EnemyBrain::EnemyBrain() {
        
    }

    EnemyBrain::~EnemyBrain() {

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

    void EnemyBrain::update(const float& dt) {
        // Does nothing
    }

    void EnemyBrain::handleCollision(Movable& body, CollisionInfo& info) {
        // Does nothing
    }

} // hikari