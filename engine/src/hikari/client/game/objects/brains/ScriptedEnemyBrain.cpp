#include "hikari/client/game/objects/brains/ScriptedEnemyBrain.hpp"
#include "hikari/client/scripting/SquirrelService.hpp"

#include "hikari/core/util/Log.hpp"

#include <algorithm>

namespace hikari {

    ScriptedEnemyBrain::ScriptedEnemyBrain(SquirrelService& squirrel, const std::string& scriptClassName, const Sqrat::Table& config)
        : vm(squirrel.getVmInstance())
        , scriptClassName(scriptClassName)
        , instance()
        , instanceConfig(vm)
    {
        if(!bindScriptClassInstance()) {
            // throw?
            HIKARI_LOG(error) << "ScriptedEnemyBrain failed to bind.";
        }
    }

    ScriptedEnemyBrain::ScriptedEnemyBrain(const ScriptedEnemyBrain & proto)
        : vm(proto.vm)
        , scriptClassName(proto.scriptClassName)
        , instance()
        , instanceConfig(vm)
    {
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
                    instance = constructor.Evaluate<Sqrat::Object>(configRef);

                    if(!instance.IsNull()) {
                        proxyAttach = Sqrat::Function(instance, "attachHost");
                        proxyDetach = Sqrat::Function(instance, "detachHost");
                        proxyUpdate = Sqrat::Function(instance, "update");
                        proxyHandleWorldCollision = Sqrat::Function(instance, "handleWorldCollision");
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

    void ScriptedEnemyBrain::update(float dt) {
        if(!proxyUpdate.IsNull()) {
            proxyUpdate.Execute(dt);
        }
    }

} // hikari