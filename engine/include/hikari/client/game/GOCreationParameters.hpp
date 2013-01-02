#ifndef HIKARI_CLIENT_GAME_GOCREATIONPARAMETERS
#define HIKARI_CLIENT_GAME_GOCREATIONPARAMETERS

#include <hikari/client/game/objects/GameObject.hpp>
#include <hikari/client/game/Direction.hpp>

#include <hikari/core/geom/Point2D.hpp>

namespace hikari {

    struct GOCreationParameters {
    public:
        int state;
        int health;
        int maxHealth;
        
        float age;
        
        bool active;
        bool affectedByGravity;

        Point2D<float> position;
        Point2D<float> velocity;

        Directions::Enum direction;
    };

}

#endif // HIKARI_CLIENT_GAME_GOCREATIONPARAMETERS