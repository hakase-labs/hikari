class MoleBehavior extends EnemyBehavior {

    slowSpeed = 0.3;
    fastSpeed = 2.0;

    constructor(classConfig = {}) {
        base.constructor(classConfig);
        ::log("MoleBehavior constructor called.");

        if("slowSpeed" in classConfig) {
            slowSpeed = classConfig.slowSpeed;
        }

        if("fastSpeed" in classConfig) {
            fastSpeed = classConfig.fastSpeed;
        }
    }

    function update(dt) {
        if(host != null) {
            local isTouchingSolidTile = host.checkIfTileAtPositionHasAttribute(
                host.getX(),
                host.getY(),
                1
            );

            if(isTouchingSolidTile) {
                host.velocityY = slowSpeed;
            } else {
                host.velocityY = fastSpeed;
            }

            if(host.direction == Directions.Up) {
                host.velocityY = -fabs(host.velocityY);
            }
        }

        base.update(dt);
    }

    function attachHost(newHost, instanceConfig = {}) {
        base.attachHost(newHost);

        if(host != null) {
            host.changeAnimation("idle");
            host.zIndex = -1;

            chooseDirection(instanceConfig);
        }
    }

    function applyConfig(config = {}) {
        chooseDirection(config);
    }

    function chooseDirection(config = {}) {
        if(host != null) {
            if("direction" in config) {
                local dir = config.direction;

                if(dir == "Up") {
                    host.direction = Directions.Up;
                    host.changeAnimation("drilling-up");
                } else {
                    host.direction = Directions.Down;
                    host.changeAnimation("drilling-down");
                }
            } else {
                host.direction = Directions.Down;
                host.changeAnimation("drilling-down");
            }
        }
    }
}

::log("MoleBehavior.nut executed!");
