#include "hikari/client/game/GameConfig.hpp"

#include <algorithm>

#include <json/value.h>
#include <json/writer.h>

namespace hikari {

    const char* GameConfig::PROPERTY_TEMPLATES = "templatePaths";
    const char* GameConfig::PROPERTY_TMPL_ITEM = "items";
    const char* GameConfig::PROPERTY_TMPL_ENEMY = "enemies";
    const char* GameConfig::PROPERTY_TMPL_PARTICLE = "particles";
    const char* GameConfig::PROPERTY_TMPL_PROJECTILE = "projectiles";
    const char* GameConfig::PROPERTY_TMPL_WEAPON = "weapons";

    GameConfig::GameConfig()
        : itemTemplatePath("")
        , enemyTemplatePath("")
        , particleTemplatePath("")
        , projectileTemplatePath("")
        , weaponTemplatePath("") 
    {

    }

    GameConfig::GameConfig(const Json::Value& configJson)
        : itemTemplatePath("")
        , enemyTemplatePath("")
        , particleTemplatePath("")
        , projectileTemplatePath("")
        , weaponTemplatePath("")
    {
        extractValuesFromJson(configJson);
    }

    void GameConfig::extractValuesFromJson(const Json::Value& configJson) {
        if(!configJson.isNull()) {
            if(configJson.isMember(PROPERTY_TEMPLATES)) {
                const Json::Value & templatePathsJson = configJson.get(PROPERTY_TEMPLATES, Json::Value());

                if(templatePathsJson.isMember(PROPERTY_TMPL_ITEM)) {
                    itemTemplatePath = templatePathsJson.get(PROPERTY_TMPL_ITEM, "items.json").asString();
                }

                if(templatePathsJson.isMember(PROPERTY_TMPL_ENEMY)) {
                    enemyTemplatePath = templatePathsJson.get(PROPERTY_TMPL_ENEMY, "enemies.json").asString();
                }

                if(templatePathsJson.isMember(PROPERTY_TMPL_PARTICLE)) {
                    particleTemplatePath = templatePathsJson.get(PROPERTY_TMPL_PARTICLE, "particles.json").asString();
                }

                if(templatePathsJson.isMember(PROPERTY_TMPL_PROJECTILE)) {
                    projectileTemplatePath = templatePathsJson.get(PROPERTY_TMPL_PROJECTILE, "projectiles.json").asString();
                }

                if(templatePathsJson.isMember(PROPERTY_TMPL_WEAPON)) {
                    weaponTemplatePath = templatePathsJson.get(PROPERTY_TMPL_WEAPON, "weapons.json").asString();
                }
            }
        }
    }

    const std::string & GameConfig::getItemTemplatePath() const {
        return itemTemplatePath;
    }

    const std::string & GameConfig::getEnemyTemplatePath() const {
        return enemyTemplatePath;
    }

    const std::string & GameConfig::getParticleTemplatePath() const {
        return particleTemplatePath;
    }

    const std::string & GameConfig::getProjectileTemplatePath() const {
        return projectileTemplatePath;
    }

    const std::string & GameConfig::getWeaponTemplatePath() const {
        return weaponTemplatePath;
    }

} // hikari
