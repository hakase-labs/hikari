#include <hikari/client/game/objects/brains/OctopusBatteryBrain.hpp>
#include <hikari/core/game/AnimationSet.hpp>
#include <hikari/core/math/RetroVector.hpp>
#include <hikari/core/util/Log.hpp>

namespace hikari {

    OctopusBatteryBrain::OctopusBatteryBrain()
        : timer(0.0f)
        , enteringNewState(false)
    {
        state = STATE_IDLE;
    }

    OctopusBatteryBrain::~OctopusBatteryBrain() {

    }

    void OctopusBatteryBrain::update(const float& dt) {
        //updateTimer.restart();

        if(getHost() != nullptr) {
            switch(state) {
                case STATE_IDLE:
                    if(enteringNewState) {
                        enteringNewState = false;
                        getHost()->setCurrentAnimation(getHost()->getAnimationSet()->get("stopping"));
                        getHost()->setVelocityX(0);
                        getHost()->setVelocityY(0);
                    }

                    timer += dt;

                    if(timer >= 2.0f) {
                        timer = 0.0f;
                        enteringNewState = true;
                        state = STATE_WALKING;
                    }
                    break;
                case STATE_WALKING:
                    if(enteringNewState) {
                        enteringNewState = false;
                        getHost()->setCurrentAnimation(getHost()->getAnimationSet()->get("walking"));

                        const Direction &dir = getHost()->getDirection();

                        if(dir == Directions::Up) {
                            getHost()->setVelocityY(-(2.0f/* * 60.0f */));
                        } else if(dir == Directions::Right) {
                            getHost()->setVelocityX(2.0f/* * 60.0f */);
                        } else if(dir == Directions::Down) {
                            getHost()->setVelocityY(2.0f/* * 60.0f */);
                        } else if(dir == Directions::Left) {
                            getHost()->setVelocityX(-(2.0f/* * 60.0f */));
                        } else {
                            // No idea
                        }
                    }
                    break;
                default:
                    break;
            }
        }
        //updateSamples.push_back(updateTimer.getElapsedTime());

        /*if(updateSamples.size() >= 4000) {
            signed long long totalTime = 0;

            std::for_each(updateSamples.begin(), updateSamples.end(), [&](const sf::Time & t){
                totalTime += t.asMicroseconds();
            });

            double averageTime = (static_cast<double>(totalTime) / static_cast<double>(updateSamples.size()));

            std::cout << "Total time:   " << totalTime << "ms.\n";
            std::cout << "Average time: " << averageTime << "ms" << std::endl;

            updateSamples.clear();
        }*/
    }

    void OctopusBatteryBrain::handleCollision(Movable& body, CollisionInfo& info) {
        timer = 0.0f;
        state = STATE_IDLE;
        enteringNewState = true;

        getHost()->setDirection(Directions::opposite(getHost()->getDirection()));
    }

} // hikari