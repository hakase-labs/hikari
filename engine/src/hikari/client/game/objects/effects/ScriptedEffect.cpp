#include "hikari/client/game/objects/effects/ScriptedEffect.hpp"
#include "hikari/client/scripting/SquirrelService.hpp"
#include "hikari/core/util/Log.hpp"

#include <stdexcept>

namespace hikari {

    const char * ScriptedEffect::FUNCTION_NAME_APPLY = "applyEffect";
    const char * ScriptedEffect::FUNCTION_NAME_UNAPPLY = "unapplyEffect";
    const char * ScriptedEffect::BASE_CLASS_NAME = "EffectBase";

    ScriptedEffect::ScriptedEffect(SquirrelService& service, const std::string& effectClassName, const Sqrat::Table& config)
        : vm(service.getVmInstance())
        , effectClassName(effectClassName)
        , config(config)
    {
        HIKARI_LOG(debug2) << "ScriptedEffect::ScriptedEffect()";

        if(!bindScriptClassInstance()) {
            throw std::runtime_error("ScriptedEffect could not be constructed.");
        }
    }

    ScriptedEffect::ScriptedEffect(const ScriptedEffect &proto) 
        : vm(proto.vm)
        , effectClassName(proto.effectClassName)
        , config(proto.config)
    {
        HIKARI_LOG(debug2) << "ScriptedEffect::ScriptedEffect(const ScriptedEffect &proto) Copy Constructed!";

        if(!bindScriptClassInstance()) {
            throw std::runtime_error("ScriptedEffect could not be constructed, could not bind to a class instance of type " + effectClassName);
        }
    }
    
    bool ScriptedEffect::bindScriptClassInstance() {
        bool isValid = true;

        if(effectClassName.empty()) {
            isValid = false;
        } else {
            try {
                Sqrat::Function constructor(Sqrat::RootTable(vm), effectClassName.c_str());

                if(!constructor.IsNull()) { 
                    Sqrat::Object& configRef = config;

                    instance = constructor.Evaluate<Sqrat::Object>(configRef);

                    if(!instance.IsNull()) {
                        proxyApply = Sqrat::Function(instance, FUNCTION_NAME_APPLY);
                        proxyUnapply = Sqrat::Function(instance, FUNCTION_NAME_UNAPPLY);
                    } else {
                        isValid = false;
                        HIKARI_LOG(debug2) << "Constructor for '" << effectClassName << "' did not return the correct object type.";
                    }
                } else {
                    isValid = false;
                    HIKARI_LOG(debug2) << "Could not find a constructor for '" << effectClassName << "'.";
                }
            } catch(Sqrat::Exception squirrelException) {
                isValid = false;
                HIKARI_LOG(debug1) << "Could not create an instance of '" << effectClassName << "'. Reason: " << squirrelException.Message();
            }
        }

        return isValid;
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