#ifndef HIKARI_CORE
#define HIKARI_CORE

#include "hikari/core/Platform.hpp"

extern "C"
{
    /**
        Gets the version number of Hikari.
    */
    HIKARI_API extern const char* hkrHikariVersion();
}

#endif // HIKARI_CORE