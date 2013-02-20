#include "hikari/client/game/objects/ItemSpawner.hpp"

#include "hikari/core/util/Log.hpp"

namespace hikari {

    ItemSpawner::ItemSpawner(const std::string & itemName)
        : Spawner()
        , itemName(itemName)
    {

    }

    ItemSpawner::~ItemSpawner() {
        // No-op
    }

    void ItemSpawner::onActivated() {
        GameObject::onActivated();

        HIKARI_LOG(debug3) << "ItemSpawner::onActivated(), id = " << getId();
    }

    void ItemSpawner::onDeactivated() {
        GameObject::onDeactivated();

        HIKARI_LOG(debug3) << "ItemSpawner::onDeactivated(), id = " << getId();
    }

} // hikari
