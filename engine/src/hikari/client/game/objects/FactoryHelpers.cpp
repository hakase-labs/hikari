#include "hikari/client/Services.hpp"
#include "hikari/client/game/objects/FactoryHelpers.hpp"
#include "hikari/client/game/objects/ItemFactory.hpp"
#include "hikari/client/game/objects/GameObject.hpp"
#include "hikari/client/game/objects/CollectableItem.hpp"
#include "hikari/client/game/Effect.hpp"
#include "hikari/client/game/objects/effects/NothingEffect.hpp"
#include "hikari/client/game/objects/effects/ScriptedEffect.hpp"
#include "hikari/client/scripting/SquirrelService.hpp"

#include "hikari/core/game/map/Room.hpp"
#include "hikari/core/geom/BoundingBox.hpp"
#include "hikari/core/util/AnimationSetCache.hpp"
#include "hikari/core/util/FileSystem.hpp"
#include "hikari/core/util/ImageCache.hpp"
#include "hikari/core/util/ServiceLocator.hpp"
#include "hikari/core/util/Log.hpp"

#include <sqrat.h>
#include <json/reader.h>

#include <iterator>

namespace hikari {
namespace FactoryHelpers {

    void populateCollectableItemFactory(
        const std::string & descriptorFilePath,
        const std::weak_ptr<hikari::ItemFactory> & factory,
        ServiceLocator & services
    ) {
        auto imageCachePtr        = services.locateService<ImageCache>(Services::IMAGECACHE);
        auto squirrelPtr          = services.locateService<SquirrelService>(Services::SCRIPTING);
        auto animationSetCachePtr = services.locateService<AnimationSetCache>(Services::ANIMATIONSETCACHE);
        
        if(auto imageCache = imageCachePtr.lock()) {
            if(auto squirrel = squirrelPtr.lock()) {
                if(auto animationSetCache = animationSetCachePtr.lock()) {
                    if(auto factoryPtr = factory.lock()) {

                        HIKARI_LOG(debug) << "Populating item factory...";

                        auto fileContents = FileSystem::openFile(descriptorFilePath);
                        Json::Value root;
                        Json::Reader reader;

                        if(reader.parse(*fileContents, root, false)) {
                            auto templateCount = root.size();

                            for(decltype(templateCount) i = 0; i < templateCount; ++i) {
                                const auto & templateObject = root[i];

                                const auto name              = templateObject["name"].asString();
                                const auto effect            = templateObject["effect"].asString();
                                const auto effectConfig      = templateObject["effectConfig"];
                                const auto animationSet      = templateObject["animationSet"].asString();
                                const auto animationName     = templateObject["animationName"].asString();
                                const auto boundingBoxObject = templateObject["boundingBox"];
                                const auto ageless           = templateObject["ageless"].asBool();
                                const auto maximumAge        = templateObject["maximumAge"].asDouble();

                                hikari::BoundingBoxF boundingBox(
                                    0.0f,
                                    0.0f,
                                    static_cast<float>(boundingBoxObject["width"].asDouble()),
                                    static_cast<float>(boundingBoxObject["height"].asDouble())
                                );

                                boundingBox.setOrigin(
                                    static_cast<float>(boundingBoxObject["originX"].asDouble()),
                                    static_cast<float>(boundingBoxObject["originY"].asDouble())
                                );

                                std::shared_ptr<Effect> effectInstance(nullptr);

                                if(effect == "" || effect == "NothingEffect") {
                                    effectInstance.reset(new NothingEffect());
                                } else {
                                    try {
                                        Sqrat::Table configTable;

                                        if(!effectConfig.isNull()) {
                                            const auto configPropertyNames = effectConfig.getMemberNames();

                                            for(auto propName = std::begin(configPropertyNames); propName != std::end(configPropertyNames); std::advance(propName, 1)) {
                                                const auto propValue = effectConfig.get(*propName, Json::Value::null);

                                                if(propValue.isBool()) {
                                                    configTable.SetValue((*propName).c_str(), propValue.asBool());
                                                } else if(propValue.isDouble()) {
                                                    configTable.SetValue((*propName).c_str(), propValue.asDouble());
                                                } else if(propValue.isIntegral()) {
                                                    configTable.SetValue((*propName).c_str(), propValue.asInt());
                                                } else if(propValue.isString()) {
                                                    configTable.SetValue((*propName).c_str(), propValue.asString());
                                                } else if(propValue.isNull()) {
                                                    configTable.SetValue((*propName).c_str(), nullptr);
                                                }
                                            }
                                        }

                                        effectInstance.reset(new ScriptedEffect(*squirrel, effect, configTable));
                                    } catch(std::runtime_error & ex) {
                                        HIKARI_LOG(error) << "Couldn't create scripted effect of type " << effect << ". Falling back to NothingEffect. The exception was: " << ex.what();
                                        effectInstance.reset(new NothingEffect());
                                    }
                                }

                                auto item = std::make_shared<CollectableItem>(GameObject::generateObjectId(), nullptr, effectInstance);

                                auto animationSetPtr = animationSetCache->get(animationSet);
                                auto spriteTexture = imageCache->get(animationSetPtr->getImageFileName());
                                item->setAnimationSet(animationSetPtr);
                                item->setSpriteTexture(spriteTexture);
                                item->changeAnimation(animationName);
                                item->setBoundingBox(boundingBox);
                                item->setAgeless(ageless);
                                item->setMaximumAge(static_cast<float>(maximumAge));

                                factoryPtr->registerPrototype(name, item);
                            }
                        }
                    } else {
                        // ItemFactory is borked!
                    }
                } else {
                    // AnimationSetCache is borked!
                }
            } else {
                // SquirrelService is borked!
            }
        } else {
            // ImageCache is borked!
        }
    }

} // hikari::FactoryHelpers
} // hikari