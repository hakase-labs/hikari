#include "hikari/client/scripting/SquirrelUtils.hpp"
#include "hikari/core/util/exception/HikariException.hpp"

#include <json/value.h>

namespace hikari {
namespace SquirrelUtils {

    Sqrat::Object jsonToSquirrel(const Json::Value & json) {
        if(!json.isNull() && (json.isArray() || json.isObject())) {
            Sqrat::Table squirrelTable(Sqrat::DefaultVM::Get());

            const auto configPropertyNames = json.getMemberNames();

            for(auto propName = std::begin(configPropertyNames); propName != std::end(configPropertyNames); std::advance(propName, 1)) {
                const auto propValue = json.get(*propName, Json::Value::null);

                if(propValue.isBool()) {
                    squirrelTable.SetValue((*propName).c_str(), propValue.asBool());
                } else if(propValue.isDouble()) {
                    squirrelTable.SetValue((*propName).c_str(), propValue.asDouble());
                } else if(propValue.isIntegral()) {
                    squirrelTable.SetValue((*propName).c_str(), propValue.asInt());
                } else if(propValue.isString()) {
                    squirrelTable.SetValue((*propName).c_str(), propValue.asString());
                } else if(propValue.isNull()) {
                    squirrelTable.SetValue((*propName).c_str(), nullptr);
                }
            }

            return squirrelTable;
        }

        throw HikariException("JSON object cannot be converted to Squirrel table");
    }

} // SquirrelUtils
} // hikari