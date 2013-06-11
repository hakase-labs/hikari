class PeterchyEnemyBehavior extends EnemyBehavior {
    state = State.IDLE;
    timer = 0.0;
    enteringNewState = false;
    direction = Directions.Left;

    constructor(config = {}) {
        base.constructor(config);
        ::log("PeterchyEnemyBehavior constructor called.");
    }

    function update(dt) {
        if(host != null) {

            switch(direction) {
                case Directions.Left:
                    host.velocityX = -0.5;
                break;

                case Directions.Right:
                    host.velocityX = 0.5;
                break;

                default:
                break;
            }

            host.direction = direction;
            
            /*
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
            */
        }

        base.update(dt);
    }

    function handleWorldCollision(side) {
        if(side == Directions.Left) {
            direction = Directions.Right;
        } else if(side == Directions.Right) {
            direction = Directions.Left;
        }
    }
}

::log("PeterchyEnemyBehavior.nut executed!");