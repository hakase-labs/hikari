#include "hikari/client/Services.hpp"
#include "hikari/client/game/objects/FactoryHelpers.hpp"
#include "hikari/client/game/objects/EnemyFactory.hpp"
#include "hikari/client/game/objects/ItemFactory.hpp"
#include "hikari/client/game/objects/ProjectileFactory.hpp"
#include "hikari/client/game/objects/GameObject.hpp"
#include "hikari/client/game/objects/CollectableItem.hpp"
#include "hikari/client/game/objects/Enemy.hpp"
#include "hikari/client/game/objects/Projectile.hpp"
#include "hikari/client/game/Effect.hpp"
#include "hikari/client/game/objects/effects/NothingEffect.hpp"
#include "hikari/client/game/objects/effects/ScriptedEffect.hpp"
#include "hikari/client/game/objects/EnemyBrain.hpp"
#include "hikari/client/game/objects/brains/ScriptedEnemyBrain.hpp"
#include "hikari/client/scripting/SquirrelService.hpp"

#include "hikari/core/game/map/Room.hpp"
#include "hikari/core/geom/BoundingBox.hpp"
#include "hikari/core/util/AnimationSetCache.hpp"
#include "hikari/core/util/FileSystem.hpp"
#include "hikari/core/util/ImageCache.hpp"
#include "hikari/core/util/ServiceLocator.hpp"
#include "hikari/core/util/Log.hpp"
#include "hikari/core/util/exception/HikariException.hpp"

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
                        throw HikariException("Cannot populate CollectableItemFactory because ItemFactory is null.");
                    }
                } else {
                    // AnimationSetCache is borked!
                    throw HikariException("Cannot populate CollectableItemFactory because AnimationSetCache is null.");
                }
            } else {
                // SquirrelService is borked!
                throw HikariException("Cannot populate CollectableItemFactory because SquirrelService is null.");
            }
        } else {
            // ImageCache is borked!
            throw HikariException("Cannot populate CollectableItemFactory because ImageCache is null.");
        }
    }

    void populateEnemyFactory(
        const std::string & descriptorFilePath,
        const std::weak_ptr<hikari::EnemyFactory> & factory,
        ServiceLocator & services
    ) {
        auto imageCachePtr        = services.locateService<ImageCache>(Services::IMAGECACHE);
        auto squirrelPtr          = services.locateService<SquirrelService>(Services::SCRIPTING);
        auto animationSetCachePtr = services.locateService<AnimationSetCache>(Services::ANIMATIONSETCACHE);
        
        if(auto imageCache = imageCachePtr.lock()) {
            if(auto squirrel = squirrelPtr.lock()) {
                if(auto animationSetCache = animationSetCachePtr.lock()) {
                    if(auto factoryPtr = factory.lock()) {

                        HIKARI_LOG(debug) << "Populating enemy factory...";

                        if(FileSystem::exists(descriptorFilePath)) {
                            auto fileContents = FileSystem::openFile(descriptorFilePath);
                            Json::Value root;
                            Json::Reader reader;

                            if(reader.parse(*fileContents, root, false)) {
                                auto templateCount = root.size();

                                if(templateCount > 0) {
                                    for(decltype(templateCount) i = 0; i < templateCount; ++i) {
                                        const auto & templateObject = root[i];

                                        const auto name              = templateObject["name"].asString();
                                        const auto behavior          = templateObject["behavior"];
                                        const auto animationSet      = templateObject["animationSet"].asString();
                                        const auto boundingBoxObject = templateObject["boundingBox"];
                                        const auto statesObject      = templateObject["states"];
                                        const auto characteristicsObject = templateObject["characteristics"];

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

                                        std::shared_ptr<EnemyBrain> brain(nullptr);

                                        const auto behaviorType = behavior["type"].asString();

                                        if(behaviorType == "scripted") {
                                            const auto behaviorName = behavior["name"].asString();
                                            brain = std::make_shared<ScriptedEnemyBrain>(*squirrel, behaviorName);
                                        } else {
                                            // Some other built-in behvior; currently not supported.
                                        }

                                        auto animationSetPtr = animationSetCache->get(animationSet);
                                        auto spriteTexture = imageCache->get(animationSetPtr->getImageFileName());

                                        auto instance = std::make_shared<Enemy>(GameObject::generateObjectId(), nullptr);
                                        instance->setAnimationSet(animationSetPtr);
                                        instance->setSpriteTexture(spriteTexture);
                                        instance->setBoundingBox(boundingBox);
                                        instance->setDirection(Directions::Down);
                                        instance->setCurrentAnimation(animationSetPtr->get("idle"));

                                        if(!characteristicsObject.isNull()) {
                                            const bool gravitated = characteristicsObject["gravitated"].asBool();
                                            const bool phasing = characteristicsObject["phasing"].asBool();

                                            instance->setGravitated(gravitated);
                                            instance->setPhasing(phasing);
                                        }

                                        instance->setBrain(brain);
                                        instance->setActive(true);

                                        factoryPtr->registerPrototype(name, instance);
                                        HIKARI_LOG(debug2) << "Registered prototype for \"" << name << "\" enemy.";
                                    }
                                } else {
                                    HIKARI_LOG(debug3) << "No enemy templates found.";
                                }
                            }
                        } else {
                            HIKARI_LOG(debug3) << "Can't find enemy descriptor file: \"" << descriptorFilePath << "\"";
                        }
                    } else {
                        // ItemFactory is borked!
                        throw HikariException("Cannot populate CollectableItemFactory because ItemFactory is null.");
                    }
                } else {
                    // AnimationSetCache is borked!
                    throw HikariException("Cannot populate CollectableItemFactory because AnimationSetCache is null.");
                }
            } else {
                // SquirrelService is borked!
                throw HikariException("Cannot populate CollectableItemFactory because SquirrelService is null.");
            }
        } else {
            // ImageCache is borked!
            throw HikariException("Cannot populate CollectableItemFactory because ImageCache is null.");
        }
    }

    void populateProjectileFactory(
        const std::string & descriptorFilePath,
        const std::weak_ptr<hikari::ProjectileFactory> & factory,
        ServiceLocator & services
    ) {
        auto imageCachePtr        = services.locateService<ImageCache>(Services::IMAGECACHE);
        auto squirrelPtr          = services.locateService<SquirrelService>(Services::SCRIPTING);
        auto animationSetCachePtr = services.locateService<AnimationSetCache>(Services::ANIMATIONSETCACHE);
        
        if(auto imageCache = imageCachePtr.lock()) {
            if(auto squirrel = squirrelPtr.lock()) {
                if(auto animationSetCache = animationSetCachePtr.lock()) {
                    if(auto factoryPtr = factory.lock()) {

                        HIKARI_LOG(debug) << "Populating projectile factory...";

                        if(FileSystem::exists(descriptorFilePath)) {
                            auto fileContents = FileSystem::openFile(descriptorFilePath);
                            Json::Value root;
                            Json::Reader reader;

                            if(reader.parse(*fileContents, root, false)) {
                                auto templateCount = root.size();

                                if(templateCount > 0) {
                                    HIKARI_LOG(debug3) << "Found " << templateCount << " projectile template(s).";

                                    for(decltype(templateCount) i = 0; i < templateCount; ++i) {
                                        const auto & templateObject = root[i];

                                        // TODO: Create instances
                                        const auto name              = templateObject["name"].asString();
                                        
                                        auto instance = std::make_shared<hikari::Projectile>();

                                        factoryPtr->registerPrototype(name, instance);
                                    }
                                } else {
                                    HIKARI_LOG(debug3) << "No projectile templates found.";
                                }
                            }
                        } else {
                            HIKARI_LOG(debug3) << "Can't find projectile descriptor file: \"" << descriptorFilePath << "\"";
                        }
                    } else {
                        // ItemFactory is borked!
                        throw HikariException("Cannot populate ProjectileFactory because ProjectileFactory is null.");
                    }
                } else {
                    // AnimationSetCache is borked!
                    throw HikariException("Cannot populate ProjectileFactory because AnimationSetCache is null.");
                }
            } else {
                // SquirrelService is borked!
                throw HikariException("Cannot populate ProjectileFactory because SquirrelService is null.");
            }
        } else {
            // ImageCache is borked!
            throw HikariException("Cannot populate ProjectileFactory because ImageCache is null.");
        }
    }
} // hikari::FactoryHelpers
} // hikari