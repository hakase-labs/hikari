#ifndef HIKARI_CLIENT_GAME_OBJECTS_BRAINS_OCTOPUSBATTERYBRAIN
#define HIKARI_CLIENT_GAME_OBJECTS_BRAINS_OCTOPUSBATTERYBRAIN

#include "hikari/client/game/objects/EnemyBrain.hpp"

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

namespace hikari {

    class OctopusBatteryBrain : public EnemyBrain {
    private:
        enum STATE {
            STATE_IDLE = 100,
            STATE_WALKING
        };

        STATE state;
        float timer;
        bool enteringNewState;

        sf::Clock updateTimer;
        std::vector<sf::Time> updateSamples;
    public:
        OctopusBatteryBrain();
        virtual ~OctopusBatteryBrain();

        virtual void handleCollision(Movable& body, CollisionInfo& info);

        virtual void update(const float& dt);
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_BRAINS_OCTOPUSBATTERYBRAIN