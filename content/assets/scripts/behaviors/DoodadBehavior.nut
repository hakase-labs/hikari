class DoodadBehavior extends EnemyBehavior {
    didSetAnimation = false;
    counter = 0.0;

    constructor(classConfig = {}) {
        base.constructor(classConfig);
        ::log("DoodadBehavior constructor called.");
    }

    function update(dt) {
        if(host != null) {
            // Do some stuff. Or do nothing.
            if(!didSetAnimation) {
                host.changeAnimation("destructable-wall");
                host.isObstacle = true;
                host.isShielded = false;
                host.faction = Factions.World;
                //host.velocityY = -0.04;
                host.isPhasing = true;
                didSetAnimation = true;
                host.direction = Directions.Left;
            }

            counter += dt;

            if(counter >= 3.0) {
                local dir = host.direction;
                counter = 0.0;
                host.direction = Utils.getOppositeDirection(dir);
            }

            host.velocityY = host.direction == Directions.Left ? -0.6 : 0.6;
            host.velocityX = host.direction == Directions.Left ? -0.6 : 0.6;
        }

        base.update(dt);
    }

    function handleWorldCollision(side) {

    }

    function handleObjectTouch(otherId) {
        if(host) {
            if(otherId == ::heroId) {
                host.isGravitated = true;
            }
        }
    }
}

::log("DoodadBehavior.nut executed!");