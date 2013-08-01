#include "hikari/core/util/TilesetCache.hpp"
#include "hikari/core/util/FileSystem.hpp"
#include "hikari/core/util/PhysFS.hpp"
#include "hikari/core/util/Log.hpp"
#include <json/reader.h>
#include <json/value.h>

namespace hikari {

    TilesetCache::TilesetCache(const std::shared_ptr<TilesetLoader> &loader) 
        : loader(loader) 
    {

    }

    TilesetCache::Resource TilesetCache::loadResource(const std::string &fileName) {
        HIKARI_LOG(debug) << "Caching tileset: " << fileName;

        auto fs = FileSystem::openFileRead(fileName);

        if(!fs->good()) {
            std::stringstream ss;
            ss << "I/O Problem while loading tileset data from ";
            ss << fileName;

            throw std::runtime_error(ss.str().c_str());
        }

        Json::Reader reader;
        Json::Value root;

        bool success = reader.parse(*fs, root);

        if(!success) {
            std::stringstream ss;
            ss << "Problem parsing tileset data from \"";
            ss << fileName;
            ss << "\", error(s): \n";
            ss << reader.getFormatedErrorMessages();

            throw std::runtime_error(ss.str().c_str());
        }

        return loader->loadFromJson(root);
    }

}