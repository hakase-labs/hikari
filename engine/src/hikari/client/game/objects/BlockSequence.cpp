#include "hikari/client/game/objects/BlockSequence.hpp"
#include "hikari/client/game/objects/BlockTiming.hpp"
#include "hikari/core/util/Log.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace hikari {   
    BlockSequence::BlockSequence(const BlockSequenceDescriptor & descriptor, int id)
        : GameObject(id)
        , zIndex(0)
        , step(0)
        , timer(0.0f)
        , descriptor(descriptor)
        , blockEntities()
        , outlineShape()
        , blockRects()
    {
        HIKARI_LOG(debug) << "BlockSequence()";

        const auto & bounds = getBounds();

        outlineShape.setSize({
            bounds.getWidth(),
            bounds.getHeight()
        });

        outlineShape.setPosition({
            bounds.getX(),
            bounds.getY()
        });

        outlineShape.setFillColor({ 255, 192, 10, 128 });

        const auto & blockPositions = descriptor.getBlockPositions();

        std::for_each(
            std::begin(blockPositions),
            std::end(blockPositions),
            [&](const Point2D<int> & topLeft){
                sf::RectangleShape shape;

                shape.setPosition({
                    topLeft.getX(),
                    topLeft.getY()
                });

                shape.setSize({
                    16.0f,
                    16.0f
                });

                shape.setFillColor({ 255, 32, 32, 128 });

                blockRects.push_back(shape);
            });
    }

    BlockSequence::~BlockSequence() {
        HIKARI_LOG(debug) << "~BlockSequence()";
    }

    void BlockSequence::onActivated() {
        HIKARI_LOG(debug2) << "BlockSequence::onActivated()";
        reset();
    }

    void BlockSequence::onDeactivated() {
        HIKARI_LOG(debug2) << "BlockSequence::onDeactivated()";
        reset();
    }

    void BlockSequence::update(float dt) {
        if(isActive()) {
            timer += dt;

            const auto & timing = descriptor.getTiming();
            const BlockTiming & timingStep = timing.at(step);

            // Check if we want to advance along in the sequence...
            if(timer >= timingStep.getAtTime()) {
                // Process activiations
                std::for_each(
                    std::begin(timingStep.getActivations()),
                    std::end(timingStep.getActivations()),
                    [this](const int & id) {
                        blockRects[id].setFillColor({ 32, 255, 32, 196 });
                    }
                );
                // Process deactiviations
                std::for_each(
                    std::begin(timingStep.getDeactivations()),
                    std::end(timingStep.getDeactivations()),
                    [this](const int & id) {
                        blockRects[id].setFillColor({ 255, 32, 32, 128 });
                    }
                );
                // Advance the step counter
                step++;
            }

            // We got to the end so reset.
            if(step >= timing.size()) {
                reset();
            }
        }
    }

    void BlockSequence::reset() {
        step = 0;
        timer = 0.0f;

        for(auto it = std::begin(blockRects), end = std::end(blockRects); it != end; it++) {
            it->setFillColor({ 255, 32, 32, 128 });
        }
    }

    void BlockSequence::render(sf::RenderTarget &target) {
        target.draw(outlineShape);

        for(auto it = std::begin(blockRects), end = std::end(blockRects); it != end; it++) {
            target.draw(*it);
        }
    }

    void BlockSequence::setZIndex(int index) {
        zIndex = index;
    }

    int BlockSequence::getZIndex() const {
        return zIndex;
    }

    const Rectangle2D<int> & BlockSequence::getBounds() const {
        return descriptor.getBounds();
    }

} // hikari
