/**
 * Base class for all scripted effects (extra lives, get weapon energy, get 
 * health energy, etc.). Extend this class to implement a different kind of
 * effect.
 */
class EffectBase {
    /**
     * Constructor. Takes an optional configuration table for class-level 
     * configuration settings.
     *
     * @param  {Table} classConfig an optional table for class-level 
     *                             configuration settings.
     */
    constructor(config = {}) {
        ::log("EffectBase constructor called.");
    }

    /**
     * Called when the effect should be applied. This method is where the effect
     * applying logic is implemented.
     *
     * @see EffectBase::unapplyEffect
     */
    function applyEffect() {
        ::log("EffectBase::applyEffect called.");
    }

    /**
     * Called when the effect should be unapplied (the reverse effect). This
     * method is where the logic to reverse the effect is implemented.
     *
     * @see EffectBase::applyEffect
     */
    function unapplyEffect() {
        ::log("EffectBase::unapplyEffect called.");
    }
}

::log("EffectBase.nut executed!");