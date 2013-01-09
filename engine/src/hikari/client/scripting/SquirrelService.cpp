#include <hikari/client/scripting/SquirrelService.hpp>
#include <hikari/client/game/objects/Entity.hpp>
#include <hikari/client/game/objects/Enemy.hpp>
#include <hikari/core/game/map/Room.hpp>
#include <hikari/core/game/Direction.hpp>
#include <hikari/core/util/FileSystem.hpp>

#include <sqrat.h>

#include <memory>
#include <cstdarg>

namespace hikari {

    void SquirrelService::squirrelPrintFunction(HSQUIRRELVM vm, const SQChar *s, ...) {
        va_list vl;
        va_start(vl, s);
        vprintf(s, vl);
        va_end(vl);
        printf("\n");
    }

    void SquirrelService::squirrelErrorFunction(HSQUIRRELVM vm, const SQChar *s, ...) {
        va_list vl;
        va_start(vl, s);
        vprintf(s, vl);
        va_end(vl);
        printf("\n");
    }

    SquirrelService::SquirrelService(const SQInteger initialStackSize = 1024)
        : Service()
        , initialStackSize(initialStackSize)
        , vm(nullptr)
    {
        initVirtualMachine();
        initStandardLibraries();
        initBindings();
    }

    SquirrelService::~SquirrelService() {
        deinitVirtualMachine();
    }

    void SquirrelService::initVirtualMachine() {
        if(!vm) {
            vm = sq_open(1024);
            sq_setprintfunc(vm, squirrelPrintFunction, squirrelPrintFunction);
        }
    }
    
    void SquirrelService::initStandardLibraries() {
        if(vm) {
            sq_pushroottable(vm);
            sqstd_register_mathlib(vm); 
            sq_pop(vm, 1);
        }
    }

    void SquirrelService::deinitVirtualMachine() {
        if(vm) {
            sq_close(vm);
        }
    }

    void SquirrelService::initBindings() {
        if(vm) {
            Sqrat::DefaultVM::Set(vm);

            //
            // Constant/Enum bindings
            //
            Sqrat::ConstTable().Enum("Directions", Sqrat::Enumeration()
                .Const("None", Directions::None)
                .Const("Up", Directions::Up)
                .Const("Right", Directions::Right)
                .Const("Down", Directions::Down)
                .Const("Left", Directions::Left)
            );

            //
            // Utility bindings
            //
            Sqrat::RootTable().Bind(
                "Utils", 
                Sqrat::Table()
                    .Func("getOppositeDirection", &Directions::opposite)
            );
            
            //
            // Class bindings
            //
            Sqrat::RootTable().Bind(
                "Enemy", 
                Sqrat::Class<Enemy>()
                    .Prop<const float>("velocityX", &Enemy::getVelocityX, &Enemy::setVelocityX) 
                    .Prop<const float>("velocityY", &Enemy::getVelocityY, &Enemy::setVelocityY)
                    .Prop<const bool>("isActive", &Enemy::isActive, &Enemy::setActive)
                    .Prop<const bool>("isGravitated", &Enemy::isGravitated, &Enemy::setGravitated)
                    .Prop<const bool>("isObstacle", &Enemy::isObstacle, &Enemy::setObstacle)
                    .Prop<const Direction>("direction", &Enemy::getDirection, &Enemy::setDirection)
                    .Func("changeAnimation", &Enemy::changeAnimation)
                    .GlobalFunc("getX", &EntityHelpers::getX)
                    .GlobalFunc("getY", &EntityHelpers::getY)
                    .GlobalFunc("setX", &EntityHelpers::setX)
                    .GlobalFunc("setY", &EntityHelpers::setY)
            );
        }
    }

    const HSQUIRRELVM SquirrelService::getVmInstance() {
        return vm;
    }

    void SquirrelService::runScriptFile(const std::string & fileName) {
        if(vm) {
            auto fileContents = FileSystem::readFileAsString(fileName);

            if(!fileContents.empty()) {
                runScriptString(fileContents);
            } else {
                // TODO: Need to handle this with an exception, etc.
            }
        }
    }

    void SquirrelService::runScriptString(const std::string & scriptString) {
        if(vm) {
            // TODO: See if the Sqrat::Script object can be used here; what is its internal object referring to?
            Sqrat::Script script;
            script.CompileString(scriptString);
            script.Run();
        }
    }

    void SquirrelService::collectGarbage() {
        if(vm) {
            sq_collectgarbage(vm);
        }
    }

} // hikari