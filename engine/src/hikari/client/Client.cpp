#include "hikari/client/Client.hpp"

#include "hikari/core/util/FileSystem.hpp"
#include "hikari/core/util/Log.hpp"
#include "hikari/core/util/PhysFS.hpp"

#include <json/reader.h>
 
namespace hikari {
  
    const std::string Client::APP_TITLE           = "hikari";
    const std::string Client::PATH_CONTENT        = "content.zip";
    const std::string Client::PATH_CUSTOM_CONTENT = "custom.zip";
    const std::string Client::PATH_CONFIG_FILE    = "conf.json";
 
    const unsigned int Client::SCREEN_WIDTH          = 256;
    const unsigned int Client::SCREEN_HEIGHT         = 240;
    const unsigned int Client::SCREEN_BITS_PER_PIXEL = 32;
 
    Client::Client(int argc, char** argv)
        : clientConfig()
        , services()
    {
        initLogging(argc, argv);
        initFileSystem(argc, argv);
        initConfig();
        initServices();
        initGame();
    }
 
    void Client::initConfig() {
        if(FileSystem::exists(PATH_CONFIG_FILE)) {
            auto fs = FileSystem::openFile(PATH_CONFIG_FILE);

            Json::Reader reader;
            Json::Value value;

            bool success = reader.parse(*fs, value, false);

            if(!success) {
                HIKARI_LOG(info) << "Configuration file could not be found or was corrupt, using defaults.";
            } else {
                clientConfig = ClientConfig(value);
            }
        } else {
            HIKARI_LOG(fatal) << "Couldn't find configuration file '" << PATH_CONFIG_FILE << "'";
        }
    }
 
    void Client::initFileSystem(int argc, char** argv) {
        // Create virtual file system
        PhysFS::init(argv[0]);
        PhysFS::addToSearchPath(PhysFS::getBaseDir());

        // Load standard content
        try {
            PhysFS::addToSearchPath(PhysFS::getBaseDir() + PATH_CONTENT);
        } catch(PhysFS::Exception & ex) {
            HIKARI_LOG(error) << "Couldn't load content package. Reason: " << ex.what();
        }

        // Load custom content (if it exists)
        try {
            PhysFS::addToSearchPath(PhysFS::getBaseDir() + PATH_CUSTOM_CONTENT);
        } catch(PhysFS::Exception & ex) {
            HIKARI_LOG(warning) << "(This is normal) Couldn't load custom content package. Reason: " << ex.what();
        }

        PhysFS::setWriteDir(PhysFS::getBaseDir());
    }
 
    void Client::initGame() {
        loadScriptingEnvironment();
        loadObjectTemplates();
    }
 
    void Client::initLogging(int argc, char** argv) {
        ::hikari::Log::setReportingLevel(debug4);
    }
 
    void Client::initServices() {
 
    }
 
    void Client::deinitFileSystem() {
        PhysFS::deinit();
    }
 
    void Client::loadScriptingEnvironment() {
 
    }
 
    void Client::loadObjectTemplates() {
 
    }

    int Client::run() {
        return 0;
    }
 
} // hikari
