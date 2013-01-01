#include <hikari/core/game/SpriteAnimator.hpp>
#include <hikari/core/game/Animation.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace hikari {

    SpriteAnimator::SpriteAnimator(sf::Sprite &sprite)
        : Animator()
        , invertXOffset(false)
        , invertYOffset(false)
        , sprite(sprite)
        , sourceRectangle(sprite.getTextureRect()) {

    }

    SpriteAnimator::~SpriteAnimator() {

    }

    void SpriteAnimator::setInvertXOffset(const bool& flip) {
        this->invertXOffset = flip;
    }

    void SpriteAnimator::setInvertYOffset(const bool& flip) {
        this->invertYOffset = flip;
    }

    void SpriteAnimator::update(float delta) {
        Animator::update(delta);

        const AnimationFrame &currentFrame = getAnimation()->getFrameAt(getCurrentFrameIndex());
        const Rectangle2D<int> &currentFrameRectangle = currentFrame.getSourceRectangle();

        sourceRectangle.top = currentFrameRectangle.getTop();
        sourceRectangle.width = currentFrameRectangle.getWidth();
        sourceRectangle.height = currentFrameRectangle.getHeight();
        sourceRectangle.left = currentFrameRectangle.getLeft();

        sprite.setTextureRect(sourceRectangle);
        
        sprite.setOrigin(
            static_cast<float>(currentFrame.getHotspot().getX()),
            static_cast<float>(currentFrame.getHotspot().getY())
        ); 

        if(invertXOffset) {
            sprite.setOrigin(
                static_cast<float>(currentFrame.getSourceRectangle().getWidth() - currentFrame.getHotspot().getX()),
                static_cast<float>(sprite.getOrigin().y)
            ); 
        } 

        if(invertYOffset) {
            sprite.setOrigin(
                static_cast<float>(sprite.getOrigin().x),
                static_cast<float>(currentFrame.getSourceRectangle().getHeight() - currentFrame.getHotspot().getY())
            ); 
        } 
    }

} // hikari