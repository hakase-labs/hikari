class HammerJoeBehavior extends EnemyBehavior {
    state = State.IDLE;
    timer = 0.0;
    enteringNewState = true;

    constructor(classConfig = {}) {
        base.constructor(classConfig);
    }

    /**
     * Performs host-attachment stuff.
     * @override
     */
    function attachHost(newHost, instanceConfig = {}) {
        base.attachHost(newHost);

        if(host != null) {
            host.weaponId = 6;
        }
    }

    function update(dt) {
        if(host != null) {

            timer += dt;
            facePlayer();

            switch(state) {
                case State.IDLE:
                    if(enteringNewState) {
                        facePlayer();
                        host.changeAnimation("idle");
                        host.isShielded = true;
                        enteringNewState = false;
                    }

                    if(timer >= (1.0/60.0) * 47) {
                        timer = 0.0;
                        enteringNewState = true;
                        state = State.PRESHOOTING;
                    }
                    break;
                case State.PRESHOOTING:
                    if(enteringNewState) {
                        host.changeAnimation("open-mask");
                        host.isShielded = false;
                        enteringNewState = false;
                    }

                    if(timer >= (1.0/60.0) * 24) {
                        timer = 0.0;
                        enteringNewState = true;
                        state = State.SHOOTING;
                    }
                    break;
                case State.SHOOTING:
                    if(enteringNewState) {
                        host.changeAnimation("shooting");
                        enteringNewState = false;
                        host.fireWeapon();
                    }

                    if(timer >= (1.0/60.0) * 32) {
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

    function handleWorldCollision(side) {

    }
}

::log("HammerJoeBehavior.nut executed!");
