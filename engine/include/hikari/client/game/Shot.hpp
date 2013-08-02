#ifndef HIKARI_CLIENT_GAME_SHOT
#define HIKARI_CLIENT_GAME_SHOT

#include <list>
#include <memory>

namespace hikari {

    class GameObject;

    class Shot {
    private:
        std::list<std::weak_ptr<GameObject>> trackedObjects;

    public:
        explicit Shot(std::list<std::weak_ptr<GameObject>> trackedObjects);
        Shot(const Shot & proto);

        /**
         * Returns whether the shot is active or not.
         * 
         * @return true if the shot is active, false otherwise
         */
        bool isActive() const;
    };
} // hikari

#endif // HIKARI_CLIENT_GAME_SHOT