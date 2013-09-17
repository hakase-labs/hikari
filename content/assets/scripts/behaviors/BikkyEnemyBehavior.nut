(function(fileName) {
    class BikkyEnemyBehavior extends EnemyBehavior {
        state = State.IDLE;
        timer = 0.0;
        enteringNewState = false;

        constructor(classConfig = {}) {
            base.constructor(config);
            ::log("BikkyEnemyBehavior constructor called.");
        }

        function update(dt) {
            if(host != null) {

                timer += dt;

                switch(state) {
                    case State.IDLE:
                        if(enteringNewState) {
                            host.changeAnimation("idle");
                            host.velocityX = 0;
                            host.isShielded = true;
                            enteringNewState = false;
                            ::hikari.sound.playSample(32);
                        }

                        if(timer >= 1.0) {
                            timer = 0.0;
                            enteringNewState = true;
                            state = State.PREJUMPING;
                        }
                        break;

                    case State.PREJUMPING:
                        if(enteringNewState) {
                            enteringNewState = false;
                            timer = 0.0;
                            host.isShielded = false;
                        }

                        if(timer >= 0.1333) {
                            timer = 0.0;
                            enteringNewState = true;
                            state = State.JUMPING;
                        }
                        break;

                    case State.JUMPING:
                        if(enteringNewState) {
                            facePlayer();
                            host.changeAnimation("jumping");
                            host.velocityY = -5.0;
                            enteringNewState = false;
                        }

                        host.velocityX = host.direction == Directions.Left ? -1.3 : 1.3;

                        if(host.velocityY > 0) {
                            enteringNewState = true;
                            state = State.FALLING;
                        }
                        break;

                    case State.FALLING:
                        if(enteringNewState) {
                            enteringNewState = false;
                        }
                        break;
                        
                    default:
                        break;
                }
            }

            base.update(dt);
        }

        /**
         * Causes the enemy to turn toward the player.
         */
        function facePlayer() {
            if(host != null) {
                local hostX = host.getX();
                local targetX = ::heroX;

                // If I'm to the right of the hero, face left, otherwise face right
                if(hostX > targetX) {
                    host.direction = Directions.Left;
                } else {
                    host.direction = Directions.Right;
                }
            }
        }

        function handleWorldCollision(side) {
            if(state == State.FALLING) {
                if(side == Directions.Down) {
                    timer = 0.0;
                    state = State.IDLE;
                    enteringNewState = true;
                }
            }
        }
    }

    ::log(fileName + " executed!");
})("BikkyEnemyBehavior.nut");