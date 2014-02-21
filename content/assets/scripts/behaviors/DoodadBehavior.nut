class DoodadBehavior extends EnemyBehavior {
    classConfig = {};

    constructor(_classConfig = {}) {
        base.constructor(_classConfig);
        classConfig = _classConfig;
        ::log("DoodadBehavior constructor called.");
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
            host.changeAnimation(("animation" in classConfig) ? classConfig.animation : "default");
            host.isObstacle = true;
            host.isShielded = false;
            host.isPhasing = false;
            host.faction = Factions.World;
            host.direction = Directions.Down;
            isInitialized = true;
        }
    }

    function handleWorldCollision(side) {

    }

    function handleObjectTouch(otherId) {

    }
}

::log("DoodadBehavior.nut executed!");