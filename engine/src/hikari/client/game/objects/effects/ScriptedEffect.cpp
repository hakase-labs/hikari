#include <hikari/client/game/objects/effects/ScriptedEffect.hpp>
#include <hikari/client/scripting/SquirrelService.hpp>
#include <hikari/core/util/Log.hpp>

namespace hikari {

    const char * ScriptedEffect::FUNCTION_NAME_APPLY = "applyEffect";
    const char * ScriptedEffect::FUNCTION_NAME_UNAPPLY = "unapplyEffect";
    const char * ScriptedEffect::BASE_CLASS_NAME = "EffectBase";

    ScriptedEffect::ScriptedEffect(SquirrelService& service, const std::string& effectClassName)
        : vm(service.getVmInstance())
        , effectClassName(effectClassName)
    {
        HIKARI_LOG(debug2) << "ScriptedEffect::ScriptedEffect()";

        bindScriptClassInstance();
    }

    ScriptedEffect::ScriptedEffect(const ScriptedEffect &proto) 
        : vm(proto.vm)
        , effectClassName(proto.effectClassName)
    {
        HIKARI_LOG(debug2) << "ScriptedEffect::ScriptedEffect(const ScriptedEffect &proto) Copy Constructed!";

        bindScriptClassInstance();
    }
    
    bool ScriptedEffect::bindScriptClassInstance() {
        if(effectClassName.empty()) {
            return false;
        }

        try {
            Sqrat::Function constructor(Sqrat::RootTable(vm), effectClassName.c_str());

            if(!constructor.IsNull()) {
                    instance = constructor.Evaluate<Sqrat::Object>();

                if(!instance.IsNull()) {
                    proxyApply = Sqrat::Function(instance, FUNCTION_NAME_APPLY);
                    proxyUnapply = Sqrat::Function(instance, FUNCTION_NAME_UNAPPLY);
                } else {
                    HIKARI_LOG(debug2) << "Constructor for '" << effectClassName << "' did not return the correct object type.";
                }
            } else {
                HIKARI_LOG(debug2) << "Could not find a constructor for '" << effectClassName << "'.";
            }
        } catch(Sqrat::Exception squirrelException) {
            HIKARI_LOG(debug1) << "Could not create an instance of '" << effectClassName << "'. Reason: " << squirrelException.Message();
        }

        return true;
    }

    std::shared_ptr<Effect> ScriptedEffect::clone() const {
        return std::shared_ptr<Effect>(new ScriptedEffect(*this));
    }

    ScriptedEffect::~ScriptedEffect() {

    }

    void ScriptedEffect::apply() {
        if(!proxyApply.IsNull()) {
            proxyApply.Execute();
        }
    }

    void ScriptedEffect::unapply() {
        if(!proxyUnapply.IsNull()) {
            proxyUnapply.Execute();
        }
    }

} // hikari