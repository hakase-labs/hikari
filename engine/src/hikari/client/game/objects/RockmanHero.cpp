/*
#include <hikari/client/game/objects/RockmanHero.hpp>

namespace hikari {
    
    RockmanHero::RockmanHero(const int& id, std::shared_ptr<AnimationSet> animations, std::shared_ptr<Input> input)
        : Hero(id)
        , animator(getSprite())
        , input(input)
    {
        this->boundingBox.setHeight(22);
        this->boundingBox.setWidth(16);

        setAnimationSet(animations);
    }
    
    void RockmanHero::setAnimationSet(std::shared_ptr<AnimationSet> animations) {
        this->animations = animations;

        sf::Sprite& sprite = getSprite();
        //sprite.SetImage();
    }

    RockmanHero::~RockmanHero() {

    }

    void RockmanHero::render(sf::RenderTarget &target) {

    }

    void RockmanHero::update(const float &dt) {

    }

}
*/