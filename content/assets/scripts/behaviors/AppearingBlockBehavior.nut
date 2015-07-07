class AppearingBlockBehavior extends DoodadBehavior {
    previousObstacleStatus = false;

    constructor(_classConfig = {}) {
        base.constructor(_classConfig);
    }

    function update(dt) {
        base.update(dt);
    }

    /**
     * Attaches to a host object; sets up initial config.
     * @override
     */
    function attachHost(newHost, instanceConfig = {}) {
        base.attachHost(newHost, instanceConfig);

        if(host) {
            host.changeAnimation(("animation" in classConfig) ? classConfig.animation : "appearing-block-blue");
            host.isObstacle = true;
            host.isShielded = false;
            host.isPhasing = true;
            host.faction = Factions.World;
            host.direction = Directions.Down;
            host.liveOffscreen = true;
        }
    }

    function onActivated() {
        // TODO: Restart animation so it looks like it's appearing.
    }
}

::log("AppearingBlockBehavior.nut executed!");

