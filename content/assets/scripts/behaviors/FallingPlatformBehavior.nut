class FallingPlatformBehavior extends EnemyBehavior {
    counter = 0.0;
    shouldCount = false;
    fallDelay = (1.0/60.0) * 20;

    constructor(classConfig = {}) {
        base.constructor(classConfig);
        ::log("FallingPlatformBehavior constructor called.");
        ::log("classConfig.len() = " + classConfig.len());
        ::log("Is fallDelay in there? " + !!("fallDelay" in classConfig));

        if("fallDelay" in classConfig) {
            fallDelay = classConfig.fallDelay;
        }
    }

    function update(dt) {
        if(host != null) {
            if(shouldCount) {
                counter += dt;

                if(counter >= fallDelay) {
                    host.isGravitated = true;
                    shouldCount = false;
                }
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
            host.changeAnimation(("animation" in instanceConfig) ? instanceConfig.animation : "destructable-wall");
            host.isObstacle = true;
            host.isShielded = false;
            host.isPhasing = true;
            host.faction = Factions.World;
            host.direction = Directions.Down;
            isInitialized = true;
        }
    }

    function handleObjectTouch(otherId) {
        if(host) {
            if(otherId == ::heroId) {
                shouldCount = true;
                // host.isGravitated = true;
            }
        }
    }
}

::log("FallingPlatformBehavior.nut executed!");