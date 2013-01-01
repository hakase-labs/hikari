#ifndef HIKARI_CORE_UTIL_CLONEABLE
#define HIKARI_CORE_UTIL_CLONEABLE

#include <hikari/core/Platform.hpp>
#include <memory>

namespace hikari {

    template <typename T>
    class HIKARI_API Cloneable {
    public:
        virtual ~Cloneable() { }
    protected:
        virtual std::shared_ptr<T> clone() const = 0; 
    };
}

#endif // HIKARI_CORE_UTIL_CLONEABLE