#ifndef HIKARI_CLIENT_GAMECONFIG
#define HIKARI_CLIENT_GAMECONFIG

#include <map>
#include <string>
#include <vector>

namespace Json {
    class Value;
}

namespace hikari {

    class GameConfig {
    private:
        // Property names
        static const char* PROPERTY_TEMPLATES;
        static const char* PROPERTY_TMPL_ITEM;
        static const char* PROPERTY_TMPL_ENEMY;
        static const char* PROPERTY_TMPL_PARTICLE;
        static const char* PROPERTY_TMPL_PROJECTILE;
        static const char* PROPERTY_TMPL_WEAPON;
        static const char* PROPERTY_INITIAL_STATE;
        static const char* PROPERTY_STARTUP_SCRIPTS;
        static const char* PROPERTY_WEAPONS;

        // Default values
        static const char* DEFAULT_ITEM_PATH;
        static const char* DEFAULT_ENEMY_PATH;
        static const char* DEFAULT_PARTICLE_PATH;
        static const char* DEFAULT_PROJECTILE_PATH;
        static const char* DEFAULT_WEAPON_PATH;
        static const char* DEFAULT_INITIAL_STATE;
        static const char* DEFAULT_SCRIPT_1;
        static const char* DEFAULT_SCRIPT_2;

        std::string itemTemplatePath;
        std::string enemyTemplatePath;
        std::string particleTemplatePath;
        std::string projectileTemplatePath;
        std::string weaponTemplatePath;
        std::string initialState;
        std::vector<std::string> startupScripts;
        std::vector<std::string> heroWeaponNames;

        void extractValuesFromJson(const Json::Value& configJson);
    public:
        GameConfig();
        GameConfig(const Json::Value& configJson);

        const std::string & getItemTemplatePath() const;
        const std::string & getEnemyTemplatePath() const;
        const std::string & getParticleTemplatePath() const;
        const std::string & getProjectileTemplatePath() const;
        const std::string & getWeaponTemplatePath() const;
        const std::string & getInitialState() const;
        const std::vector<std::string> & getStartUpScripts() const;
        const std::vector<std::string> & getHeroWeaponNames() const;
    };

} // hikari

#endif // HIKARI_CLIENT_GAMECONFIG
