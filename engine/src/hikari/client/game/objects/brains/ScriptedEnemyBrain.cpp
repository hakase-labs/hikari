#include <hikari/client/game/objects/brains/ScriptedEnemyBrain.hpp>
#include <algorithm>

namespace hikari {

    ScriptedEnemyBrain::ScriptedEnemyBrain(HSQUIRRELVM vm, const std::string& scriptClassName)
        : vm(vm)
        , scriptClassName(scriptClassName)
    {
        std::cout << "ScriptedEnemyBrain::ScriptedEnemyBrain()" << std::endl;

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
                    std::cout << "Constructor for '" << scriptClassName << "' did not return the correct object type." << std::endl;
                }
            } else {
                std::cout << "Could not find a constructor for '" << scriptClassName << "'." << std::endl;
            }
        } catch(Sqrat::Exception squirrelException) {
            std::cout << "Could not create an instance of '" << scriptClassName << "'. Reason: " << squirrelException.Message() << std::endl;
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
        //updateTimer.restart();
        if(!proxyUpdate.IsNull()) {
            proxyUpdate.Execute(dt);
        }
        //updateSamples.push_back(updateTimer.getElapsedTime());

        /*if(updateSamples.size() >= 4000) {
            signed long long totalTime = 0;

            std::for_each(updateSamples.begin(), updateSamples.end(), [&](const sf::Time & t){
                totalTime += t.asMicroseconds();
            });

            double averageTime = (static_cast<double>(totalTime) / static_cast<double>(updateSamples.size()));

            std::cout << "Total time:   " << totalTime << "ms.\n";
            std::cout << "Average time: " << averageTime << "ms" << std::endl;
        }*/
    }

} // hikari