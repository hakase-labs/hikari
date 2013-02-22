class AddETankEffect extends EffectBase {
    constructor(config = { }) {
        base.constructor(config);
        ::print("AddETankEffect constructor called.");
    }

    function applyEffect() {
        ::print("AddETankEffect::applyEffect called.");
        ::print("You got an E-Tank!");

        ::hikari.sound.playSample(20);
    }

    function unapplyEffect() {
        ::print("AddETankEffect::unapplyEffect called.");
    }
}

// ::print("AddETankEffect processed!");