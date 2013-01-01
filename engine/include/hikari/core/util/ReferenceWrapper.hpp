#ifndef HIKARI_CORE_UTIL_REFERENCEWRAPPER
#define HIKARI_CORE_UTIL_REFERENCEWRAPPER

namespace hikari {

    /**
        A wrapper for object references. This is to be used with algorithms or 
        other functional kinds things. It simply allows a reference to an 
        object to be passed as a parameter to things like std::bind, etc.

        This exists because there is a bug* in VC10 that does not allow the
        std::ref function to construct wrappers for abstract classes. Using
        this class will allow you to construct reference wrappers for abstract
        classes.

        Like a boss.

        * See: https://connect.microsoft.com/VisualStudio/feedback/details/727368/std-reference-wrapper-fails-to-wrap-reference-to-object-of-abstract-class
    */
    template<class T> 
    class ReferenceWrapper {
    private:
        T* obj;

    public:
        typedef T type;

        explicit ReferenceWrapper(T& t)
            : obj(nullptr)
        {
            obj = &t;
        }

        operator T& () const {
            return *obj;
        }

        T& get() const {
            return obj;
        }

        T* getPointer() const {
            return obj;
        }
    };

} // hikari

#endif // HIKARI_CORE_UTIL_REFERENCEWRAPPER