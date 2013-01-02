#ifndef HIKARI_CLIENT_GAME_OBJECTS_EFFECTS_SCRIPTEDEFFECT
#define HIKARI_CLIENT_GAME_OBJECTS_EFFECTS_SCRIPTEDEFFECT

#include <hikari/client/game/Effect.hpp>

#include <squirrel.h>
#include <sqrat.h>

#include <memory>

namespace hikari {

    class ScriptedEffect : public Effect {
    private:
        HSQUIRRELVM vm;
        std::string scriptClassName;
        Sqrat::Object instance;
        Sqrat::Function proxyApply;
        Sqrat::Function proxyUnapply;

    public:
        ScriptedEffect();
        ScriptedEffect(const ScriptedEffect &proto);
        virtual std::shared_ptr<Effect> clone() const;
        virtual ~ScriptedEffect();
        virtual void apply();
        virtual void unapply();
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_EFFECTS_SCRIPTEDEFFECT