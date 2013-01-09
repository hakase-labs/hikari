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

        bool enableVsync;
        bool enableFpsDisplay;

        void extractValuesFromJson(const Json::Value& configJson);
    public:
        ClientConfig(const Json::Value& configJson);

        const bool isVsyncEnabled() const;
        const bool isFpsDisplayEnabled() const;
    };

} // hikari

#endif // HIKARI_CLIENT_CLIENTCONFIG
