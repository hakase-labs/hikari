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
                host.isShielded = true;
                host.faction = Factions.World;
                ::log("host.faction = " + host.faction);

                didSetAnimation = true;
            }
        }

        base.update(dt);
    }

    function handleWorldCollision(side) {

    }
}

::log("DoodadBehavior.nut executed!");