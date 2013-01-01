#ifndef HIKARI_CORE_GAME_ANIMATIONLOADER
#define HIKARI_CORE_GAME_ANIMATIONLOADER

#include "hikari/core/Platform.hpp"
#include <memory>
#include <string>

namespace Json {
    class Value;
}

namespace hikari {

    class Animation;
    class AnimationSet;

    class HIKARI_API AnimationLoader {
    public:
        static std::shared_ptr<Animation> load(const std::string &fileName);
        static std::shared_ptr<AnimationSet> loadSet(const std::string &fileName);
        AnimationLoader();
        std::shared_ptr<Animation> loadFromJsonObject(const Json::Value &json);
    private:
        static const char* PROPERTY_NAME;
        static const char* PROPERTY_ALIASES;
        static const char* PROPERTY_IMAGE_FILE_NAME;
        static const char* PROPERTY_ANIMATIONS;
        static const char* PROPERTY_REPEAT;
        static const char* PROPERTY_KEYFRAME;
        static const char* PROPERTY_SYNCGROUP;
        static const char* PROPERTY_FRAMES;
        static const char* PROPERTY_FRAME_X;
        static const char* PROPERTY_FRAME_Y;
        static const char* PROPERTY_FRAME_WIDTH;
        static const char* PROPERTY_FRAME_HEIGHT;
        static const char* PROPERTY_FRAME_LENGTH;
        static const char* PROPERTY_FRAME_HOTSPOT_X;
        static const char* PROPERTY_FRAME_HOTSPOT_Y;
        //static std::shared_ptr<Animation> loadFromJSON(const std::string &fileName);
        static std::shared_ptr<Animation> loadFromJson(const Json::Value &json);
    };
    
} // hikari

#endif // HIKARI_CORE_GAME_ANIMATIONLOADER