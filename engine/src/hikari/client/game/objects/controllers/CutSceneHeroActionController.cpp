#include "hikari/client/game/objects/controllers/CutSceneHeroActionController.hpp"
#include "hikari/client/game/objects/Hero.hpp"

namespace hikari {

    CutSceneHeroActionController::CutSceneHeroActionController(const std::shared_ptr<Hero> & heroPtr)
        : hero(heroPtr)
        , moveLeftFlag(false)
        , moveRightFlag(false)
        , jumpFlag(false)
        , superJumpFlag(false)
    {

    }

    CutSceneHeroActionController::~CutSceneHeroActionController() {

    }

    bool CutSceneHeroActionController::shouldMoveUp() const {
        return false;
    }

    bool CutSceneHeroActionController::shouldMoveRight() const {
        return moveRightFlag;
    }

    bool CutSceneHeroActionController::shouldMoveDown() const {
        return false;
    }

    bool CutSceneHeroActionController::shouldMoveLeft() const {
        return moveLeftFlag;
    }

    bool CutSceneHeroActionController::shouldJump() const {
        return jumpFlag;
    }

    bool CutSceneHeroActionController::shouldSuperJump() const {
        return superJumpFlag;
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

    void CutSceneHeroActionController::moveLeft() {
        moveLeftFlag = true;
    }

    void CutSceneHeroActionController::moveRight() {
        moveRightFlag = true;
    }

    void CutSceneHeroActionController::stopMoving() {
        moveLeftFlag = false;
        moveRightFlag = false;
    }

    void CutSceneHeroActionController::jump() {
        jumpFlag = true;
    }

    void CutSceneHeroActionController::superJump() {
        superJumpFlag = true;
    }

    void CutSceneHeroActionController::stopJumping() {
        jumpFlag = false;
        superJumpFlag = false;
    }
} // hikari
