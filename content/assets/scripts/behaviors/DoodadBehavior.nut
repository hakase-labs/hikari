class DoodadBehavior extends EnemyBehavior {
    didSetAnimation = false;

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
                didSetAnimation = true;
            }
        }

        base.update(dt);
    }

    function handleWorldCollision(side) {

    }
}

::log("DoodadBehavior.nut executed!");