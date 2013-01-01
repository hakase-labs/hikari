#include <hikari/core/util/JsonUtils.hpp>
#include <hikari/core/util/FileSystem.hpp>
#include <hikari/core/util/PhysFS.hpp>
#include <json/reader.h>
#include <json/value.h>
#include <sstream>

namespace hikari {

    Json::Value JsonUtils::loadJson(const std::string &fileName) {
        Json::Value root;
        Json::Reader reader;

        //PhysFS::FileStream fs(fileName.c_str(), PhysFS::OM_READ);
        auto fs = FileSystem::openFile(fileName);

        if(!fs->good()) {
            std::stringstream ss;

            ss << "I/O problem while loading JSON object from \"";
            ss << fileName;
            ss << "\"";

            throw std::exception(ss.str().c_str());
        }

        bool success = reader.parse(*fs, root);

        if(!success) {
            std::stringstream ss;

            ss << "There was a problem parsing a JSON object from \"";
            ss << fileName;
            ss << "\". ";
            ss << reader.getFormatedErrorMessages();

            throw std::exception(ss.str().c_str());
        }

        return root;
    }

} // hikari