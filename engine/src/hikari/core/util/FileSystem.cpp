#include "hikari/core/util/FileSystem.hpp"
#include "hikari/core/util/PhysFS.hpp"

#include <physfs/ifile_stream.hpp>
#include <physfs/ofile_stream.hpp>

#include <istream>
#include <ostream>
#include <sstream>

namespace hikari {

    std::unique_ptr<std::istream> FileSystem::openFileRead(const std::string & fileName) {
        return std::unique_ptr<std::istream>(new IFileStream(fileName));
    }

    std::unique_ptr<std::ostream> FileSystem::openFileWrite(const std::string & fileName) {
        return std::unique_ptr<std::ostream>(new OFileStream(fileName));
    }

    std::string FileSystem::readFileAsString(const std::string &fileName) {
        std::stringstream result;

        if(exists(fileName)) {
            auto fs = openFileRead(fileName);

            result << fs->rdbuf();
        }

        return result.str();
    }

    std::vector<char> FileSystem::readFileAsCharBuffer(const std::string & fileName) {
        std::vector<char> buffer;

        if(exists(fileName)) {
            int length = 0;
            auto fs = openFileRead(fileName);

            fs->seekg (0, std::ios::end);
            length = static_cast<int>(fs->tellg());
            fs->seekg (0, std::ios::beg);

            buffer.reserve(length);

            fs->read(&buffer[0], length);
        }

        return buffer;
    }

    bool FileSystem::exists(const std::string& path) {
        return PhysFS::exists(path);
    }

    bool FileSystem::isDirectory(const std::string& path) {
        return PhysFS::isDirectory(path);
    }

    FileSystem::StringVector FileSystem::enumerateFiles(const std::string& directory) {
        return PhysFS::enumerateFiles(directory);
    }

    FileSystem::StringVector FileSystem::getFileListing(const std::string& directory) {
        return PhysFS::getFileListing(directory);
    }

    FileSystem::StringVector FileSystem::getDirectoryListing(const std::string& directory) {
        return PhysFS::getDirListing(directory);
    }

} // hikari