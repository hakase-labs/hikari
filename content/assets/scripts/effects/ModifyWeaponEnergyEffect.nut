class ModifyWeaponEnergyEffect extends EffectBase {
    amount = 0;

    constructor(config = { amount = 0 }) {
        // Ensure valid configuration parameters.
        config = _.defaults(config, { 
            amount = 0 
        });

        base.constructor(config);

        amount = config.amount;

        ::log("ModifyWeaponEnergyEffect constructor called.");
    }

    function applyEffect() {
        ::log("ModifyWeaponEnergyEffect::applyEffect called.");
        ::log("You life was modified by " + amount + "!");
        ::hikari.game.refillWeapon(amount);
    }

    function unapplyEffect() {
        ::log("ModifyWeaponEnergyEffect::unapplyEffect called.");
    }
}

::log("ModifyWeaponEnergyEffect processed!");