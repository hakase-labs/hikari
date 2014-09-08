#ifndef HIKARI_CORE_GAME_GAMEPROGRESS
#define HIKARI_CORE_GAME_GAMEPROGRESS

#include "hikari/core/Platform.hpp"
#include "hikari/core/util/Service.hpp"
#include <memory>

namespace hikari {

    class EventBus;

    class HIKARI_API GameProgress : public Service {
    private:
        static const unsigned char NUM_BOSSES = 8;
        static const unsigned char NUM_WEAPONS = NUM_BOSSES + 3; // Need to determine this.
        static const unsigned char DEFAULT_PLAYER_ENERGY;
        static const int DEFAULT_WEAPON_ENERGY;
        static const int DEFAULT_NUM_LIVES;

        int lives;
        int eTanks;
        int mTanks;
        float playerEnergy;    // Your (the player's) energy
        float playerMaxEnergy;
        float bossEnergy;      // The energy of the boss currently being fought
        float bossMaxEnergy;
        float weaponEnergies[NUM_WEAPONS];
        unsigned char currentBoss;
        unsigned char currentWeapon;

        bool bossesDefeated[NUM_BOSSES];
        bool weaponsEnabled[NUM_WEAPONS];

        std::weak_ptr<EventBus> eventBus;
    public:
        GameProgress();
        virtual ~GameProgress();

        void setEventBus(const std::weak_ptr<EventBus> & eventBus);
        const std::weak_ptr<EventBus> & getEventBus() const;

        //
        // Getters
        //
        const unsigned char getBossCount() const;
        const unsigned char getDefeatedBossCount() const;
        int getLives() const;
        int getETanks() const;
        int getMTanks() const;
        float getPlayerEnergy() const;
        float getPlayerMaxEnergy() const;
        float getBossEnergy() const;
        float getBossMaxEnergy() const;
        unsigned char getCurrentBoss() const;
        unsigned char getCurrentWeapon() const;
        unsigned char getWeaponCount() const;
        float getWeaponEnergy(unsigned char weapon) const;
        float getWeaponMaxEnergy() const;
        unsigned char getWeakestWeapon() const;
        bool bossIsDefeated(unsigned char boss) const;
        bool weaponIsEnabled(unsigned char weapon) const;

        //
        // Setters
        //
        void setBossDefeated(unsigned char boss, bool defeated);
        void setLives(int value);
        void setETanks(int value);
        void setMTanks(int value);
        void setPlayerEnergy(float value);
        void setPlayerMaxEnergy(float value);
        void setBossEnergy(float value);
        void setBossMaxEnergy(float value);
        void setCurrentBoss(unsigned char boss);
        void setCurrentWeapon(unsigned char weapon);
        void setWeaponEnergy(unsigned char weapon, float value);
        void enableWeapon(unsigned char weapon, bool enable);

        //
        // Methods
        //
        void resetLivesToDefault();
        void resetPlayerEnergyToDefault();
        void resetWeaponEnergyToDefault();
    };

} // hikari

#endif // HIKARI_CORE_GAME_GAMEPROGRESS
