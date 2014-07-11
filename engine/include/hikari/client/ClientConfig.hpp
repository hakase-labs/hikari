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
        static const char* PROPERTY_AUDIO;
        static const char* PROPERTY_SAMPLES;
        static const char* PROPERTY_MUSIC;
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
        float musicVolume;
        float sampleVolume;
        std::string videoMode;
        std::map<std::string, std::string> keybindings;

        void extractValuesFromJson(const Json::Value& configJson);
    public:
        typedef std::map<std::string, std::string> StringMap;

        static const char* VIDEO_SCALE_1X;
        static const char* VIDEO_SCALE_2X;
        static const char* VIDEO_SCALE_3X;
        static const char* VIDEO_SCALE_FULL;

        ClientConfig();
        ClientConfig(const Json::Value& configJson);

        bool isVsyncEnabled() const;
        void setVsyncEnabled(bool enabled);

        bool isFpsDisplayEnabled() const;
        void setFpsDisplayEnabled(bool enabled);

        float getMusicVolume() const;
        void setMusicVolume(float volume);

        float getSampleVolume() const;
        void setSampleVolume(float volume);

        unsigned int getScriptingStackSize() const;

        std::string getVideoMode() const;
        void setVideoMode(const std::string & mode);

        StringMap getKeyBindings() const;
        void setKeyBindings(const StringMap & bindings);

        /**
         * Serializes the configuration options into a string.
         *
         * @return string representation of the config
         */
        std::string serialize();
    };

} // hikari

#endif // HIKARI_CLIENT_CLIENTCONFIG
