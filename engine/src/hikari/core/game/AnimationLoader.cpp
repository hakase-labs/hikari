#include <hikari/core/game/AnimationLoader.hpp>
#include <hikari/core/game/Animation.hpp>
#include <hikari/core/game/AnimationFrame.hpp>
#include <hikari/core/game/AnimationSet.hpp>
#include <hikari/core/geom/Rectangle2D.hpp>
#include <hikari/core/util/StringUtils.hpp>
#include <hikari/core/util/JsonUtils.hpp>
#include <hikari/core/util/PhysFS.hpp>
#include <json/value.h>
#include <exception>
#include <fstream>

namespace hikari {

    const char* AnimationLoader::PROPERTY_NAME = "name";
    const char* AnimationLoader::PROPERTY_ALIASES = "aliases";
    const char* AnimationLoader::PROPERTY_IMAGE_FILE_NAME = "image";
    const char* AnimationLoader::PROPERTY_ANIMATIONS = "animations";
    const char* AnimationLoader::PROPERTY_REPEAT = "repeat";
    const char* AnimationLoader::PROPERTY_KEYFRAME = "keyframe";
    const char* AnimationLoader::PROPERTY_SYNCGROUP = "syncGroup";
    const char* AnimationLoader::PROPERTY_FRAMES = "frames";
    const char* AnimationLoader::PROPERTY_FRAME_X = "x";
    const char* AnimationLoader::PROPERTY_FRAME_Y = "y";
    const char* AnimationLoader::PROPERTY_FRAME_WIDTH = "width";
    const char* AnimationLoader::PROPERTY_FRAME_HEIGHT = "height";
    const char* AnimationLoader::PROPERTY_FRAME_LENGTH = "length";
    const char* AnimationLoader::PROPERTY_FRAME_HOTSPOT_X = "hotspotX";
    const char* AnimationLoader::PROPERTY_FRAME_HOTSPOT_Y = "hotspotY";

    AnimationLoader::AnimationLoader() {

    }
    
    std::shared_ptr<Animation> AnimationLoader::load(const std::string &fileName) {
        if(PhysFS::exists(fileName)) {
            Json::Value root = JsonUtils::loadJson(fileName);
            return loadFromJson(root);
        } else {
            throw std::exception("Animation loading failed. The file cannot be found.");
        }
    }

    std::shared_ptr<AnimationSet> AnimationLoader::loadSet(const std::string &fileName) {
        if(PhysFS::exists(fileName)) {
            Json::Value root = JsonUtils::loadJson(fileName); 
            
            // Extract name and image file path
            std::string name = root[PROPERTY_NAME].asString();
            std::string imageFileName = root[PROPERTY_IMAGE_FILE_NAME].asString();

            std::shared_ptr<AnimationSet> resultSet = 
                std::shared_ptr<AnimationSet>(new AnimationSet(name, imageFileName));

            // Extract animations
            const Json::Value animations = root[PROPERTY_ANIMATIONS];
            const Json::Value::Members animationNames = animations.getMemberNames();
            
            for(int i = 0, length = animationNames.size(); i < length; i++) {
                const std::string animationName = animationNames.at(i);
                const Json::Value animationJson = animations[animationName];

                if(animationJson.isObject()) {
                    std::shared_ptr<Animation> animation = loadFromJson(animationJson);

                    if(animation) {
                        // TODO: Check to see if any of these fail
                        resultSet->add(animationName, animation);

                        // Handle name aliasing if that propety is supplied
                        auto aliases = animationJson[PROPERTY_ALIASES];

                        if(!aliases.isNull() && aliases.isArray()) {
                            for(auto alias = aliases.begin(), end = aliases.end(); alias != end; alias++) {
                                auto aliasName = (*alias).asString();
                                resultSet->add(aliasName, animation);
                            }
                        } else {
                            // TODO: Log this error as an error; this is not critical it just means the JSON is not to spec.
                        }
                    }
                }
            }

            return resultSet;
        } else {
            throw std::exception("Animation loading failed. The file cannot be found.");
        }
    }

    std::shared_ptr<Animation> AnimationLoader::loadFromJsonObject(const Json::Value &json) {
        return loadFromJson(json);
    }

    std::shared_ptr<Animation> AnimationLoader::loadFromJson(const Json::Value &json) {
        //Json::Value root = JsonUtils::loadJson(fileName);

        if(!json.isNull()) {

            bool repeat = json.get(PROPERTY_REPEAT, false).asBool();

            unsigned int keyframe = json.get(
                PROPERTY_KEYFRAME, 
                Animation::ANIMATION_BEGINNING_FRAME_INDEX
            ).asInt();

            unsigned int syncGroup = json.get(
                PROPERTY_SYNCGROUP, 
                Animation::ANIMATION_DEFAULT_SYNC_GROUP
            ).asInt();

            Json::Value jsonFrames = json[PROPERTY_FRAMES];

            unsigned int numFrames = jsonFrames.size();

            if(numFrames > 0) {
                FrameList frames;
                for(unsigned int i = 0; i < numFrames; ++i) {
                    const Json::Value jsonFrame = jsonFrames[i];

                    Rectangle2D<int> rectangle;
                    rectangle.setX(jsonFrame.get(PROPERTY_FRAME_X, 0).asInt());
                    rectangle.setY(jsonFrame.get(PROPERTY_FRAME_Y, 0).asInt());
                    rectangle.setWidth(jsonFrame.get(PROPERTY_FRAME_WIDTH, 0).asInt());
                    rectangle.setHeight(jsonFrame.get(PROPERTY_FRAME_HEIGHT, 0).asInt());

                    float length = static_cast<float>
                        (jsonFrame.get(PROPERTY_FRAME_LENGTH, 0.0f).asDouble());

                    Point2D<int> hotspot(
                        jsonFrame.get(PROPERTY_FRAME_HOTSPOT_X, 0).asInt(), 
                        jsonFrame.get(PROPERTY_FRAME_HOTSPOT_Y, 0).asInt()
                    );

                    frames.push_back(Frame(rectangle, length, hotspot));
                }                

                return std::shared_ptr<Animation>(new Animation(frames, repeat, keyframe, syncGroup));
            } else {
                throw std::exception("Problem loading animation from JSON: the animation has no frames.");
            }
        } else {
            std::stringstream ss;
            ss << "Problem while parsing animation data from JSON.\n";

            throw std::exception(ss.str().c_str());
        }
    }
    /*
    std::shared_ptr<Animation> AnimationLoader::loadFromJsonObject(const Json::Value &json) {
        bool repeat = json.get(PROPERTY_REPEAT, false).asBool();

        unsigned int keyframe = json.get(
            PROPERTY_KEYFRAME, 
            Animation::ANIMATION_BEGINNING_FRAME_INDEX
        ).asInt();

        unsigned int syncGroup = json.get(
            PROPERTY_SYNCGROUP, 
            Animation::ANIMATION_DEFAULT_SYNC_GROUP
        ).asInt();

        unsigned int numFrames = json[PROPERTY_FRAMES].size();

        if(numFrames > 0) {
            FrameList frames;
            for(unsigned int i = 0; i < numFrames; ++i) {
                const Json::Value jsonFrame = json[PROPERTY_FRAMES][i];

                Rectangle2D<int> rectangle;
                rectangle.setX(jsonFrame.get(PROPERTY_FRAME_X, 0).asInt());
                rectangle.setY(jsonFrame.get(PROPERTY_FRAME_Y, 0).asInt());
                rectangle.setWidth(jsonFrame.get(PROPERTY_FRAME_WIDTH, 0).asInt());
                rectangle.setHeight(jsonFrame.get(PROPERTY_FRAME_HEIGHT, 0).asInt());

                float length = static_cast<float>
                    (jsonFrame.get(PROPERTY_FRAME_LENGTH, 0.0f).asDouble());

                Point2D<int> hotspot(
                    jsonFrame.get(PROPERTY_FRAME_HOTSPOT_X, 0).asInt(), 
                    jsonFrame.get(PROPERTY_FRAME_HOTSPOT_Y, 0).asInt()
                );

                frames.push_back(Frame(rectangle, length, hotspot));
            }                

            return std::shared_ptr<Animation>
                (new Animation(frames, repeat, keyframe, syncGroup));
        } else {
            throw std::exception("Problem loading animation from JSON; the animation has no frames.");
        }

        return std::shared_ptr<Animation>();
    }
    */

} // hikari