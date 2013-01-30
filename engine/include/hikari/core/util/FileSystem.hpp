#ifndef HIKARI_CORE_UTIL_FILESYSTEM
#define HIKARI_CORE_UTIL_FILESYSTEM

#include "hikari/core/Platform.hpp"
#include <memory>
#include <istream>

namespace hikari {

    class HIKARI_API FileSystem {
    public:
        struct OpenMode {
            enum Type {
                Read,
                Write
            };
        };

        static std::unique_ptr<std::istream> openFile(const std::string & fileName, OpenMode::Type openMode = OpenMode::Read);
        
        static const std::string readFileAsString(const std::string & fileName);
    };

} // hikari

#endif // HIKARI_CORE_UTIL_FILESYSTEM