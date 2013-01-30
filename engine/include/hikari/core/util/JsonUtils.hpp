#ifndef HIKARI_CORE_UTIL_JSONUTILS
#define HIKARI_CORE_UTIL_JSONUTILS

#include "hikari/core/Platform.hpp"
#include <json/value.h>
#include <string>

namespace hikari {

    class HIKARI_API JsonUtils {
    public:
        static Json::Value loadJson(const std::string &fileName);
    };

} // hikari

#endif // HIKARI_CORE_UTIL_JSONUTILS