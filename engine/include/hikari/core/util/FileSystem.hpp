#ifndef HIKARI_CORE_UTIL_FILESYSTEM
#define HIKARI_CORE_UTIL_FILESYSTEM

#include "hikari/core/Platform.hpp"

#include <istream>
#include <memory>
#include <string>
#include <vector>

namespace hikari {

    class HIKARI_API FileSystem {
    public:

        typedef std::vector<std::string> StringVector;

        struct OpenMode {
            enum Type {
                Read,
                Write
            };
        };

        static std::unique_ptr<std::istream> openFile(const std::string & fileName, OpenMode::Type openMode = OpenMode::Read);
        
        static std::string readFileAsString(const std::string & fileName);
        static std::vector<char> readFileAsCharBuffer(const std::string & fileName);

        static bool exists(const std::string& path);

        static bool isDirectory(const std::string& path);

        static StringVector enumerateFiles(const std::string& directory);

        static StringVector getFileListing(const std::string& directory);

        static StringVector getDirectoryListing(const std::string& directory);
    };

} // hikari

#endif // HIKARI_CORE_UTIL_FILESYSTEM