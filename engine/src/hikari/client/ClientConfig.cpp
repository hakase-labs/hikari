#include "hikari/client/ClientConfig.hpp"
#include <json/value.h>

namespace hikari {
    const char* ClientConfig::PROPERTY_VSYNC = "vsync";
    const char* ClientConfig::PROPERTY_FPS = "showfps";
    const char* ClientConfig::PROPERTY_SCRIPTING = "scripting";
    const char* ClientConfig::PROPERTY_SCRIPTING_STACKSIZE = "stackSize";

    void ClientConfig::extractValuesFromJson(const Json::Value& configJson) {
        if(!configJson.isNull()) {
            if(configJson.isMember(PROPERTY_FPS)) {
                enableFpsDisplay = configJson.get(PROPERTY_FPS, false).asBool();
            }
            if(configJson.isMember(PROPERTY_VSYNC)) {
                enableVsync = configJson.get(PROPERTY_VSYNC, false).asBool();
            }

            //
            // Extract scripting config values
            //
            if(configJson.isMember(PROPERTY_SCRIPTING)) {
                auto scriptingConfigJson = configJson.get(PROPERTY_SCRIPTING, Json::Value());

                if(scriptingConfigJson.isMember(PROPERTY_SCRIPTING_STACKSIZE)) {
                    stackSize = static_cast<unsigned int>(scriptingConfigJson.get(PROPERTY_SCRIPTING_STACKSIZE, 1024).asUInt());
                }
            }
        }
    }

    ClientConfig::ClientConfig()
        : enableVsync(false)
        , enableFpsDisplay(false)
        , stackSize(1024)
    {

    }

    ClientConfig::ClientConfig(const Json::Value& configJson)
        : enableVsync(false)
        , enableFpsDisplay(false)
        , stackSize(1024)
    {
        extractValuesFromJson(configJson);
    }

    const bool ClientConfig::isVsyncEnabled() const {
        return enableVsync;
    }

    const bool ClientConfig::isFpsDisplayEnabled() const {
        return enableFpsDisplay;
    }

    const unsigned int ClientConfig::getScriptingStackSize() const {
        return stackSize;
    }
} // hikari
