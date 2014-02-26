#ifndef HIKARI_CLIENT_SCRIPTING_SQUIRRELUTILS
#define HIKARI_CLIENT_SCRIPTING_SQUIRRELUTILS

#include <squirrel.h>
#include <sqstdmath.h>
#include <sqrat.h>

#include <string>

// Forward declaration of Json classes
namespace Json {
    class Value;
}

namespace hikari {
namespace SquirrelUtils {

    /**
     * Converts a JSON value (an array or object) to a Squirre array or table.
     * If conversion cannot take place or encounters an error, an exception is
     * thrown.
     *
     * @param  json the JSON value to convert
     * @return      the Squirrel equivalent of the JSON object
     */
    Sqrat::Object jsonToSquirrel(const Json::Value & json);
}
} // hikari

#endif // HIKARI_CLIENT_SCRIPTING_SQUIRRELUTILS
