(function() {
    local STATE_GUARDING = 0;
    local STATE_ATTACKING = 1;
    local STATE_RUNNING = 2;

    /**
     * Behavior for the Metool enemy.
     */
    class MetoolEnemyBehavior extends EnemyBehavior {
        /**
         * The range horizontally that the Metool must be from the player in order
         * to attack.
         * 
         * @type {Number}
         */
        static ATTACK_RANGE_X = 60;

        //
        // Instance variables
        //
        state = STATE_GUARDING;
        stateFunction = null;
        stateTimer = 0.0;
        enteringNewState = false;

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
            stateTimer += dt;
            shootingTimer += dt;

            if(stateFunction) {
                stateFunction();
            }

            /*
            switch(state) {
                case STATE_GUARDING:

                break;

                default:
                break;
            }

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
            */

            base.update(dt);
        }

        function attachHost(newHost, instanceConfig = {}) {
            base.attachHost(newHost);

            if(host != null) {
                setState("guarding");
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

        function setState(stateName) {
            stateTimer = 0.0;
            enteringNewState = true;

            ::log("setting state to " + stateName);

            if("guarding" == stateName) {
                stateFunction = _stateGuarding;
            } else if("attacking" == stateName) {
                stateFunction = _stateAttacking;
            } else if("walking" == stateName) {
                stateFunction = _stateWalking;
            }
        }

        /**
         * Function used when the Metool is in the "guarding" state. This is like
         * the update function for "guarding".
         * 
         * @return {[type]} [description]
         */
        function _stateGuarding() {
            if(enteringNewState) {
                ::log("_stateGuarding");
                enteringNewState = false;

                if(host) {
                    host.isShielded = true;
                    host.changeAnimation("guarding");
                    host.velocityX = 0.0;
                }
            }

            if(faceTimer >= 1.0) {
                facePlayer();
                faceTimer = 0;
            }

            if(stateTimer >= 1.0) {
                local distanceX = abs(::heroX - host.getX());

                if(distanceX <= ATTACK_RANGE_X && !isHeroShooting) {
                    setState("attacking");
                }
            }
        }

        /**
         * Function used when the Metool is in the "attacking" state.
         * 
         * @return {[type]} [description]
         */
        function _stateAttacking() {
            if(enteringNewState) {
                ::log("_stateAttacking");
                enteringNewState = false;

                if(host) {
                    host.isShielded = false;
                    host.changeAnimation("shooting");
                    host.weaponId = 5;
                }
            }

            // 7 frames in, shoot weapon
            if(!isShooting && stateTimer >= 0.1167) {
                host.fireWeapon();
                isShooting = true;
                stateTimer = 0.0;
            }

            if(stateTimer >= 1.0) {
                isShooting = false;
                setState("walking");
            }
        }

        function _stateWalking() {
            if(enteringNewState) {
                ::log("_stateWalking");
                enteringNewState = false;

                if(host) {
                    host.changeAnimation("walking");
                }
            }

            host.velocityX = host.direction == Directions.Left ? -2.5 : 2.5;

            if(stateTimer >= 0.5) {
                ::log("stateTimer >= 0.5");
                setState("guarding");
            }
            ::log("_stateWalking() ");
            ::log(stateTimer.tostring());
        }
    }
}());