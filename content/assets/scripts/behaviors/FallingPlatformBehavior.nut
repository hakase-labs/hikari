class FallingPlatformBehavior extends DoodadBehavior {
    isInitialized = false;
    counter = 0.0;
    shouldCount = false;
    fallDelay = (1.0/60.0) * 20;

    constructor(_classConfig = {}) {
        base.constructor(_classConfig);

        if("fallDelay" in _classConfig) {
            fallDelay = _classConfig.fallDelay;
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
            host.changeAnimation(("animation" in classConfig) ? classConfig.animation : "destructable-wall-vertical-green");
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