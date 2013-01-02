#ifndef HIKARI_CLIENT_GAME_SCRIPTBRAIN
#define HIKARI_CLIENT_GAME_SCRIPTBRAIN

#include <string>
// #include <hikari/client/scripting/LuaState.hpp>
// #include <oolua.h>

namespace hikari {

    class GameObject;
    // class LuaState;

    class ScriptBrain {
    private:
        std::string logicClass;
        //OOLUA::Script &vm;
        //OOLUA::Lua_table classTable;
        //OOLUA::Lua_table instanceTable;
        //OOLUA::Lua_func_ref constructorFunc;
        //OOLUA::Lua_func_ref updateFunc;
        
    public:
        ScriptBrain(/* LuaState &script, */ const std::string &scriptFileName);
        void registerObject(GameObject &object);
        void unregisterObject(GameObject &object);
        void operator() (GameObject &object, const float &dt);
    };

} // hikari

#endif HIKARI_CLIENT_GAME_SCRIPTBRAIN