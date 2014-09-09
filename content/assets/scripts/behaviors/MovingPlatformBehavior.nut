class MovingPlatformBehavior extends DoodadBehavior {
    isInitialized = false;
    counter = 0.0;
    shouldCount = false;
    switchDirectionDelay = (1.0/60.0) * 3000;

    constructor(_classConfig = {}) {
        base.constructor(_classConfig);
    }

    function update(dt) {
        if(host != null) {
            if(shouldCount) {
                counter += dt;

                if(counter >= switchDirectionDelay) {
                    counter = 0.0;
                    host.direction = Utils.getOppositeDirection(host.direction);
                }
            }

            if(host.direction == Directions.Left) {
                host.velocityX = 1.0;
            } else {
                host.velocityX = -1.0;
            }
        }

        base.update(dt);
    }

    /**
     * Attaches to a host object; sets up initial config.
     * @override
     */
    function attachHost(newHost, instanceConfig = {}) {
        base.attachHost(newHost, instanceConfig);

        if(host) {
            host.changeAnimation(("animation" in classConfig) ? classConfig.animation : "destructable-wall-vertical-green");
            host.isObstacle = true;
            host.isShielded = false;
            host.isPhasing = true;
            host.faction = Factions.World;
            host.direction = Directions.Right;
            host.isGravitated = false;
            isInitialized = true;
        }
    }
}

::log("FallingPlatformBehavior.nut executed!");
