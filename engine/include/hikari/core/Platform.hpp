#ifndef HIKARI_CORE_PLATFORM
#define HIKARI_CORE_PLATFORM

#if defined(_MSC_VER) && defined(HIKARI_LIBRARY_BUILD)
#define HIKARI_API _declspec(dllexport)
#else
#define HIKARI_API
#endif

#ifndef HIKARI_API
#define HIKARI_API
#endif

#endif // HIKARI_CORE_PLATFORM