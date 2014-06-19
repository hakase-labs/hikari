#include "hikari/core/util/Log.hpp"
#include "hikari/client/game/StageSelectStateConfig.hpp"
#include <json/value.h>

namespace hikari {

    StageSelectStateConfig::StageSelectStateConfig(const Json::Value & config) {
        extractValuesFromJson(config);
    }

    void StageSelectStateConfig::extractValuesFromJson(const Json::Value & config) {
        if(!config.isNull()) {

            musicName = config.get("music", "None").asString();
            backgroundImage = config.get("background", "None").asString();
            foregroundImage = config.get("foreground", "None").asString();

            // Extract portraits
            portraits = extractPortraits(config.get("portraits", Json::Value()));

            // Extract sprites
            if(config.isMember("sprites")) {
                const auto & spritesJson = config.get("sprites", Json::Value());

                if(spritesJson.isMember("cursor")) {
                    const auto & cursorSpriteJson = spritesJson.get("cursor", Json::Value());

                    cursorInfo.first = cursorSpriteJson.get("animationSet", "").asString();
                    cursorInfo.second = cursorSpriteJson.get("animationName", "").asString();
                }

                if(spritesJson.isMember("eye")) {
                    const auto & eyeSpriteJson = spritesJson.get("eye", Json::Value());

                    eyeInfo.first = eyeSpriteJson.get("animationSet", "").asString();
                    eyeInfo.second = eyeSpriteJson.get("animationName", "").asString();
                }
            }

            // Extract eye positions
            if(config.isMember("eyePositions")) {
                const auto & positionsJson = config.get("eyePositions", Json::Value());

                for(unsigned int i = 0, length = positionsJson.size(); i < length; ++i) {
                    const auto & positionPairJson = positionsJson[i]; // an array of two objects
                    const auto & positionRightJson = positionPairJson[0u]; // the first object is the right eye
                    const auto & positionLeftJson = positionPairJson[1u]; // the second object is the left eye

                    float rightEyeX = positionRightJson.get("x", 0).asFloat();
                    float rightEyeY = positionRightJson.get("y", 0).asFloat();
                    float leftEyeX = positionLeftJson.get("x", 0).asFloat();
                    float leftEyeY = positionLeftJson.get("y", 0).asFloat();

                    Point2D<float> rightEyePosition(rightEyeX, rightEyeY);
                    Point2D<float> leftEyePosition(leftEyeX, leftEyeY);

                    eyePositions.push_back(std::make_pair(rightEyePosition, leftEyePosition));
                }
            }

            HIKARI_LOG(debug4) << "Found " << portraits.size() << " portrait(s).";
        }
    }

    std::vector<StageSelectStateConfig::PortraitInfo> StageSelectStateConfig::extractPortraits(const Json::Value & config) {
        std::vector<StageSelectStateConfig::PortraitInfo> portraits;

        std::size_t protraitCount = config.size();

        for(std::size_t i = 0; i < protraitCount; ++i) {
            const auto & portraitJson = config[i];
            PortraitInfo info;

            info.label = portraitJson.get("label", "").asString();
            info.introLabel = portraitJson.get("introLabel", "").asString();
            info.animationSet = portraitJson.get("animationSet", "").asString();
            info.animationName = portraitJson.get("animationName", "").asString();
            portraits.push_back(info);
        }

        return portraits;
    }

    const std::string & StageSelectStateConfig::getMusicName() const {
        return musicName;
    }

    const std::string & StageSelectStateConfig::getBackground() const {
        return backgroundImage;
    }

    const std::string & StageSelectStateConfig::getForeground() const {
        return foregroundImage;
    }

    const StageSelectStateConfig::StringPair & StageSelectStateConfig::getCursorInfo() const {
        return cursorInfo;
    }

    const StageSelectStateConfig::StringPair & StageSelectStateConfig::getEyeInfo() const {
        return eyeInfo;
    }

    const StageSelectStateConfig::PointPairListF & StageSelectStateConfig::getEyePositions() const {
        return eyePositions;
    }

    const StageSelectStateConfig::PointListF & StageSelectStateConfig::getCursorPositions() const {
        return cursorPositions;
    }

    const std::vector<StageSelectStateConfig::PortraitInfo> & StageSelectStateConfig::getPortraits() const {
        return portraits;
    }


} // hikari
