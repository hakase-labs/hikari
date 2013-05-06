class TellyEnemyBehavior extends EnemyBehavior {
    timer = 0.0;
    direction = Directions.Up;

    constructor(config = {}) {
        base.constructor(config);
        ::log("TellyEnemyBehavior constructor called.");
    }

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