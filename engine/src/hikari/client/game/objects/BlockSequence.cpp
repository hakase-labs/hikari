#include "hikari/client/game/objects/BlockSequence.hpp"
#include "hikari/core/util/Log.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace hikari {   
    BlockSequence::BlockSequence(const BlockSequenceDescriptor & descriptor, int id)
        : GameObject(id)
        , descriptor(descriptor)
    {
        HIKARI_LOG(debug) << "BlockSequence()";
    }

    BlockSequence::~BlockSequence() {
        HIKARI_LOG(debug) << "~BlockSequence()";
    }

    void BlockSequence::onActivated() {
        HIKARI_LOG(debug2) << "BlockSequence::onActivated()";
    }

    void BlockSequence::onDeactivated() {
        HIKARI_LOG(debug2) << "BlockSequence::onDeactivated()";
    }

    void BlockSequence::update(float dt) {
        if(isActive()) {

        }
    }

    void BlockSequence::reset() {

    }

    void BlockSequence::render(sf::RenderTarget &target) {
        sf::RectangleShape shape;

        shape.setSize({
            descriptor.getBounds().getWidth(),
            descriptor.getBounds().getHeight()
        });

        shape.setPosition({
            descriptor.getBounds().getX(),
            descriptor.getBounds().getY()
        });

        shape.setFillColor({ 64, 128, 128, 128 });

        target.draw(shape);
    }

} // hikari
