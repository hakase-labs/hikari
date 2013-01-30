#include "hikari/core/util/FileSystem.hpp"
#include "hikari/core/util/PhysFS.hpp"
#include <physfs/ifile_stream.hpp>
#include <sstream>

namespace hikari {

    std::unique_ptr<std::istream> FileSystem::openFile(const std::string & fileName, OpenMode::Type openMode) {
        return std::unique_ptr<std::istream>(new IFileStream(fileName));
    }

    const std::string FileSystem::readFileAsString(const std::string &fileName) {
        std::stringstream result;

        if(PhysFS::exists(fileName)) {
            auto fs = FileSystem::openFile(fileName);

            result << fs->rdbuf();
        }

        return result.str();
    }

} // hikari