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
        HIKARI_LOG(debug2) << "ScriptedEnemyBrain::ScriptedEnemyBrain()";

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
        } catch(Sqrat::Exception squirrelException) {
            HIKARI_LOG(debug1) << "Could not create an instance of '" << scriptClassName << "'. Reason: " << squirrelException.Message();
        }
    }
    
    ScriptedEnemyBrain::~ScriptedEnemyBrain() {
        HIKARI_LOG(debug2) << "DETROYED ScriptedEnemyBrain::ScriptedEnemyBrain()";
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