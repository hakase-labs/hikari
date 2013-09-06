#include "hikari/client/game/GameProgress.hpp"

#include <algorithm>

namespace hikari {

    const int GameProgress::DEFAULT_NUM_LIVES = 3;
    const unsigned char GameProgress::DEFAULT_PLAYER_ENERGY = 56;

    GameProgress::GameProgress()
        : lives(3)
        , eTanks(0)
        , mTanks(0)
        , playerEnergy(DEFAULT_PLAYER_ENERGY)
        , currentBoss(0)
        , currentWeapon(0)
    {
        std::fill(weaponEnergies, weaponEnergies + sizeof(weaponEnergies), 0);
        std::fill(weaponsEnabled, weaponsEnabled + sizeof(weaponsEnabled), false);
        std::fill(bossesDefeated, bossesDefeated + sizeof(bossesDefeated), false);
    }

    GameProgress::~GameProgress() { }

    const unsigned char GameProgress::getBossCount() const {
        return GameProgress::NumBosses;
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

    unsigned char GameProgress::getETanks() const {
        return eTanks;
    }

    unsigned char GameProgress::getMTanks() const {
        return mTanks;
    }

    int GameProgress::getPlayerEnergy() const {
        return playerEnergy;
    }

    unsigned char GameProgress::getWeaponCount() const {
        return GameProgress::NumWeapons;
    }

    unsigned char GameProgress::getCurrentBoss() const {
        return currentBoss;
    }

    unsigned char GameProgress::getCurrentWeapon() const {
        return currentWeapon;
    }

    unsigned char GameProgress::getWeaponEnergy(unsigned char weapon) const {
        if(weapon < getWeaponCount()) {
            return weaponEnergies[weapon];
        }
        return 0;
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

    void GameProgress::setETanks(unsigned char value) {
        eTanks = value;
    }

    void GameProgress::setMTanks(unsigned char value) {
        mTanks = value;
    }

    void GameProgress::setPlayerEnergy(int value) {
        playerEnergy = std::max(0, value);
    }

    void GameProgress::setCurrentBoss(unsigned char boss) {
        if(boss < getBossCount()) {
            currentBoss = boss;
        }
    }

    void GameProgress::setCurrentWeapon(unsigned char weapon) {
        currentWeapon = weapon;
    }

    void GameProgress::setWeaponEnergy(unsigned char weapon, unsigned char value) {
        if(weapon < getWeaponCount()) {
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
            setWeaponEnergy(i, 100);
        }
    }

} // hikari
