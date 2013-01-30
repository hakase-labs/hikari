#include "hikari/client/game/objects/brains/TellyBrain.hpp"
#include "hikari/client/game/GameWorld.hpp"

namespace hikari {

    TellyBrain::TellyBrain(const GameWorld * const world)
        : world(world)
        , targetPosition(0, 0)
    {
        timer
            .setInterval((1.0f / 60.0f) * 5.0f)
            .setCallback(
                [&, this](Timer& timer) {
                    // make sure we run this again in 5 frames
                    timer.reset();

                    this->adjustCourse();
                }
        );
    }

    void TellyBrain::adjustCourse() {
        if(getHost()) {

            // get my position and player's position
            Vector2<float> myPosition = getHost()->getPosition();
                        
            if(this->world) {
                targetPosition = this->world->getPlayerPosition();
            }

            // see which axis has greater distance
            float distanceX = targetPosition.getX() - myPosition.getX();
            float distanceY = targetPosition.getY() - myPosition.getY();

            // horizontal axis is greater
            if(std::abs(distanceX) > std::abs(distanceY)) {
                // if distance is positive it means that player is on my right
                if(distanceX > 0.0f) {
                    getHost()->setDirection(Directions::Right);
                    getHost()->setVelocityX(0.2f);
                } else {
                    getHost()->setDirection(Directions::Left);
                    getHost()->setVelocityX(-0.2f);
                }
                getHost()->setVelocityY(0);
            } else {
                // vertical axis is greater
                if(distanceY > 0.0f) {
                    //getHost()->setDirection(Directions::Down);
                    getHost()->setVelocityY(0.2f);
                } else {
                    //getHost()->setDirection(Directions::Up);
                    getHost()->setVelocityY(-0.2f); 
                }
                getHost()->setVelocityX(0);
            }
        }
    }

    TellyBrain::~TellyBrain() {

    }

    void TellyBrain::handleCollision(Movable& body, CollisionInfo& info) {

    }

    void TellyBrain::update(const float& dt) {
        if(!timer.isRunning()) {
            timer.start();
        }
        timer.update(dt);
    }

} // hikari