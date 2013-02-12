#ifndef HIKARI_CORE_UTIL_SERVICELOCATOR
#define HIKARI_CORE_UTIL_SERVICELOCATOR

#include "hikari/core/Platform.hpp"
#include "hikari/core/util/Log.hpp"
#include "hikari/core/util/exception/HikariException.hpp"
#include "hikari/core/util/exception/ServiceNotRegisteredException.hpp"
#include <map>
#include <memory>
#include <stdexcept>
#include <string>

namespace hikari {

    class Service;

    class HIKARI_API ServiceLocator {
    private:
        typedef std::shared_ptr<Service> ServicePtr;
        typedef std::map<std::string, ServicePtr> ServicePtrMap;
        ServicePtrMap services;

    public:
        void registerService(const std::string &name, const ServicePtr &pointer) {
            auto found = services.find(name);

            if(found != services.end()) {
                throw ServiceNotRegisteredException("Service name: " + name + " is already registered");
            }

            HIKARI_LOG(info) << "Registering service \"" << name << "\".";
            services.insert(std::make_pair(name, pointer));
        }

        void unregisterService(const std::string &name) {
            auto found = services.find(name);

            if(found != services.end()) {
                HIKARI_LOG(info) << "Unregistering service \"" << name << "\".";
                services.erase(found);
            }
        }

        template<class T>
        std::weak_ptr<T> locateService(const std::string &name) {
            auto found = services.find(name);

            if(found != services.end()) {
                std::shared_ptr<T> ptr = std::dynamic_pointer_cast<T>(found->second);

                if(ptr) {
                    return ptr;
                } else {
                    throw HikariException("Found service but requested type did not match (bad pointer cast). Check your types.");
                }
            } else {
                throw ServiceNotRegisteredException("Invalid service name: " + name);
            }
        }
    };

} // hikari

#endif // HIKARI_CORE_UTIL_SERVICELOCATOR