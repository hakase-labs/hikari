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

// The original author's license is below.
// 
/*
    SFML - Simple and Fast Multimedia Library
    Copyright (C) 2007-2013 Laurent Gomila (laurent.gom@gmail.com)

    This software is provided 'as-is', without any express or implied warranty.
    In no event will the authors be held liable for any damages arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it freely,
    subject to the following restrictions:

    1. The origin of this software must not be misrepresented;
       you must not claim that you wrote the original software.
       If you use this software in a product, an acknowledgment
       in the product documentation would be appreciated but is not required.

    2. Altered source versions must be plainly marked as such,
       and must not be misrepresented as being the original software.

    3. This notice may not be removed or altered from any source distribution.
 */