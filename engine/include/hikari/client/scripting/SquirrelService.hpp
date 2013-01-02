#ifndef HIKARI_CLIENT_SCRIPTING_SQUIRRELSERVICE
#define HIKARI_CLIENT_SCRIPTING_SQUIRRELSERVICE

#include <hikari/core/util/Service.hpp>

#include <squirrel.h>
#include <sqstdmath.h>

#include <string>

namespace hikari {

    class SquirrelService : public Service {
    private:
        static void squirrelPrintFunction(HSQUIRRELVM vm, const SQChar *s, ...);
        static void squirrelErrorFunction(HSQUIRRELVM vm, const SQChar *s, ...);

        SQInteger initialStackSize;
        HSQUIRRELVM vm;

        void initVirtualMachine();
        void initStandardLibraries();
        void initBindings();

        void deinitVirtualMachine();

    public:
        SquirrelService(const SQInteger initialStackSize);
        virtual ~SquirrelService();

        void runScriptFile(const std::string & fileName);
        void runScriptString(const std::string & scriptString);
        void collectGarbage();
    };

} // hikari

#endif // HIKARI_CLIENT_SCRIPTING_SQUIRRELSERVICE