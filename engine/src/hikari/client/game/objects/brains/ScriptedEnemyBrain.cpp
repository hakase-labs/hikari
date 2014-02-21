#include "hikari/client/game/objects/brains/ScriptedEnemyBrain.hpp"
#include "hikari/client/scripting/SquirrelService.hpp"

#include "hikari/core/util/Log.hpp"

#include <algorithm>

namespace hikari {

    const char * ScriptedEnemyBrain::FUNCTION_NAME_ATTACH = "attachHost";
    const char * ScriptedEnemyBrain::FUNCTION_NAME_DETACH = "detachHost";
    const char * ScriptedEnemyBrain::FUNCTION_NAME_HANDLECOLLISION = "handleWorldCollision";
    const char * ScriptedEnemyBrain::FUNCTION_NAME_HANDLEOBJECTTOUCH = "handleObjectTouch";
    const char * ScriptedEnemyBrain::FUNCTION_NAME_UPDATE = "update";
    const char * ScriptedEnemyBrain::BASE_CLASS_NAME = "EnemyBehavior";

    ScriptedEnemyBrain::ScriptedEnemyBrain(SquirrelService& squirrel, const std::string& scriptClassName, const Sqrat::Table& config)
        : vm(squirrel.getVmInstance())
        , scriptClassName(scriptClassName)
        , instance()
        , instanceConfig(config)
    {
        HIKARI_LOG(debug4) << "constructor :: Is instanceConfig null? " << instanceConfig.IsNull();
        if(!bindScriptClassInstance()) {
            // throw?
            HIKARI_LOG(debug4) << "ScriptedEnemyBrain failed to bind.";
        }
    }

    ScriptedEnemyBrain::ScriptedEnemyBrain(const ScriptedEnemyBrain & proto)
        : vm(proto.vm)
        , scriptClassName(proto.scriptClassName)
        , instance()
        , instanceConfig(proto.instanceConfig)
    {
                HIKARI_LOG(debug4) << "constructor :: Is instanceConfig null? " << instanceConfig.IsNull();

        if(!bindScriptClassInstance()) {
            // throw?
            HIKARI_LOG(error) << "ScriptedEnemyBrain failed to bind.";
        }
    }
    
    ScriptedEnemyBrain::~ScriptedEnemyBrain() {

    }

    bool ScriptedEnemyBrain::bindScriptClassInstance() {
        bool isValid = true;

        if(scriptClassName.empty()) {
            isValid = false;
        } else {
            try {
                Sqrat::Function constructor(Sqrat::RootTable(vm), scriptClassName.c_str());

                if(!constructor.IsNull()) {
                    Sqrat::Object& configRef = instanceConfig;
                    HIKARI_LOG(debug3) << "bindScriptClassInstance :: Is instanceConfig null? " << instanceConfig.IsNull();
                    instance = constructor.Evaluate<Sqrat::Object>(configRef);

                    if(!instance.IsNull()) {
                        proxyAttach = Sqrat::Function(instance, FUNCTION_NAME_ATTACH);
                        proxyDetach = Sqrat::Function(instance, FUNCTION_NAME_DETACH);
                        proxyUpdate = Sqrat::Function(instance, FUNCTION_NAME_UPDATE);
                        proxyHandleWorldCollision = Sqrat::Function(instance, FUNCTION_NAME_HANDLECOLLISION);
                        proxyHandleObjectTouch = Sqrat::Function(instance, FUNCTION_NAME_HANDLEOBJECTTOUCH);
                    } else {
                        HIKARI_LOG(debug2) << "Constructor for '" << scriptClassName << "' did not return the correct object type.";
                    }
                } else {
                    HIKARI_LOG(debug2) << "Could not find a constructor for '" << scriptClassName << "'.";
                }
            } catch(Sqrat::Exception & squirrelException) {
                HIKARI_LOG(debug1) << "Could not create an instance of '" << scriptClassName << "'. Reason: " << squirrelException.Message();
            }
        }

        return isValid;
    }

    std::unique_ptr<EnemyBrain> ScriptedEnemyBrain::clone() const {
        return std::unique_ptr<EnemyBrain>(new ScriptedEnemyBrain(*this));
    }

    void ScriptedEnemyBrain::attach(Enemy* host) {
        EnemyBrain::attach(host);

        if(!proxyAttach.IsNull()) {
            proxyAttach.Execute(host);
        }
    }

    void ScriptedEnemyBrain::detach() {
        if(!proxyDetach.IsNull()) {
            proxyDetach.Execute();
        }
    }

    void ScriptedEnemyBrain::handleCollision(Movable& body, CollisionInfo& info) {
        if(info.isCollisionX) {
            proxyHandleWorldCollision.Execute(info.directionX);
        } else if(info.isCollisionY) {
            proxyHandleWorldCollision.Execute(info.directionY);
        }
    }

    void ScriptedEnemyBrain::handleObjectTouch(int otherId) {
        if(!proxyHandleObjectTouch.IsNull()) {
            proxyHandleObjectTouch.Execute(otherId);
        }
    }


    void ScriptedEnemyBrain::update(float dt) {
        if(!proxyUpdate.IsNull()) {
            proxyUpdate.Execute(dt);
        }
    }

} // hikari