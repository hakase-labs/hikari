enum State {
  IDLE,
  MOVING,
  SHOOTING
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

    function update(dt) {
        
    }

    function handleWorldCollision(side) {

    }
}

::log("EnemyBehavior.nut executed!");