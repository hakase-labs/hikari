class SpringerBehavior extends GroundWalkingBehavior {

    state = State.IDLE;
    timer = 0.0;
    directionTimer = 0.0;
    enteringNewState = false;
    normalSpeed = 0.5;
    fastSpeed = 1.0;
    animationDuration = 0.3;
    attackDuration = 0.3 * 4.0;

    constructor(classConfig = {}) {
        base.constructor(classConfig);
        ::log("SpringerBehavior constructor called.");

        if("speed" in classConfig) {
            normalSpeed = classConfig.speed;
        }

        if("fastSpeed" in classConfig) {
            fastSpeed = classConfig.fastSpeed;
        }

        if("attackDuration" in classConfig) {
            attackDuration = classConfig.attackDuration;
        }

        state = State.MOVING;
        enteringNewState = true;
    }

    function update(dt) {
        if(host != null) {
            switch(state) {
                case State.IDLE:
                    if(enteringNewState) {
                        host.changeAnimation("attacking");
                        host.isShielded = true;
                        enteringNewState = false;
                        timer = 0.0;
                        directionTimer = 0.0;
                    }

                    if(timer >= attackDuration) {
                        timer = 0.0;
                        state = State.MOVING;
                        enteringNewState = true;
                    }

                    // This secondary timer it used to flip direction back and
                    // forth while attacking. This causes the Springer to pop up
                    // back and forth in both directions.
                    if(directionTimer >= animationDuration) {
                        directionTimer -= animationDuration;
                        direction = Utils.getOppositeDirection(direction);
                    }

                    speed = 0.0;
                    timer = timer + dt;
                    directionTimer = directionTimer + dt;
                    break;

                case State.MOVING:
                    if(enteringNewState) {
                        host.changeAnimation("idle");
                        host.isShielded = true;
                        enteringNewState = false;
                    }

                    speed = normalSpeed;

                    if(abs(host.getY() - ::heroY) < 5) {
                        speed = fastSpeed;
                    }

                    break;
            }
        }

        base.update(dt);
    }

    function attachHost(newHost, instanceConfig = {}) {
        base.attachHost(newHost);

        if(host != null) {
            host.changeAnimation("attacking");
        }
    }

    /**
     * @override
     */
    function handleObjectTouch(otherId) {
        if(otherId == ::heroId) {
            ::log("I'm touching the hero!");

            if(state != State.IDLE) {
                enteringNewState = true;
                state = State.IDLE;
                timer = 0.0;
            }
        }
    }
}

::log("SpringerBehavior.nut executed!");
