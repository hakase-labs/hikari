#ifndef HIKARI_CLIENT_GAME_OBJECTS_FACTORYHELPERS
#define HIKARI_CLIENT_GAME_OBJECTS_FACTORYHELPERS

#include <memory>
#include <string>

namespace hikari {

    //
    // Forward declarations
    //
    class ServiceLocator;
    class EnemyFactory;
    class ItemFactory;

namespace FactoryHelpers {

    /**
     * Populates an ItemFactory with prototype instances loaded from a descriptor file.
     *
     * This function loads a descriptor file and creates instances of items to 
     * be used as prototypes. These prototypes are then injected into a specified
     * factory.
     * 
     * @param descriptorFilePath the path to the descriptor file
     * @param factory            the factory to populate
     * @param services           the service locator
     */
    void populateCollectableItemFactory(
        const std::string & descriptorFilePath,
        const std::weak_ptr<hikari::ItemFactory> & factory,
        ServiceLocator & services
    );

    /**
     *  Populates an EnemyFactory with prototype instances loaded from a descriptor file.
     *
     * This function loads a descriptor file and creates instances of enemies
     * to be used as prototypes. These prototypes are then injected into a 
     * specified factory.
     *
     * @param descriptorFilePath the path to the descriptor file
     * @param factory            the factory to populate
     * @param services           the service locator
     */
    void populateEnemyFactory(
        const std::string & descriptorFilePath,
        const std::weak_ptr<hikari::EnemyFactory> & factory,
        ServiceLocator & services
    );

} // hikari::FactoryHelpers
} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_FACTORYHELPERS