#include "hikari/client/game/GameProgress.hpp"
#include "hikari/client/game/events/EventBus.hpp"

#include <algorithm>

namespace hikari {

    const int GameProgress::DEFAULT_NUM_LIVES = 3;
    const unsigned char GameProgress::DEFAULT_PLAYER_ENERGY = 28;
    const int GameProgress::DEFAULT_WEAPON_ENERGY = 28;

    GameProgress::GameProgress()
        : lives(3)
        , eTanks(0)
        , mTanks(0)
        , playerEnergy(DEFAULT_PLAYER_ENERGY)
        , playerMaxEnergy(DEFAULT_PLAYER_ENERGY)
        , bossEnergy(DEFAULT_PLAYER_ENERGY)
        , bossMaxEnergy(DEFAULT_PLAYER_ENERGY)
        , currentBoss(0)
        , currentWeapon(0)
    {
        std::fill(std::begin(weaponEnergies), std::end(weaponEnergies), DEFAULT_WEAPON_ENERGY);
        std::fill(std::begin(weaponsEnabled), std::end(weaponsEnabled), false);
        std::fill(std::begin(bossesDefeated), std::end(bossesDefeated), false);

        // Mega Buster is always the 0th weapon, so enable it
        enableWeapon(0, true);
        enableWeapon(1, true);
        enableWeapon(2, true);
        setETanks(4);
    }

    GameProgress::~GameProgress() { }

    void GameProgress::setEventBus(const std::weak_ptr<EventBus>& eventBus) {
        this->eventBus = eventBus;
    }

    const std::weak_ptr<EventBus>& GameProgress::getEventBus() const {
        return eventBus;
    }

    const unsigned char GameProgress::getBossCount() const {
        return GameProgress::NUM_BOSSES;
    }

    const unsigned char GameProgress::getDefeatedBossCount() const {
        unsigned char count = 0;

        for(unsigned char i = 0; i < getBossCount(); ++i) {
            if(bossIsDefeated(i)) {
                count++;
            }
        }

        return count;
    }

    int GameProgress::getLives() const {
        return lives;
    }

    int GameProgress::getETanks() const {
        return eTanks;
    }

    int GameProgress::getMTanks() const {
        return mTanks;
    }

    float GameProgress::getPlayerEnergy() const {
        return playerEnergy;
    }

    float GameProgress::getPlayerMaxEnergy() const {
        return playerMaxEnergy;
    }

    float GameProgress::getBossEnergy() const {
        return bossEnergy;
    }

    float GameProgress::getBossMaxEnergy() const {
        return bossMaxEnergy;
    }

    unsigned char GameProgress::getWeaponCount() const {
        return GameProgress::NUM_BOSSES;
    }

    unsigned char GameProgress::getCurrentBoss() const {
        return currentBoss;
    }

    unsigned char GameProgress::getCurrentWeapon() const {
        return currentWeapon;
    }

    float GameProgress::getWeaponEnergy(unsigned char weapon) const {
        if(weapon < getWeaponCount()) {
            return weaponEnergies[weapon];
        }
        return 0;
    }

    float GameProgress::getWeaponMaxEnergy() const {
        return DEFAULT_WEAPON_ENERGY;
    }

    unsigned char GameProgress::getWeakestWeapon() const {
        unsigned char weakest = 0;

        for(unsigned char i = 0; i < getWeaponCount(); ++i) {
            if(getWeaponEnergy(i) < getWeaponEnergy(weakest)) {
                weakest = i;
            }
        }

        return weakest;
    }

    bool GameProgress::bossIsDefeated(unsigned char boss) const {
        if(boss < getBossCount()) {
            return bossesDefeated[boss];
        }
        return false;
    }

    bool GameProgress::weaponIsEnabled(unsigned char weapon) const {
        if(weapon < getWeaponCount()) {
            return weaponsEnabled[weapon];
        }
        return false;
    }

    void GameProgress::setBossDefeated(unsigned char boss, bool defeated) {
        if(boss < getBossCount()) {
            bossesDefeated[boss] = defeated;
        }
    }

    void GameProgress::setLives(int value) {
        lives = value;
    }

    void GameProgress::setETanks(int value) {
        eTanks = value;
    }

    void GameProgress::setMTanks(int value) {
        mTanks = value;
    }

    void GameProgress::setPlayerEnergy(float value) {
        playerEnergy = std::max(0.0f, value);
    }

    void GameProgress::setPlayerMaxEnergy(float value) {
        playerMaxEnergy = std::max(0.0f, value);
    }

    void GameProgress::setBossEnergy(float value) {
        bossEnergy = std::max(0.0f, value);
    }

    void GameProgress::setBossMaxEnergy(float value) {
        bossMaxEnergy = std::max(0.0f, value);
    }

    void GameProgress::setCurrentBoss(unsigned char boss) {
        if(boss < getBossCount()) {
            currentBoss = boss;
        }
    }

    void GameProgress::setCurrentWeapon(unsigned char weapon) {
        if(getCurrentWeapon() != weapon) {
            currentWeapon = weapon;

            if(auto events = eventBus.lock()) {
                // events->triggerEvent(
                //     std::make_shared<PropertyChangedEventData>(
                //         "currentWeapon"
                //     )
                // );
            }
        }

    }

    void GameProgress::setWeaponEnergy(unsigned char weapon, float value) {
        if(weapon < getWeaponCount()) {
            value = std::max(0.0f, value);
            weaponEnergies[weapon] = value;
        }
    }

    void GameProgress::enableWeapon(unsigned char weapon, bool enable) {
        if(weapon < getWeaponCount()) {
            weaponsEnabled[weapon] = enable;
        }
    }

    void GameProgress::resetLivesToDefault() {
        setLives(DEFAULT_NUM_LIVES);
    }

    void GameProgress::resetPlayerEnergyToDefault() {
        setPlayerEnergy(DEFAULT_PLAYER_ENERGY);
    }

    void GameProgress::resetWeaponEnergyToDefault() {
        for(unsigned char i = 0; i < getWeaponCount(); ++i) {
            setWeaponEnergy(i, DEFAULT_WEAPON_ENERGY);
        }
    }

} // hikari
