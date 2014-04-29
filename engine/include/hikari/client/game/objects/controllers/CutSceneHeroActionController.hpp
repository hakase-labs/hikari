#ifndef HIKARI_CLIENT_GAME_OBJECTS_CONTROLLERS_CUTSCENEHEROACTIONCONTROLLER
#define HIKARI_CLIENT_GAME_OBJECTS_CONTROLLERS_CUTSCENEHEROACTIONCONTROLLER

#include "hikari/client/game/objects/controllers/HeroActionController.hpp"
#include <memory>

namespace hikari {

    class Hero;

    class CutSceneHeroActionController : public HeroActionController {
    private:
        std::shared_ptr<Hero> hero;
    public:
        CutSceneHeroActionController(const std::shared_ptr<Hero> & heroPtr);
        virtual ~CutSceneHeroActionController();

        virtual bool shouldMoveUp() const;
        virtual bool shouldMoveRight() const;
        virtual bool shouldMoveDown() const;
        virtual bool shouldMoveLeft() const;
        virtual bool shouldJump() const;
        virtual bool shouldSuperJump() const;
        virtual bool shouldStopJumping() const;
        virtual bool shouldShootWeapon() const;
        virtual bool shouldChargeWeapon() const;
        virtual bool shouldSlide() const;
        virtual bool shouldStopSliding() const;
    };

} // hikari

#endif // HIKARI_CLIENT_GAME_OBJECTS_CONTROLLERS_CUTSCENEHEROACTIONCONTROLLER
