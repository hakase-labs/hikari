class AppearingBlockBehavior extends DoodadBehavior {
    previousObstacleStatus = false;

    constructor(_classConfig = {}) {
        base.constructor(_classConfig);
    }

    function update(dt) {
        base.update(dt);

        // if(host) {
        //     if(host.isObstacle != previousObstacleStatus) {
        //         ::log("State changed!");
        //         previousObstacleStatus = host.isObstacle;

        //         if(host.isObstacle) {
        //             ::hikari.sound.playSample("Disappearing Block");
        //         }
        //     }
        // }
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

    function handleObjectTouch(otherId) {

    }
}

::log("AppearingBlockBehavior.nut executed!");

