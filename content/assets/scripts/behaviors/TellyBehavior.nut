/**
 * Implementation for the Telly enemy behavior. Slowly seeks the hero, only
 * moving on one axis at a time -- whichever axis has greater distance from the
 * enemy to the hero.
 *
 *      HHHHHHHHHHHHHHHHHHHHHHHH
 *    HH^^^^  ^^        ^^  ^^^^HH
 *  HH^^^^HH  ^^        ^^  HH^^^^HH
 *  HH^^^^HHHH^^        ^^HHHH^^^^HH
 *  HH^^^^^^  ^^        ^^  ^^^^^^HH
 *    HHHHHHHHHHHHHHHHHHHHHHHHHHHH
 *    HHHH      HHHHHHHH      HHHH
 *    HH    HHHH  HHHH  HHHH    HH
 *    HH    HHHH  HHHH  HHHH    HH
 *    HHHH      HHHHHHHH      HHHH
 *    HHHHHHHHHHHHHHHHHHHHHHHHHHHH
 *  HH^^^^^^  ^^        ^^  ^^^^^^HH
 *  HH^^^^HHHH^^        ^^HHHH^^^^HH
 *  HH^^^^HH  ^^        ^^  HH^^^^HH
 *    HH^^^^  ^^        ^^  ^^^^HH
 *      HHHHHHHHHHHHHHHHHHHHHHHH
 */
class TellyEnemyBehavior extends EnemyBehavior {
    timer = 0.0;
    direction = Directions.Up;

    constructor(classConfig = {}) {
        base.constructor(config);
        ::log("TellyEnemyBehavior constructor called.");
    }

    /**
     * Updates the Telly and occasionally causes the course to be adjusted.
     * 
     * @param  {Float} dt the detal time for this update
     */
    function update(dt) {
        if(host != null) {
            timer += dt;

            if(timer >= ((1.0 / 60.0) * 5.0)) {
                timer = 0.0;

                adjustCourse();
            }
        }

        base.update(dt);
    }

    /**
     * Overridden so we can adjust course right from the start.
     */
    function attachHost(newHost, instanceConfig = {}) {
        base.attachHost(newHost);

        if(host != null) {
            adjustCourse();
        }
    }

    /**
     * Adjusts the direction and velocity in order to move toward the hero. The
     * axis that has the greatest distance from the hero is the one that will be
     * moved along.
     */
    function adjustCourse() {
        local hostX = host.getX();
        local hostY = host.getY();
        local targetX = ::heroX;
        local targetY = ::heroY;
        local distanceX = targetX - hostX;
        local distanceY = targetY - hostY;

        if(abs(distanceX) > abs(distanceY)) {
            if(distanceX > 0.0) {
                host.direction = Directions.Right;
                host.velocityX = 0.2;
            } else {
                host.direction = Directions.Left;
                host.velocityX = -0.2;
            }
            host.velocityY = 0.0;
        } else {
            if(distanceY > 0.0) {
                host.velocityY = 0.2;
            } else {
                host.velocityY = -0.2;
            }
            host.velocityX = 0.0;
        }
    }
}

::log("TellyBehavior.nut executed!");