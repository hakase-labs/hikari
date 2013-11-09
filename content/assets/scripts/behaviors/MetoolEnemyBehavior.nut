/**
 * Behavior for the Metool enemy.
 */
class MetoolEnemyBehavior extends EnemyBehavior {

    //
    // Instance variables
    //
    faceTimer = 0.0;
    shootingTimer = 0.0;
    isShooting = false;

    constructor(classConfig = {}) {
        base.constructor(classConfig);
        ::log("MetoolEnemyBehavior constructor called.");
    }

    function update(dt) {
        // moveInFacedDirection();
        faceTimer += dt;
        shootingTimer += dt;


        // Face the player every 1 second
        if(faceTimer >= 1.0) {
            facePlayer();
            faceTimer = 0;
        }

        if(shootingTimer >= 3.0 && !isShooting) {
            shootingTimer = 0.0;
            isShooting = true;
            host.changeAnimation("shooting");
        }

        if(isShooting) {
            if(shootingTimer >= 0.5) {
                isShooting = false;
                shootingTimer = 0.0;
                host.changeAnimation("idle");
            }
        }

        base.update(dt);
    }

    function attachHost(newHost, instanceConfig = {}) {
        base.attachHost(newHost);

        if(host != null) {
            // facePlayer();
            guard();
        }
    }

    // function moveInFacedDirection() {
    //     local dir = host.direction;
        
    //     if(Directions.Left == dir) {
    //         host.velocityX = -MOVEMENT_VELOCITY_X;
    //     } else {
    //         host.velocityX = MOVEMENT_VELOCITY_X;
    //     }
    // }

    /**
     * @override
     */
    function handleWorldCollision(side) {
        if(side == Directions.Down) {
            
        }

        // Flip horizontal direction if we run into a wall
        if(Directions.Left == side) {
            host.direction = Directions.Right;
        } else if(Directions.Right == side) {
            host.direction = Directions.Left;
        }
    }

    function guard() {
        if(host) {
            host.isShielded = true;
            host.changeAnimation("guarding");
        }
    }
}