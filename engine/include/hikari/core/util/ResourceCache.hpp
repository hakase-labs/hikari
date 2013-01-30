#ifndef HIKARI_CORE_UTIL_RESOURCECACHE
#define HIKARI_CORE_UTIL_RESOURCECACHE

#include "hikari/core/Platform.hpp"

#include <string>
#include <stdexcept>

#include <memory>
#include <unordered_map>

namespace hikari {

    template<typename T>
    class HIKARI_API ResourceCache {
    public:
        typedef std::shared_ptr<T> Resource;
        typedef std::unordered_map<std::string, Resource> ResourceMap;

        virtual ~ResourceCache() { }

        Resource get(const std::string &fileName) {
            auto it = resources.find(fileName);
            if(it != resources.end()) {
                return it->second;
            } else {
                cacheResource(fileName, loadResource(fileName));
                return get(fileName);
            }
        }
    protected:
        virtual Resource loadResource(const std::string &fileName) = 0;

        void cacheResource(const std::string &key, const Resource &resourcePtr) {
            resources.insert(std::make_pair(key, resourcePtr));
        }

    private: 
        ResourceMap resources;
    };

} // hikari

#endif // HIKARI_CORE_UTIL_RESOURCECACHE