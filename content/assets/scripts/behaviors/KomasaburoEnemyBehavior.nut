class KomasaburoEnemyBehavior extends EnemyBehavior {
    state = State.IDLE;
    timer = 0.0;
    enteringNewState = false;

    constructor(config = {}) {
        base.constructor(config);
        ::log("KomasaburoEnemyBehavior constructor called.");
    }

    function update(dt) {
        if(host != null) {

            timer += dt;

            switch(state) {
                case State.IDLE:
                    if(enteringNewState) {
                        adjustDirection();
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
                        adjustDirection();
                        host.changeAnimation("shooting");
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

    function adjustDirection() {
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