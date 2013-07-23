#ifndef HIKARI_CLIENT_CLIENTCONFIG
#define HIKARI_CLIENT_CLIENTCONFIG

#include <map>
#include <string>

namespace Json {
    class Value;
}

namespace hikari {

    class ClientConfig {
    private:
        static const char* PROPERTY_VSYNC;
        static const char* PROPERTY_FPS;
        static const char* PROPERTY_SCRIPTING;
        static const char* PROPERTY_SCRIPTING_STACKSIZE;
        static const char* PROPERTY_VIDEOMODE;
        static const char* PROPERTY_BINDINGS;
        static const char* PROPERTY_KEYBOARD_BINDINGS;

        bool enableVsync;
        bool enableFpsDisplay;
        unsigned int stackSize;
        std::string videoMode;
        std::map<std::string, std::string> keybindings;

        void extractValuesFromJson(const Json::Value& configJson);
    public:
        ClientConfig();
        ClientConfig(const Json::Value& configJson);

        bool isVsyncEnabled() const;
        void setVsyncEnabled(bool enabled);

        bool isFpsDisplayEnabled() const;
        void setFpsDisplayEnabled(bool enabled);

        unsigned int getScriptingStackSize() const;

        std::string getVideoMode() const;
        void setVideoMode(const std::string & mode);

        std::map<std::string, std::string> getKeyBindings() const;
        void setKeyBindings(const std::map<std::string, std::string> & bindings);

        /**
         * Serializes the configuration options into a string.
         *
         * @return string representation of the config
         */
        std::string serialize();
    };

} // hikari

#endif // HIKARI_CLIENT_CLIENTCONFIG
