class PeterchyEnemyBehavior extends EnemyBehavior {
    direction = Directions.Left;

    constructor(classConfig = {}) {
        base.constructor(classConfig);
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
