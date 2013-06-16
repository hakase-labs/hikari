enum State {
  IDLE,
  MOVING,
  SHOOTING,
  PREJUMPING,
  JUMPING,
  FALLING,
  LANDING
}

class EnemyBehavior {
    host = null;

    constructor(config = {}) {
        ::log("EnemyBehavior constructor called.");
        host = null;
    }

    function attachHost(newHost, instanceConfig = {}) {
        host = newHost;
    }

    function detachHost() {
        host = null;
    }

    function applyConfig(config = {}) {
        // To be implemented in subclasses
    }

    function update(dt) {
        
    }

    function handleWorldCollision(side) {

    }
}

::log("EnemyBehavior.nut executed!");