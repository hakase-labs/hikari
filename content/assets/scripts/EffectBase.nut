class EffectBase {
    constructor(config = {}) {
        ::log("EffectBase constructor called.");
    }

    function applyEffect() {
        ::log("EffectBase::applyEffect called.");
    }

    function unapplyEffect() {
        ::log("EffectBase::unapplyEffect called.");
    }
}

// ::log("EffectBase.nut executed!");