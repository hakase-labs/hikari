#ifndef HIKARI_CORE_UTIL_NONCOPYABLE
#define HIKARI_CORE_UTIL_NONCOPYABLE

#include "hikari/core/Platform.hpp"

namespace hikari {

    /**
     * A simple base class for things that should not be copied. This class will
     * prevent an instance from being copied and will cause a compiler error if
     * an attempt at copying is made.
     *
     * This class was copied from SFML by Laurent Gomila.
     */
    class HIKARI_API NonCopyable {
    protected:
        NonCopyable() {}
    private:
        NonCopyable(const NonCopyable &);
        NonCopyable & operator =(const NonCopyable &);
    };

} // hikari

#endif // HIKARI_CORE_UTIL_NONCOPYABLE