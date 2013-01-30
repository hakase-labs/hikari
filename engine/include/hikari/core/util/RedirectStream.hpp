#ifndef HIKARI_CORE_UTIL_REDIRECTSTREAM
#define HIKARI_CORE_UTIL_REDIRECTSTREAM

#include "hikari/core/Platform.hpp"
#include <iostream>

namespace hikari {

    /**
        A class to safely redirect one stream to another. When a RedirectStream
        goes out of scope it restores the original buffer of the stream.

        The source and idea for this came from:
            http://www.gamedev.net/topic/523200-c---freopen-vs-freopen_s/
    */
    class HIKARI_API RedirectStream {
    private:
        std::streambuf* fromBuffer;
        std::ios* fromStream;

    public:
        explicit RedirectStream(std::ios& from, std::ios& to);
        ~RedirectStream();
    };

} // hikari

#endif // HIKARI_CORE_UTIL_REDIRECTSTREAM