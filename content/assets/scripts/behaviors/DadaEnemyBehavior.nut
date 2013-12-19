/**
 * Behavior for the Dada enemy. The Dada is a jackhammer-like enemy that jumps
 * up and down toward the player. Every third jump is a higher jump than the
 * previous two. The Dada bounces off of walls too and has googly eyes. Cool!
 */
class DadaEnemyBehavior extends EnemyBehavior {
    /**
     * The velocity to apply vertically when performing a "small" jump.
     * @type {Number}
     */
    static SMALL_JUMP_VELOCITY = -3.265625; // 3hi, 68lo
    /**
     * The velocity to apply vertically when performing a "large" jump.
     * @type {Number}
     */
    static LARGE_JUMP_VELOCITY = -7.9140625; // 7hi, 234lo
    /**
     * How fast to move horizontally when seeking the player.
     * @type {Number}
     */
    static MOVEMENT_VELOCITY_X = 1.5; // 1hi, 128lo

    static LARGE_JUMP_LIMIT = 3;
    //
    // Instance variables
    //

    /**
     * The count of jumps, which increases each time the Dada hits the ground.
     * @type {Number}
     */
    jumpCounter = 0;

    constructor(classConfig = {}) {
        base.constructor(classConfig);
        ::log("DadaEnemyBehavior constructor called.");
    }

    function update(dt) {
        moveInFacedDirection();
        base.update(dt);
    }

    function attachHost(newHost, instanceConfig = {}) {
        base.attachHost(newHost);

        if(host != null) {
            facePlayer();
        }
    }

    function moveInFacedDirection() {
        local dir = host.direction;
        
        if(Directions.Left == dir) {
            host.velocityX = -MOVEMENT_VELOCITY_X;
        } else {
            host.velocityX = MOVEMENT_VELOCITY_X;
        }
    }

    /**
     * Handles keeping track of the jump count, so that on the third jump the
     * Dada will jump higher.
     *
     * @param  {[type]} side [description]
     * @return {[type]}      [description]
     */
    function handleWorldCollision(side) {
        // Increment jump counts and move toward player
        if(side == Directions.Down) {
            facePlayer();

            jumpCounter++;
            
            if(jumpCounter >= LARGE_JUMP_LIMIT) {
                highJump();
                jumpCounter = 0;
            } else {
                lowJump();
            }
        }

        // Flip horizontal direction if we run into a wall
        if(Directions.Left == side) {
            host.direction = Directions.Right;
        } else if(Directions.Right == side) {
            host.direction = Directions.Left;
        }
    }

    /**
     * Causes the Dada to perform a high (taller) jump.
     */
    function highJump() {
        if(host) {
            host.velocityY = LARGE_JUMP_VELOCITY;
        }
    }

    /**
     * Causes the Dada to perform a low (smaller) jump.
     */
    function lowJump() {
        if(host) {
            host.velocityY = SMALL_JUMP_VELOCITY;
        }
    }
}