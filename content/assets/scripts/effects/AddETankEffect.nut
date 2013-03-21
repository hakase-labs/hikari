class AddETankEffect extends EffectBase {
    constructor(config = { }) {
        base.constructor(config);
        ::log("AddETankEffect constructor called.");
    }

    function applyEffect() {
        ::log("AddETankEffect::applyEffect called.");
        ::log("You got an E-Tank!");

        ::hikari.sound.playSample(20);
    }

    function unapplyEffect() {
        ::log("AddETankEffect::unapplyEffect called.");
    }
}

// ::log("AddETankEffect processed!");