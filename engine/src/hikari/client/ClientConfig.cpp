#include <hikari/client/ClientConfig.hpp>
#include <json/value.h>

namespace hikari {
    const char* ClientConfig::PROPERTY_VSYNC = "vsync";
    const char* ClientConfig::PROPERTY_FPS = "showfps";

    void ClientConfig::extractValuesFromJson(const Json::Value& configJson) {
        if(!configJson.isNull()) {
            if(configJson.isMember(PROPERTY_FPS)) {
                enableFpsDisplay = configJson.get(PROPERTY_FPS, false).asBool();
            }
            if(configJson.isMember(PROPERTY_VSYNC)) {
                enableVsync = configJson.get(PROPERTY_VSYNC, false).asBool();
            }
        }
    }

    ClientConfig::ClientConfig(const Json::Value& configJson)
        : enableVsync(false)
        , enableFpsDisplay(false)
    {
        extractValuesFromJson(configJson);
    }

    const bool ClientConfig::isVsyncEnabled() const {
        return enableVsync;
    }

    const bool ClientConfig::isFpsDisplayEnabled() const {
        return enableFpsDisplay;
    }
} // hikari
