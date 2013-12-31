class ModifyHealthEffect extends EffectBase {
    amount = 0;

    constructor(config = { amount = 0 }) {
        // Ensure valid configuration parameters.
        config = _.defaults(config, { 
            amount = 0 
        });

        base.constructor(config);

        amount = config.amount;

        ::log("ModifyHealthEffect constructor called.");
    }

    function applyEffect() {
        ::log("ModifyHealthEffect::applyEffect called.");
        ::log("You life was modified by " + amount + "!");
        ::hikari.game.refillHealth(amount);
    }

    function unapplyEffect() {
        ::log("ModifyHealthEffect::unapplyEffect called.");
    }
}

::log("ModifyHealthEffect processed!");