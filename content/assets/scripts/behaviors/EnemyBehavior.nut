enum State {
  IDLE,
  MOVING,
  SHOOTING,
  PREJUMPING,
  JUMPING,
  FALLING,
  LANDING
}

/**
 * EnemyBehavior is the base class for all scripted enemy behaviors. Extend this
 * class to create a new kind of behavior. This class provides an interface for
 * the game engine to communicate with a scripted enemy.
 */
class EnemyBehavior {
    /**
     * A proxy object back to an Enemy instance in the game engine.
     * @type {Enemy}
     */
    host = null;

    /**
     * Constructor. Takes a configuration table (optional).
     *
     * @param {Table} classConfig a configuration object for class-level 
     *                            settings. This can not be used to set values
     *                            on individual instances.
     */
    constructor(classConfig = {}) {
        ::log("EnemyBehavior constructor called.");
        host = null;
    }

    /**
     * Attaches this behavior to an instance of an Enemy (the host). This can be
     * used to perform any additional logic required when an enemy is spawned.
     *
     * Enemies can optionally be passed a table containing instance-level 
     * configuration settings. These can be used to specify things like how fast
     * a particular enemy is moving, how much health it has, what its initial 
     * state is, etc., on a per-instance basis.
     *
     * @param  {Enemy} newHost        an instance of Enemy to attach to
     * @param  {Table} instanceConfig an optional configuration table for 
     *                                instance-level settings.
     */
    function attachHost(newHost, instanceConfig = {}) {
        host = newHost;
    }

    /**
     * Detaches the behavior from its host (if it has one). This makes the host
     * inaccessible from the behavior instance (host will become null).
     */
    function detachHost() {
        host = null;
    }

    /**
     * Applies an instance-level configuration to the behavior instance.
     *
     * @param  {Table} config a configuration table for instance-level settings.
     */
    function applyConfig(config = {}) {
        // To be implemented in subclasses
    }

    /**
     * Updates the behavior by a given amount of time. This is where most of the
     * logic for a behavior should be performed.
     *
     * @param  {Float} dt the amount of time, in seconds, that this update tick
     *                    accounts for.
     */
    function update(dt) {
        
    }

    /**
     * Performs any handling of world collisions. By default ddeosn nothing, but
     * can be overridden to perform logic when colliding with different tiles in
     * the world. Any time there is a tile collision this method will be called,
     * for all tile types (so basically every frame as long as the enemy is not
     * phasing).
     *
     * @param  {Direction} side the side of the host that has collided
     */
    function handleWorldCollision(side) {

    }
}

::log("EnemyBehavior.nut executed!");