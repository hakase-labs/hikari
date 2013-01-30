#include "hikari/client/game/objects/brains/ScriptedEnemyBrain.hpp"
#include "hikari/client/scripting/SquirrelService.hpp"

#include "hikari/core/util/Log.hpp"

#include <algorithm>

namespace hikari {

    ScriptedEnemyBrain::ScriptedEnemyBrain(SquirrelService& squirrel, const std::string& scriptClassName)
        : vm(squirrel.getVmInstance())
        , scriptClassName(scriptClassName)
    {
        HIKARI_LOG(debug2) << "ScriptedEnemyBrain::ScriptedEnemyBrain()";

        try {
            Sqrat::Function constructor(Sqrat::RootTable(vm), scriptClassName.c_str());

            if(!constructor.IsNull()) {
                    instance = constructor.Evaluate<Sqrat::Object>();

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

    }

    void ScriptedEnemyBrain::attach(Enemy* host) {
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

    void ScriptedEnemyBrain::update(const float& dt) {
        if(!proxyUpdate.IsNull()) {
            proxyUpdate.Execute(dt);
        }
    }

} // hikari