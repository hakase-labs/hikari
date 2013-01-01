#include <hikari/core/game/map/TilesetLoader.hpp>
#include <hikari/core/game/map/Tileset.hpp>
#include <hikari/core/game/TileAnimator.hpp>
#include <hikari/core/game/AnimationLoader.hpp>
#include <hikari/core/util/ImageCache.hpp>

#include <hikari/core/util/Log.hpp>

#include <json/reader.h>
#include <json/value.h>

#include <stdexcept>

namespace hikari {

    const char* TilesetLoader::PROPERTY_NAME_SURFACE = "surface";
    const char* TilesetLoader::PROPERTY_NAME_SIZE = "size";
    const char* TilesetLoader::PROPERTY_NAME_TILES = "tiles";
    const char* TilesetLoader::PROPERTY_NAME_X = "x";
    const char* TilesetLoader::PROPERTY_NAME_Y = "y";
    const char* TilesetLoader::PROPERTY_NAME_ANIMATION = "animation";

    TilesetLoader::TilesetLoader(const std::shared_ptr<ImageCache> &imageCache,
            const std::shared_ptr<AnimationLoader> &animationLoader)
        : imageCache(imageCache)
        , animationLoader(animationLoader) {

    }

    TileDataPtr TilesetLoader::loadFromJson(const Json::Value &json) {
        isValidTilesetJson(json);
        return constructTileset(json);
    }

    bool TilesetLoader::isValidTilesetJson(const Json::Value &json) const {
        bool isValid = true;

        // Root must be an object
        if(!json.isObject()) {
            isValid = false;
        }

        // Root must contain a string property called "surface"
        if(!json[PROPERTY_NAME_SURFACE].isString()) {
            isValid = false;
        }

        // Root must contain a number property called "size"
        if(!json[PROPERTY_NAME_SIZE].isNumeric()) {
            isValid = false;
        }

        // Root must contain an array property called "tiles"
        if(!json[PROPERTY_NAME_TILES].isArray()) {
            isValid = false;
        }

        if(!isValid) {
            throw std::runtime_error("JSON structure is not a valid Tileset.");
        }

        return isValid;
    }

    bool TilesetLoader::isValidTileJson(const Json::Value &json) const {
        bool isValid = true;

        // Must be an object
        if(!json.isObject()) {
            isValid = false;
        }

        // Must contain an int property called "x"
        if(!json["x"].isInt()) {
            isValid = false;
        }

        // Must contain an int property called "y"
        if(!json["y"].isInt()) {
            isValid = false;
        }

        return isValid;
    }

    bool TilesetLoader::isTileAnimated(const Json::Value &json) const {
        return json.isMember(PROPERTY_NAME_ANIMATION) 
                && json[PROPERTY_NAME_ANIMATION].isObject();
    }

    TileDataPtr TilesetLoader::constructTileset(const Json::Value &json) const {
        std::string surfaceName(json[PROPERTY_NAME_SURFACE].asString());
        int tileSize = json[PROPERTY_NAME_SIZE].asInt();
        int numberOfTiles = json[PROPERTY_NAME_TILES].size();

        std::vector<sf::IntRect> tiles(numberOfTiles);
        std::vector<TileAnimator> tileAnimators;

        for(int i = 0; i < numberOfTiles; ++i) {
            const Json::Value &tileJson = json[PROPERTY_NAME_TILES][i];

            if(isValidTileJson(tileJson)) {
                tiles.at(i) = sf::IntRect(
                    tileJson[PROPERTY_NAME_X].asInt(), 
                    tileJson[PROPERTY_NAME_Y].asInt(), 
                    tileSize, 
                    tileSize
                );

                if(isTileAnimated(tileJson)) {
                    try {
                        std::shared_ptr<Animation> tileAnimation;
                        tileAnimation = animationLoader->loadFromJsonObject(tileJson[PROPERTY_NAME_ANIMATION]);

                        if(tileAnimation) {
                            TileAnimator animator(tiles, i);
                            animator.setAnimation(tileAnimation);
                            tileAnimators.push_back(animator);
                        } else {
                            HIKARI_LOG(debug) << "<TilesetLoader> ignoring invalid animation for tile index " << i << ".";
                        }
                    } catch(std::exception &loadException) {
                        HIKARI_LOG(debug) << "<TilesetLoader> failed to parse animation from tile index " << i << ". Exception: " << loadException.what();
                    } 
                }
            } else {
                HIKARI_LOG(debug) << "<TilesetLoader> ignoring invalid tile index " << i << ".";
            }
        }

        return TileDataPtr(
            new Tileset(
                imageCache->get(surfaceName), 
                tileSize, 
                tiles,
                tileAnimators
            )
        );
    }
} // hikari
