#include <hikari/core/util/RedirectStream.hpp>

namespace hikari {

    RedirectStream::RedirectStream(std::ios& from, std::ios& to)
        : fromStream(&from)
        , fromBuffer(from.rdbuf())
    {
        fromStream->rdbuf(to.rdbuf());
    }

    RedirectStream::~RedirectStream() {
        fromStream->rdbuf(fromBuffer);
    }

} // hikari