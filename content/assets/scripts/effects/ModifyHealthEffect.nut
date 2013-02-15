class ModifyHealthEffect extends EffectBase {
    amount = 0;

    constructor(config = { amount = 0 }) {
        // Ensure valid configuration parameters.
        config = _.defaults(config, { 
            amount = 0 
        });

        base.constructor(config);

        amount = config.amount;

        ::print("ModifyHealthEffect constructor called.");
    }

    function applyEffect() {
        ::print("ModifyHealthEffect::applyEffect called.");
        ::print("You life was modified by " + amount + "!");
    }

    function unapplyEffect() {
        ::print("ModifyHealthEffect::unapplyEffect called.");
    }
}

::print("ModifyHealthEffect processed!");