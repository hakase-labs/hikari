class AddETankEffect extends EffectBase {
    constructor(config = { }) {
        base.constructor(config);
    }

    function applyEffect() {
        ::hikari.game.setETanks(::hikari.game.getETanks() + 1);
        ::hikari.sound.playSample(20);
    }

    function unapplyEffect() {
        ::hikari.game.setETanks(::hikari.game.getETanks() - 1);
    }
}

// ::log("AddETankEffect processed!");