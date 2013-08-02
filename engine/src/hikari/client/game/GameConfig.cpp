#include "hikari/client/game/GameConfig.hpp"

#include <algorithm>

#include <json/value.h>

namespace hikari {

    const char* GameConfig::PROPERTY_TEMPLATES = "templatePaths";
    const char* GameConfig::PROPERTY_TMPL_ITEM = "items";
    const char* GameConfig::PROPERTY_TMPL_ENEMY = "enemies";
    const char* GameConfig::PROPERTY_TMPL_PARTICLE = "particles";
    const char* GameConfig::PROPERTY_TMPL_PROJECTILE = "projectiles";
    const char* GameConfig::PROPERTY_TMPL_WEAPON = "weapons";
    const char* GameConfig::PROPERTY_INITIAL_STATE = "initialState";
    const char* GameConfig::PROPERTY_STARTUP_SCRIPTS = "environment";

    const char* GameConfig::DEFAULT_ITEM_PATH = "items.json";
    const char* GameConfig::DEFAULT_ENEMY_PATH = "enemy.json";
    const char* GameConfig::DEFAULT_PARTICLE_PATH = "particles.json";
    const char* GameConfig::DEFAULT_PROJECTILE_PATH = "projectiles.json";
    const char* GameConfig::DEFAULT_WEAPON_PATH = "weapons.json";
    const char* GameConfig::DEFAULT_INITIAL_STATE = "introduction";
    const char* GameConfig::DEFAULT_SCRIPT_1 = "Environment.nut";
    const char* GameConfig::DEFAULT_SCRIPT_2 = "Bootstrap.nut";

    GameConfig::GameConfig()
        : itemTemplatePath(DEFAULT_ITEM_PATH)
        , enemyTemplatePath(DEFAULT_ENEMY_PATH)
        , particleTemplatePath(DEFAULT_PARTICLE_PATH)
        , projectileTemplatePath(DEFAULT_PROJECTILE_PATH)
        , weaponTemplatePath(DEFAULT_WEAPON_PATH)
        , initialState(DEFAULT_INITIAL_STATE)
        , startupScripts()
    {

    }

    GameConfig::GameConfig(const Json::Value& configJson)
        : itemTemplatePath(DEFAULT_ITEM_PATH)
        , enemyTemplatePath(DEFAULT_ENEMY_PATH)
        , particleTemplatePath(DEFAULT_PARTICLE_PATH)
        , projectileTemplatePath(DEFAULT_PROJECTILE_PATH)
        , weaponTemplatePath(DEFAULT_WEAPON_PATH)
        , initialState(DEFAULT_INITIAL_STATE)
        , startupScripts()
    {
        extractValuesFromJson(configJson);
    }

    void GameConfig::extractValuesFromJson(const Json::Value& configJson) {
        if(!configJson.isNull()) {
            // Startup scripts
            if(configJson.isMember(PROPERTY_STARTUP_SCRIPTS)) {
                const Json::Value & scriptArray = configJson.get(PROPERTY_STARTUP_SCRIPTS, Json::Value());
                std::size_t count = scriptArray.size();

                for(std::size_t i = 0; i < count; ++i) {
                    startupScripts.push_back(scriptArray[i].asString());
                }

            }

            // Template paths
            if(configJson.isMember(PROPERTY_TEMPLATES)) {
                const Json::Value & templatePathsJson = configJson.get(PROPERTY_TEMPLATES, Json::Value());
                itemTemplatePath = templatePathsJson.get(PROPERTY_TMPL_ITEM, DEFAULT_ITEM_PATH).asString();
                enemyTemplatePath = templatePathsJson.get(PROPERTY_TMPL_ENEMY, DEFAULT_ENEMY_PATH).asString();
                particleTemplatePath = templatePathsJson.get(PROPERTY_TMPL_PARTICLE, DEFAULT_PARTICLE_PATH).asString();
                projectileTemplatePath = templatePathsJson.get(PROPERTY_TMPL_PROJECTILE, DEFAULT_PROJECTILE_PATH).asString();
                weaponTemplatePath = templatePathsJson.get(PROPERTY_TMPL_WEAPON, DEFAULT_WEAPON_PATH).asString();
            }

            // Initial game state
            initialState = configJson.get(PROPERTY_INITIAL_STATE, DEFAULT_INITIAL_STATE).asString();
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

    const std::string & GameConfig::getInitialState() const {
        return initialState;
    }

    const std::vector<std::string> & GameConfig::getStartUpScripts() const {
        return startupScripts;
    }

} // hikari
