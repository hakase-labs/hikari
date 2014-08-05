class GroundWalkingBehavior extends EnemyBehavior {
    direction = Directions.Left;
    speed = 0.5;
    checkDistance = 1;

    constructor(classConfig = {}) {
        base.constructor(classConfig);

        if("speed" in classConfig) {
            speed = classConfig.speed;
        }

        ::log("GroundWalkingBehavior constructor called.");
    }

    function update(dt) {
        if(host != null) {

            switch(direction) {
                case Directions.Left:
                    host.velocityX = -speed;
                break;

                case Directions.Right:
                    host.velocityX = speed;
                break;

                default:
                break;
            }

            host.direction = direction;

            // Reverse direction of we're going to walk off the edge.
            if(host.isOnEdge(checkDistance)) {
                if(direction == Directions.Left) {
                    direction = Directions.Right;
                } else if(direction == Directions.Right) {
                    direction = Directions.Left;
                }
            }
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

::log("GroundWalkingBehavior.nut executed!");
