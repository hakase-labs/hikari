#ifndef HIKARI_CLIENT_CLIENTCONFIG
#define HIKARI_CLIENT_CLIENTCONFIG

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

        bool enableVsync;
        bool enableFpsDisplay;
        unsigned int stackSize;

        void extractValuesFromJson(const Json::Value& configJson);
    public:
        ClientConfig();
        ClientConfig(const Json::Value& configJson);

        const bool isVsyncEnabled() const;
        const bool isFpsDisplayEnabled() const;
        const unsigned int getScriptingStackSize() const;
    };

} // hikari

#endif // HIKARI_CLIENT_CLIENTCONFIG
