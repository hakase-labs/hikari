#ifndef HIKARI_CLIENT_GAME_OBJECTS_EFFECTS_SCRIPTEDEFFECT
#define HIKARI_CLIENT_GAME_OBJECTS_EFFECTS_SCRIPTEDEFFECT

#include "hikari/client/game/Effect.hpp"

#include <squirrel.h>
#include <sqrat.h>

#include <memory>

namespace hikari {

    class SquirrelService;

    class ScriptedEffect : public Effect {
    private:
        static const char * FUNCTION_NAME_APPLY;
        static const char * FUNCTION_NAME_UNAPPLY;
        static const char * BASE_CLASS_NAME;

    private:
        HSQUIRRELVM vm;
        std::string effectClassName;
        Sqrat::Object instance;
        Sqrat::Table config;
        Sqrat::Function proxyApply;
        Sqrat::Function proxyUnapply;

        bool bindScriptClassInstance();

    public:
        ScriptedEffect(SquirrelService& service, const std::string& effectClassName);
        ScriptedEffect(SquirrelService& service, const std::string& effectClassName, const Sqrat::Table& config);
        ScriptedEffect(const ScriptedEffect &proto);
        virtual std::shared_ptr<Effect> clone() const;
        virtual ~ScriptedEffect();
        virtual void apply();
        virtual void unapply();
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_EFFECTS_SCRIPTEDEFFECT