#include <hikari/client/game/GameProgress.hpp>

#include <algorithm>

namespace hikari {
namespace client {
namespace game {

	GameProgress::GameProgress() 
		: lives(3)
		, eTanks(0)
		, mTanks(0)
		, playerEnergy(GameProgress::MaxEnergy)
		, currentWeapon(0)
		, currentBoss(0) 
	{
		std::fill(weaponEnergies, weaponEnergies + sizeof(weaponEnergies), 0);
		std::fill(weaponsEnabled, weaponsEnabled + sizeof(weaponsEnabled), false);
		std::fill(bossesDefeated, bossesDefeated + sizeof(bossesDefeated), false);
	}

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

	unsigned char GameProgress::getLives() const {
		return lives;
	}

	unsigned char GameProgress::getETanks() const {
		return eTanks;
	}

	unsigned char GameProgress::getMTanks() const {
		return mTanks;
	}

	unsigned char GameProgress::getPlayerEnergy() const {
		return playerEnergy;
	}

	unsigned char GameProgress::getWeaponCount() const {
		return GameProgress::NumWeapons;
	}
	
	unsigned char GameProgress::getCurrentWeapon() const {
		return currentWeapon;
	}

	unsigned char GameProgress::getWeaponEnergy(unsigned char weapon) const {
		if(weapon >= 0 && weapon < getWeaponCount()) {
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
		if(boss >= 0 && boss < getBossCount()) {
			return bossesDefeated[boss];
		}
		return false;
	}

	bool GameProgress::weaponIsEnabled(unsigned char weapon) const {
		if(weapon >= 0 && weapon < getWeaponCount()) {
			return weaponsEnabled[weapon];
		}
		return false;
	}

	void GameProgress::setBossDefeated(unsigned char boss, bool defeated) {
		if(boss >= 0 && boss < getBossCount()) {
			bossesDefeated[boss] = defeated;
		}
	}

	void GameProgress::setLives(unsigned char value) {
		lives = value;
	}

	void GameProgress::setETanks(unsigned char value) {
		eTanks = value;
	}

	void GameProgress::setMTanks(unsigned char value) {
		mTanks = value;
	}

	void GameProgress::setPlayerEnergy(unsigned char value) {
		playerEnergy = value;
	}

	void GameProgress::setCurrentWeapon(unsigned char weapon) {
		currentWeapon = weapon;
	}

	void GameProgress::setWeaponEnergy(unsigned char weapon, unsigned char value) {
		if(weapon >= 0 && weapon < getWeaponCount()) {
			weaponEnergies[weapon] = value;
		}
	}

	void GameProgress::enableWeapon(unsigned char weapon, bool enable) {
		if(weapon >= 0 && weapon < getWeaponCount()) {
			weaponsEnabled[weapon] = enable;
		}
	}


} // hikari.client.game
} // hikari.client
} // hikari