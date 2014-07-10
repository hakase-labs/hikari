#ifndef HIKARI_CLIENT_GAME_STAGESELECTSTATECONFIG
#define HIKARI_CLIENT_GAME_STAGESELECTSTATECONFIG

#include "hikari/core/geom/Point2D.hpp"

#include <memory>
#include <string>
#include <vector>

namespace Json {
    class Value;
}

namespace hikari {

    class StageSelectStateConfig {
    public:
        typedef std::pair<Point2D<float>, Point2D<float>> PointPairF;
        typedef std::vector<Point2D<float>> PointListF;
        typedef std::vector<PointPairF> PointPairListF;
        typedef std::pair<std::string, std::string> StringPair;

        struct PortraitInfo {
            std::string label;
            std::string introLabel;
            std::string animationSet;
            std::string animationName;
        };

    private:
        std::string musicName;
        std::string backgroundImage;
        std::string foregroundImage;
        StringPair cursorInfo; // first = animation set name, second = animation name
        StringPair eyeInfo;    // first = animation set name, second = animation name
        PointPairListF eyePositions;
        PointListF cursorPositions;
        std::vector<PortraitInfo> portraits;

        void extractValuesFromJson(const Json::Value & config);
        std::vector<PortraitInfo> extractPortraits(const Json::Value & config);
    public:
        explicit StageSelectStateConfig(const Json::Value & config);

        const std::string & getMusicName() const;
        const std::string & getBackground() const;
        const std::string & getForeground() const;
        const StringPair & getCursorInfo() const;
        const StringPair & getEyeInfo() const;
        const PointPairListF & getEyePositions() const;
        const PointListF & getCursorPositions() const;
        const std::vector<PortraitInfo> & getPortraits() const;
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_STAGESELECTSTATECONFIG
