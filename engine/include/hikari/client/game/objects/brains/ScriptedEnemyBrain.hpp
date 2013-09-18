#ifndef HIKARI_CLIENT_GAME_OBJECTS_BRAINS_SCRIPTEDENEMYBRAIN
#define HIKARI_CLIENT_GAME_OBJECTS_BRAINS_SCRIPTEDENEMYBRAIN

#include "hikari/client/game/objects/EnemyBrain.hpp"
#include <squirrel.h>
#include <sqrat.h>

namespace hikari {

    class SquirrelService;

    /**
     * Script-powered EnemyBrain. Allows an enemy to be controlled by a script.
     * Binds to an instance of a scripted enemy behavior and proxies method
     * calls to the instance.
     */
    class ScriptedEnemyBrain : public EnemyBrain {
    private:
        static const char * FUNCTION_NAME_ATTACH;
        static const char * FUNCTION_NAME_DETACH;
        static const char * FUNCTION_NAME_HANDLECOLLISION;
        static const char * FUNCTION_NAME_UPDATE;
        static const char * BASE_CLASS_NAME;

        /** A handle to the Squirrel VM */
        HSQUIRRELVM vm;

        /** The name of the class in the scripting enviornment to bind to */
        std::string scriptClassName;

        /** A handle to the instance object we create in the VM */
        Sqrat::Object instance;

        /** A table used to pass instance-level configuration to the behavior */
        Sqrat::Table instanceConfig;

        Sqrat::Function proxyAttach;
        Sqrat::Function proxyDetach;
        Sqrat::Function proxyUpdate;
        Sqrat::Function proxyHandleWorldCollision;

        /**
         * Creates an instance of the behavior in the VM and binds it to this
         * object.
         * 
         * @return true if binding was successful, false if anything went wrong
         */
        bool bindScriptClassInstance();
    public:
        /**
         * Constructor. Expects the SquirrelService and the class name to create
         * an instance of. Optionally accepts a table containing values for any
         * class-level configuration (not instance-level configuration).
         */
        ScriptedEnemyBrain(SquirrelService& service, const std::string& scriptClassName, const Sqrat::Table& config = Sqrat::Table());
        
        /**
         * Copy constructor. Causes a new instance of the scripted class to be
         * instantiated, and bind to that instance.
         */
        ScriptedEnemyBrain(const ScriptedEnemyBrain & proto);

        /**
         * Destructor.
         */
        virtual ~ScriptedEnemyBrain();

        /**
         * Clones the instance. The scripted class will be cloned as well since
         * clones cannot share an instance in the VM (that would cause a lot of
         * problems).
         */
        virtual std::unique_ptr<EnemyBrain> clone() const;

        virtual void attach(Enemy* host);
        virtual void detach();
        virtual void handleCollision(Movable& body, CollisionInfo& info);
        virtual void update(float dt);
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_BRAINS_SCRIPTEDENEMYBRAIN