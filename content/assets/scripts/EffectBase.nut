class EffectBase {
    constructor() {
        ::print("EffectBase constructor called.");
    }

    function applyEffect() {
        ::print("EffectBase::applyEffect called.");
    }

    function unapplyEffect() {
        ::print("EffectBase::unapplyEffect called.");
    }
}

::print("EffectBase.nut executed!");