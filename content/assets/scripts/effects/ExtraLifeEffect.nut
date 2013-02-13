class ExtraLifeEffect extends EffectBase {
    constructor(config = { }) {
        base.constructor(config);
        ::print("ExtraLifeEffect constructor called.");
    }

    function applyEffect() {
        ::print("ExtraLifeEffect::applyEffect called.");
        ::print("You got an extra life!");
    }

    function unapplyEffect() {
        ::print("ExtraLifeEffect::unapplyEffect called.");
    }
}