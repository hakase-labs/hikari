#include "hikari/client/scripting/SquirrelService.hpp"
#include "hikari/client/scripting/AudioServiceScriptProxy.hpp"
#include "hikari/client/game/objects/Entity.hpp"
#include "hikari/client/game/objects/Enemy.hpp"
#include "hikari/core/game/map/Room.hpp"
#include "hikari/core/game/Direction.hpp"
#include "hikari/core/util/FileSystem.hpp"
#include "hikari/core/util/Log.hpp"

#include <sqrat.h>

#include <memory>
#include <cstdarg>

namespace hikari {

    const SQInteger SquirrelService::DEFAULT_STACK_SIZE = 1024;

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

    void SquirrelService::squirrelLoggingProxyFunction(const std::string & message) {
        HIKARI_LOG(script) << message;
    }

    SquirrelService::SquirrelService(SQInteger initialStackSize)
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
            vm = sq_open(initialStackSize);
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
            // Globals
            //
            auto hikariTable = Sqrat::Table();
            auto internalTable = Sqrat::Table();
            auto audioSystemTable = Sqrat::Table();

            //
            // Bind "internal" functions (not to be used directly by end-user scripts)
            //
            internalTable.Func(_SC("log"),              &squirrelLoggingProxyFunction);
            internalTable.Func(_SC("readFileAsString"), &FileSystem::readFileAsString);

            //
            // Bind AudioSystem functions
            //
            audioSystemTable.Func(_SC("playMusic"),       &AudioServiceScriptProxy::playMusic);
            audioSystemTable.Func(_SC("stopMusic"),       &AudioServiceScriptProxy::stopMusic);
            audioSystemTable.Func(_SC("playSample"),      &AudioServiceScriptProxy::playSample);
            audioSystemTable.Func(_SC("stopAllSamples"),  &AudioServiceScriptProxy::stopAllSamples);
            audioSystemTable.Func(_SC("isMusicLoaded"),   &AudioServiceScriptProxy::isMusicLoaded);
            audioSystemTable.Func(_SC("isSamplesLoaded"), &AudioServiceScriptProxy::isSamplesLoaded);

            hikariTable.Bind(_SC("internal"), internalTable);
            hikariTable.Bind(_SC("sound"), audioSystemTable);

            Sqrat::RootTable().Bind(_SC("hikari"), hikariTable);

            //
            // Constant/Enum bindings
            //
            Sqrat::ConstTable().Enum(_SC("Directions"), Sqrat::Enumeration()
                .Const(_SC("None"), Directions::None)
                .Const(_SC("Up"), Directions::Up)
                .Const(_SC("Right"), Directions::Right)
                .Const(_SC("Down"), Directions::Down)
                .Const(_SC("Left"), Directions::Left)
            );

            //
            // Utility bindings
            //
            Sqrat::RootTable().Bind(
                _SC("Utils"), 
                Sqrat::Table()
                    .Func(_SC("getOppositeDirection"), &Directions::opposite)
            );
            
            //
            // Class bindings
            //
            Sqrat::RootTable().Bind(
                _SC("Enemy"),  
                Sqrat::Class<Enemy>()
                    .Prop<const float>(_SC("velocityX"), &Enemy::getVelocityX, &Enemy::setVelocityX) 
                    .Prop<const float>(_SC("velocityY"), &Enemy::getVelocityY, &Enemy::setVelocityY)
                    .Prop<bool>(_SC("isActive"), &Enemy::isActive, &Enemy::setActive)
                    .Prop<const bool>(_SC("isGravitated"), &Enemy::isGravitated, &Enemy::setGravitated)
                    .Prop<const bool>(_SC("isObstacle"), &Enemy::isObstacle, &Enemy::setObstacle)
                    .Prop<const Direction>(_SC("direction"), &Enemy::getDirection, &Enemy::setDirection)
                    .Func(_SC("changeAnimation"), &Enemy::changeAnimation)
                    .Func(_SC("getId"), &Enemy::getId)
                    .Func(_SC("fireWeapon"), &Enemy::fireWeapon)
                    .GlobalFunc(_SC("getX"), &EntityHelpers::getX)
                    .GlobalFunc(_SC("getY"), &EntityHelpers::getY)
                    .GlobalFunc(_SC("setX"), &EntityHelpers::setX)
                    .GlobalFunc(_SC("setY"), &EntityHelpers::setY)
                    .GlobalFunc(_SC("checkIfTileAtPositionHasAttribute"), &EntityHelpers::checkIfTileAtPositionHasAttribute)
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
            auto collected = sq_collectgarbage(vm);
            HIKARI_LOG(info) << "Collected VM garbage; " << collected << " reference cycles.";
        }
    }

} // hikari