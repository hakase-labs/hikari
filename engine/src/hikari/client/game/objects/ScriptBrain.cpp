#include <hikari/client/game/objects/ScriptBrain.hpp>
#include <hikari/client/game/objects/GameObject.hpp>
#include <hikari/core/util/Log.hpp>
// #include <hikari/client/scripting/LuaState.hpp>

namespace hikari {

    ScriptBrain::ScriptBrain(/* LuaState &script, */ const std::string &scriptFileName) {
                /* : vm(script.getLuaVm())
        , scriptFile(scriptFileName)
        , classTable()
        , instanceTable()
        , constructorFunc()
        , updateFunc() */
        // script.runFile(scriptFileName);

        // OOLUA::pull2cpp(vm.get_ptr(), classTable);
        // bool gotTable = classTable.valid();

        // HIKARI_LOG(debug) << "ScriptBrain table = " << gotTable;
    }

    void ScriptBrain::registerObject(GameObject &object) {
        // Here's what's going on:
        //
        //  1) We grab a reference to the constructor function, "new", from the
        //     class table.
        //
        //  2) We call the constructor function, which creates a new table and
        //     leaves it at the top of the stack.
        //
        //  3) We grab the table at the top of the stack -- that's our
        //     instance data.
        //
        //  4) We grab a reference to the "update" function from the class instance.
        //
        // Note: It's also possible to modify the instance table for per-object 
        //       configuration on the Lua side of the fence. If there is a clean way
        //       to get values from the JSON config into here that would be awesome.

        /*
        bool gotBinder = classTable.safe_at("new", constructorFunc);

        HIKARI_LOG(debug) << "ScriptBrain got constructor function? " << gotBinder;

        // Do any instance-level table manipulation here...
        OOLUA::Lua_table config;
        OOLUA::new_table(vm.get_ptr(), config);
        config.set_value("behavior", "metool");
        config.set_value("goid", object.getId());

        vm.call(constructorFunc, classTable, object.getId(), config);

        HIKARI_LOG(debug) << "ScriptBrain binding the class instance to local table... ";

        OOLUA::pull2cpp(vm.get_ptr(), instanceTable);

        HIKARI_LOG(debug) << "ScriptBrain got class instance table? " << instanceTable.valid();

        bool gotUpdate = instanceTable.safe_at("update", updateFunc);

        HIKARI_LOG(debug) << "ScriptBrain got class real update? " << gotUpdate;
        */
    }

    void ScriptBrain::unregisterObject(GameObject &object) {
        //vm.call(unregisterFunc, &object);
    }

    void ScriptBrain::operator() (GameObject &object, const float &dt) {
        // vm.call(updateFunc, instanceTable, &object, dt);
        // TODO: change line above to: script.call(luaFunc, object);
    }

}