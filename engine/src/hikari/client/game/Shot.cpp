#include "hikari/client/game/Shot.hpp"
#include "hikari/client/game/objects/GameObject.hpp"

#include "hikari/core/util/Log.hpp"

namespace hikari {
    Shot::Shot(std::list<std::weak_ptr<GameObject>> trackedObjects)
        : trackedObjects(trackedObjects)
    {
        HIKARI_LOG(debug4) << "Created Shot with " << trackedObjects.size() << " tracked objects!";
    }

    Shot::Shot(const Shot & proto)
        : trackedObjects(proto.trackedObjects)
    {

    }

    bool Shot::isActive() const {
        for(auto i = std::begin(trackedObjects), end = std::end(trackedObjects); i != end; ++i) {
            const std::weak_ptr<GameObject> & trackedObjectWeak = *(i);

            if(std::shared_ptr<GameObject> trackedObject = trackedObjectWeak.lock()) {
                // If we find any objects that are still active then we must be
                // active too. If all tracked objects are no longer active then
                // the shot is not longer active.
                if(trackedObject->isActive()) {
                    return true;
                }
            }
        }

        return false;
    }
} // hikari
