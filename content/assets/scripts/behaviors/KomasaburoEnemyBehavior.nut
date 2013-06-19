class KomasaburoEnemyBehavior extends EnemyBehavior {
    state = State.IDLE;
    timer = 0.0;
    enteringNewState = false;

    constructor(config = {}) {
        base.constructor(config);
        ::log("KomasaburoEnemyBehavior constructor called.");
    }

    /**
     * Performs host-attachment stuff.
     * @override
     */
    function attachHost(newHost, instanceConfig = {}) {
        base.attachHost(newHost);

        if(host != null) {
            host.weaponId = 0;
        }
    }

    function update(dt) {
        if(host != null) {

            timer += dt;

            switch(state) {
                case State.IDLE:
                    if(enteringNewState) {
                        facePlayer();
                        host.changeAnimation("idle");
                        enteringNewState = false;
                    }

                    if(timer >= 3.0) {
                        timer = 0.0;
                        enteringNewState = true;
                        state = State.SHOOTING;
                    }
                    break;
                case State.SHOOTING:
                    if(enteringNewState) {
                        facePlayer();
                        host.changeAnimation("shooting");
                        host.fireWeapon();
                        enteringNewState = false;
                    }

                    if(timer >= 0.5) {
                        timer = 0.0;
                        enteringNewState = true;
                        state = State.IDLE;
                    }
                    break;
                default:
                    break;
            }
        }

        base.update(dt);
    }

    /**
     * Causes the enemy to turn toward the player.
     */
    function facePlayer() {
        if(host != null) {
            local hostX = host.getX();
            local targetX = ::heroX;

            // If I'm to the right of the hero, face left, otherwise face right
            if(hostX > targetX) {
                host.direction = Directions.Left;
            } else {
                host.direction = Directions.Right;
            }
        }
    }

    function handleWorldCollision(side) {

    }
}

::log("KomasaburoEnemyBehavior.nut executed!");