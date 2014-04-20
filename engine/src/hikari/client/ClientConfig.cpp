#include "hikari/client/ClientConfig.hpp"

#include <algorithm>

#include <json/value.h>
#include <json/writer.h>

namespace hikari {
    const char* ClientConfig::PROPERTY_AUDIO = "audio";
    const char* ClientConfig::PROPERTY_SAMPLES = "samples";
    const char* ClientConfig::PROPERTY_MUSIC = "music";
    const char* ClientConfig::PROPERTY_VSYNC = "vsync";
    const char* ClientConfig::PROPERTY_FPS = "showfps";
    const char* ClientConfig::PROPERTY_SCRIPTING = "scripting";
    const char* ClientConfig::PROPERTY_SCRIPTING_STACKSIZE = "stackSize";
    const char* ClientConfig::PROPERTY_VIDEOMODE = "videoMode";
    const char* ClientConfig::PROPERTY_BINDINGS = "bindings";
    const char* ClientConfig::PROPERTY_KEYBOARD_BINDINGS = "keyboard";

    const char* ClientConfig::VIDEO_SCALE_1X = "1x";
    const char* ClientConfig::VIDEO_SCALE_2X = "2x";
    const char* ClientConfig::VIDEO_SCALE_3X = "3x";
    const char* ClientConfig::VIDEO_SCALE_FULL = "full";

    void ClientConfig::extractValuesFromJson(const Json::Value& configJson) {
        if(!configJson.isNull()) {
            if(configJson.isMember(PROPERTY_FPS)) {
                enableFpsDisplay = configJson.get(PROPERTY_FPS, false).asBool();
            }
            if(configJson.isMember(PROPERTY_VSYNC)) {
                enableVsync = configJson.get(PROPERTY_VSYNC, false).asBool();
            }

            //
            // Extract audio settings
            //
            if(configJson.isMember(PROPERTY_AUDIO)) {
                const Json::Value & audioConfigJson = configJson.get(PROPERTY_AUDIO, Json::Value());
                const double sampleVolumeSetting = audioConfigJson.get(PROPERTY_SAMPLES, 100.f).asDouble();
                const double musicVolumeSetting = audioConfigJson.get(PROPERTY_MUSIC, 100.f).asDouble();

                musicVolume = static_cast<float>(musicVolumeSetting);
                sampleVolume = static_cast<float>(sampleVolumeSetting);
            }

            //
            // Extract scripting config values
            //
            if(configJson.isMember(PROPERTY_SCRIPTING)) {
                const Json::Value & scriptingConfigJson = configJson.get(PROPERTY_SCRIPTING, Json::Value());

                if(scriptingConfigJson.isMember(PROPERTY_SCRIPTING_STACKSIZE)) {
                    stackSize = static_cast<unsigned int>(scriptingConfigJson.get(PROPERTY_SCRIPTING_STACKSIZE, 1024).asUInt());
                }
            }

            //
            // Extract video mode settings
            //
            if(configJson.isMember(PROPERTY_VIDEOMODE)) {
                videoMode = configJson.get(PROPERTY_VIDEOMODE, "1x").asString();
            }

            //
            // Extract keyboard bindings
            //
            if(configJson.isMember(PROPERTY_BINDINGS)) {
                const Json::Value & bindingsObject = configJson[PROPERTY_BINDINGS];

                if(bindingsObject.isMember(PROPERTY_KEYBOARD_BINDINGS)) {
                    const Json::Value & keyboardBindingObject = bindingsObject[PROPERTY_KEYBOARD_BINDINGS];
                    const Json::Value::Members & keyboardBindingKeys = keyboardBindingObject.getMemberNames();
                 
                    for(unsigned int i = 0, length = keyboardBindingKeys.size(); i < length; i++) {
                        const std::string & key = keyboardBindingKeys[i];
                        
                        //
                        // TODO: Additional checking to make sure these keys are valid buttons
                        //

                        keybindings.insert(
                            std::make_pair(key, keyboardBindingObject[key].asString())
                        );
                    }
                }
            }
        }
    }

    ClientConfig::ClientConfig()
        : enableVsync(false)
        , enableFpsDisplay(false)
        , stackSize(1024)
        , musicVolume(100.0f)
        , sampleVolume(100.0f)
        , videoMode("1x")
        , keybindings()
    {

    }

    ClientConfig::ClientConfig(const Json::Value& configJson)
        : enableVsync(false)
        , enableFpsDisplay(false)
        , stackSize(1024)
        , musicVolume(100.0f)
        , sampleVolume(100.0f)
        , videoMode("1x")
        , keybindings()
    {
        extractValuesFromJson(configJson);
    }

    bool ClientConfig::isVsyncEnabled() const {
        return enableVsync;
    }

    void ClientConfig::setVsyncEnabled(bool enabled) {
        enableVsync = enabled;
    }

    bool ClientConfig::isFpsDisplayEnabled() const {
        return enableFpsDisplay;
    }

    void ClientConfig::setFpsDisplayEnabled(bool enabled) {
        enableFpsDisplay = enabled;
    }

    float ClientConfig::getMusicVolume() const {
        return musicVolume;
    }

    void ClientConfig::setMusicVolume(float volume) {
        musicVolume = volume;
    }

    float ClientConfig::getSampleVolume() const {
        return sampleVolume;
    }

    void ClientConfig::setSampleVolume(float volume) {
        sampleVolume = volume;
    }

    unsigned int ClientConfig::getScriptingStackSize() const {
        return stackSize;
    }

    std::string ClientConfig::getVideoMode() const {
        return videoMode;
    }

    void ClientConfig::setVideoMode(const std::string & mode) {
        videoMode = mode;
    }

    ClientConfig::StringMap ClientConfig::getKeyBindings() const {
        return keybindings;
    }

    void ClientConfig::setKeyBindings(const ClientConfig::StringMap & bindings) {
        keybindings = bindings;
    }

    std::string ClientConfig::serialize() {
        Json::StyledWriter writer;
        Json::Value container(Json::objectValue);

        container[PROPERTY_VSYNC] = isVsyncEnabled();
        container[PROPERTY_VIDEOMODE] = getVideoMode();
        container[PROPERTY_AUDIO] = Json::Value(Json::objectValue);
        container[PROPERTY_AUDIO][PROPERTY_SAMPLES] = getSampleVolume();
        container[PROPERTY_AUDIO][PROPERTY_MUSIC] = getMusicVolume();
        container[PROPERTY_FPS] = isFpsDisplayEnabled();
        container[PROPERTY_SCRIPTING] = Json::Value(Json::objectValue);
        container[PROPERTY_SCRIPTING][PROPERTY_SCRIPTING_STACKSIZE] = getScriptingStackSize();
        
        // Write all of the keybindings to an object and then attach it
        Json::Value keybindingObject(Json::objectValue);

        std::for_each(
            std::begin(keybindings),
            std::end(keybindings),
            [&](ClientConfig::StringMap::value_type & entry) {
                const std::string & key = entry.first;
                const std::string & value = entry.second;
                keybindingObject[key] = value;
            }
        );

        container[PROPERTY_BINDINGS] = keybindingObject;

        return writer.write(container);
    }

} // hikari
