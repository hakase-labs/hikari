class ExtraLifeEffect extends EffectBase {
    constructor(config = { }) {
        base.constructor(config);
    }

    function applyEffect() {
        ::hikari.game.setLives(::hikari.game.getLives() + 1);
        ::hikari.sound.playSample("Power Up");
    }

    function unapplyEffect() {
        ::hikari.game.setLives(::hikari.game.getLives() - 1);
    }
}