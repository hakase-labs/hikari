class OctopusBatteryEnemyBehavior extends EnemyBehavior {
    state = State.IDLE;
    timer = 0.0;
    enteringNewState = false;
    direction = Directions.Up;

    constructor(classConfig = {}) {
        base.constructor(config);
        ::log("OctopusBatteryEnemyBehavior constructor called.");
    }

    function update(dt) {
        if(host != null) {
            //::log("My timer = " + timer);
            local dir = host.direction;
            switch(state) {
                case State.IDLE:
                    if(enteringNewState) {
                        host.changeAnimation("stopping");
                        host.velocityX = 0;
                        host.velocityY = 0;
                        enteringNewState = false;
                    }

                    timer += dt;

                    if(timer >= 2.0) {
                        ::log("I'm idle, but now I will move... " + timer + " heroX = " + heroX);
                        timer = 0.0;
                        enteringNewState = true;
                        state = State.MOVING;
                    }
                    break;
                case State.MOVING:
                    if(enteringNewState) {
                        host.changeAnimation("walking");
                        enteringNewState = false;

                        switch(dir) {
                            case Directions.Up:
                                host.velocityY = -2.0;
                                break;
                            case Directions.Right:
                                host.velocityX = 2.0;
                                break;
                            case Directions.Down:
                                host.velocityY = 2.0;
                                break;
                            case Directions.Left:
                                host.velocityX = -2.0;
                                break;
                            default:
                                break;
                        }
                    }
                    break;
                default:
                    break;
            }
        }

        base.update(dt);
    }

    function handleWorldCollision(side) {
        ::log("OctopusBatteryEnemyBehavior::handleWorldCollision");
        timer = 0.0;
        state = State.IDLE;
        enteringNewState = true;
        direction = Utils.getOppositeDirection(side);

        if(host != null) {
            host.direction = direction;
        }
    }
}

::log("OctopusBatteryEnemyBehavior.nut executed!");