#include "hikari/client/game/objects/controllers/CutSceneHeroActionController.hpp"
#include "hikari/client/game/objects/Hero.hpp" 

namespace hikari {

    CutSceneHeroActionController::CutSceneHeroActionController(const std::shared_ptr<Hero> & heroPtr)
        : hero(heroPtr) 
    {

    }

    CutSceneHeroActionController::~CutSceneHeroActionController() {

    }

    bool CutSceneHeroActionController::shouldMoveUp() const {
        return false;
    }

    bool CutSceneHeroActionController::shouldMoveRight() const {
        return false;
    }

    bool CutSceneHeroActionController::shouldMoveDown() const {
        return false;
    }

    bool CutSceneHeroActionController::shouldMoveLeft() const {
        return false;
    }

    bool CutSceneHeroActionController::shouldJump() const {
        return false;
    }

    bool CutSceneHeroActionController::shouldStopJumping() const {
        return false;
    }

    bool CutSceneHeroActionController::shouldShootWeapon() const {
        return false;
    }

    bool CutSceneHeroActionController::shouldChargeWeapon() const {
        return false;
    }

    bool CutSceneHeroActionController::shouldSlide() const {
        return false;
    }

    bool CutSceneHeroActionController::shouldStopSliding() const {
        return false;
    }

} // hikari