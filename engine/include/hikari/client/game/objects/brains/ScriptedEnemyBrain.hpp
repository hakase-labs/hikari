#ifndef HIKARI_CLIENT_GAME_OBJECTS_BRAINS_SCRIPTEDENEMYBRAIN
#define HIKARI_CLIENT_GAME_OBJECTS_BRAINS_SCRIPTEDENEMYBRAIN

#include "hikari/client/game/objects/EnemyBrain.hpp"
#include <squirrel.h>
#include <sqrat.h>

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

namespace hikari {

    class SquirrelService;

    class ScriptedEnemyBrain : public EnemyBrain {
    private:
        HSQUIRRELVM vm;
        std::string scriptClassName;
        Sqrat::Object instance;
        Sqrat::Table instanceConfig;
        Sqrat::Function proxyAttach;
        Sqrat::Function proxyDetach;
        Sqrat::Function proxyUpdate;
        Sqrat::Function proxyHandleWorldCollision;

        sf::Clock updateTimer;
        std::vector<sf::Time> updateSamples;

    public:
        ScriptedEnemyBrain(SquirrelService& service, const std::string& scriptClassName, const Sqrat::Table& config = Sqrat::Table());
        virtual ~ScriptedEnemyBrain();

        virtual void attach(Enemy* host);
        virtual void detach();

        virtual void handleCollision(Movable& body, CollisionInfo& info);

        virtual void update(float dt);
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_BRAINS_SCRIPTEDENEMYBRAIN