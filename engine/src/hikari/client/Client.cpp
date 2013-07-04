#include "hikari/client/Client.hpp"

#include "hikari/core/util/Log.hpp"
#include "hikari/core/util/PhysFS.hpp"
 
namespace hikari {
  
    const std::string Client::APP_TITLE = "hikari";
    const std::string Client::PATH_CONTENT = "content.zip";
    const std::string Client::PATH_CUSTOM_CONTENT = "custom.zip";
 
    const unsigned int Client::SCREEN_WIDTH = 256;
    const unsigned int Client::SCREEN_HEIGHT = 240;
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
 
    }
 
    void Client::loadScriptingEnvironment() {
 
    }
 
    void Client::loadObjectTemplates() {
 
    }

    int Client::run() {
        return 0;
    }
 
} // hikari
