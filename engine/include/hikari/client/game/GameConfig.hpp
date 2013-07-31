#ifndef HIKARI_CLIENT_GAMECONFIG
#define HIKARI_CLIENT_GAMECONFIG

#include <map>
#include <string>

namespace Json {
    class Value;
}

namespace hikari {

    class GameConfig {
    private:
        static const char* PROPERTY_TEMPLATES;
        static const char* PROPERTY_TMPL_ITEM;
        static const char* PROPERTY_TMPL_ENEMY;
        static const char* PROPERTY_TMPL_PARTICLE;
        static const char* PROPERTY_TMPL_PROJECTILE;
        static const char* PROPERTY_TMPL_WEAPON;

        std::string itemTemplatePath;
        std::string enemyTemplatePath;
        std::string particleTemplatePath;
        std::string projectileTemplatePath;
        std::string weaponTemplatePath;

        void extractValuesFromJson(const Json::Value& configJson);
    public:
        GameConfig();
        GameConfig(const Json::Value& configJson);

        const std::string & getItemTemplatePath() const;
        const std::string & getEnemyTemplatePath() const;
        const std::string & getParticleTemplatePath() const;
        const std::string & getProjectileTemplatePath() const;
        const std::string & getWeaponTemplatePath() const;
    };

} // hikari

#endif // HIKARI_CLIENT_GAMECONFIG
