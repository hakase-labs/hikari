class ExtraLifeEffect extends EffectBase {
    constructor(config = { }) {
        base.constructor(config);
        ::log("ExtraLifeEffect constructor called.");
    }

    function applyEffect() {
        ::log("ExtraLifeEffect::applyEffect called.");
        ::log("You got an extra life!");

        ::hikari.sound.playSample(20);
    }

    function unapplyEffect() {
        ::log("ExtraLifeEffect::unapplyEffect called.");
    }
}